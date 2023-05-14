/**
  * @file cmd_line_parser.cpp
  * @author Artiom N.(cl)2019
  * @brief Command line parser class implementation.
  *
  */

#include <tsw/cmd_line_parser.h>
#include <tsw/types.h>


namespace tsw
{

CmdLineParser::CmdLineParser(int argc, const char** argv, SubParser module_options_parser)
{

}

const NameValueMap& CmdLineParser::get_fields() const
{
    return fields_;
}

void CmdLineParser::parse_command_line()
{
    args::ArgumentParser parser("This is a test program.", "This goes after the options.");
    args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});

    try
    {
//        parser.ParseCLI(command_line_.size(), command_line_);
    }
    catch (const args::Help&)
    {
//        std::cout << parser;
    }
    catch (const args::ParseError &e)
    {
//        std::cerr << e.what() << std::endl;
//        std::cerr << parser;
    }
}

} // namespace tsw
