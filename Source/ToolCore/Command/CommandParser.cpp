
#include "CommandParser.h"

#include "NewProjectCmd.h"

namespace ToolCore
{

CommandParser::CommandParser(Context* context) : Object(context)
{

}

CommandParser::~CommandParser()
{

}

Command* CommandParser::Parse(const Vector<String>& arguments)
{
    Command* cmd = NULL;

    for (unsigned i = 0; i < arguments.Size(); ++i)
    {
        if (arguments[i].Length())
        {
            String argument = arguments[i].ToLower();

            if (argument == "new")
            {
                cmd = new NewProjectCmd(context_);
            }
        }

        if (cmd)
        {
            if (cmd->Parse(arguments, i, errorMsg_))
                return cmd;
        }

    }

    return NULL;
}

}
