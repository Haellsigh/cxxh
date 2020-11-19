#include <filesystem>
#include <iostream>

#include <boost/program_options.hpp>
#include <fmt/format.h>

#include "creators/class.hh"
#include "creators/header.hh"
#include "creators/source.hh"
#include "creators/utilities.hh"

namespace po = boost::program_options;
using namespace cxxh;

template <class T> std::ostream &operator<<(std::ostream &os, const std::vector<T> &v)
{
    copy(v.begin(), v.end(), std::ostream_iterator<T>(os, " "));
    return os;
}

/*!
 * \brief Checks that 'opt1' and 'opt2' are not specified at the same time.
 */
void conflicting_options(const po::variables_map &vm, const char *opt1, const char *opt2)
{
    if (vm.count(opt1) && !vm[opt1].defaulted() && vm.count(opt2) && !vm[opt2].defaulted())
        throw std::logic_error(std::string("Conflicting options '") + opt1 + "' and '" + opt2 + "'.");
}

/*!
 * \brief Checks that option opt equals one of these values.
 *
 * \tparam T The type of the option.
 */
template <typename T, typename... Args> void is_one_of(const po::variables_map &vm, const char *opt, Args... these)
{
    auto value = vm[opt].as<T>();
    if (((value != these) && ...))
    {
        throw std::domain_error(fmt::format("Invalid value '{}' for option '{}'", value, opt));
    }
}

/**
 * Usage:
 * Create a source file in folder f1/f2 in namespace n1::n2 called c1
 * cxxh [folders/.../][]
 *
 */
int main(int argc, char **argv)
{
    try
    {
        po::options_description generic("Generic options");
        generic.add_options()                                                                         //
            ("help", "prints usage message")                                                          //
            ("version", "prints version information")                                                 //
            ("verbose", po::value<bool>()->zero_tokens()->default_value(false)->implicit_value(true), //
             "shows the list of files created")                                                       //
            ("dry-run", po::value<bool>()->zero_tokens()->default_value(false)->implicit_value(true), //
             "do not actually create files")                                                          //
            ("force", po::value<bool>()->zero_tokens()->default_value(false)->implicit_value(true),   //
             "overwrite existing files");                                                             //

        po::options_description usage("Creation options");
        usage.add_options()                                                   //
            ("class,c", po::value<std::vector<std::string>>()->multitoken(),  //
             "creates a class")                                               //
            ("header,h", po::value<std::vector<std::string>>()->multitoken(), //
             "creates a header file")                                         //
            ("source,s", po::value<std::vector<std::string>>()->multitoken(), //
             "creates a source file");                                        //

        po::options_description options("Options");
        options.add_options()                                                       //
            ("folder,f", po::value<std::string>(),                                  //
             "creates everything in arg folder")                                    //
            ("lowercase,l", po::value<bool>()->default_value(true),                 //
             "creates lowercase filenames, default is true")                        //
            ("include-guard,ig", po::value<std::string>()->default_value("pragma"), //
             "include guard for header files, one of: pragma, ifndef")              //
            ("header-ext,he", po::value<std::string>()->default_value("hh"),        //
             "header files extension, default is hh")                               //
            ("source-ext,se", po::value<std::string>()->default_value("cc"),        //
             "source files extension, default is cc");                              //

        generic.add(usage).add(options);

        po::variables_map vm;
        po::store(po::command_line_parser(argc, argv).options(generic) /*.positional(p)*/.run(), vm);
        po::notify(vm);

        bool class_command = vm.count("class");
        bool header_command = vm.count("header");
        bool source_command = vm.count("source");

        is_one_of<std::string>(vm, "include-guard", "pragma", "ifndef");

        if (vm.count("version"))
        {
            std::cout << "cxxh v0.4.0\n";
            return 0;
        }

        if (vm.count("help") || !(class_command || header_command || source_command))
        {
            std::cout << "Usage: cxxh [options] <creation option> args\n";
            generic.print(std::cout);
            return 0;
        }

        if (vm.count("header-ext"))
        {
            auto he = vm["header-ext"].as<std::string>();
            if (Creators::utilities::contains(he, "."))
            {
                throw std::invalid_argument(fmt::format("Invalid header extension: {}", he));
            }
        }

        if (vm.count("source-ext"))
        {
            auto se = vm["source-ext"].as<std::string>();
            if (Creators::utilities::contains(se, "."))
            {
                throw std::invalid_argument(fmt::format("Invalid source extension: {}", se));
            }
        }

        if (class_command)
        {
            Creators::Class::create(vm);
        }

        if (header_command)
        {
            Creators::Header::create(vm);
        }

        if (source_command)
        {
            Creators::Source::create(vm);
        }
    }
    catch (std::filesystem::filesystem_error &e)
    {
        if (e.code() == std::errc::file_exists)
        {
            std::cout << "File " << e.path1().lexically_relative(std::filesystem::current_path()).string()
                      << " already exists\n";
            std::cout << "Run with --force to overwrite\n";
        }
    }
    catch (std::exception &e)
    {
        std::cout << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
