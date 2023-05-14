/**
  * @file cmd_line_parser.h
  * @author Artiom N.(cl)2019
  * @brief Command line parser class definition.
  *
  */

#pragma once

#include <functional>
#include <args.hxx>

#include "types.h"


namespace tsw
{

class CmdLineParser
{
public:
    typedef std::function<void(args::Subparser &)> SubParser;

    CmdLineParser(int argc, const char** argv, SubParser module_options_parser);

public:
    const NameValueMap& get_fields() const;

private:
    void parse_command_line();

private:
    const NameValueMap fields_;
};

}
