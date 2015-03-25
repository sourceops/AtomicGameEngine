
#include <Atomic/Core/ProcessUtils.h>
#include <Atomic/IO/Log.h>
#include <Atomic/Engine/Engine.h>

#include <ToolCore/ToolSystem.h>
#include <ToolCore/License/LicenseSystem.h>
#include <ToolCore/Command/Command.h>
#include <ToolCore/Command/CommandParser.h>

#include "AtomicTool.h"

using namespace ToolCore;

DEFINE_APPLICATION_MAIN(AtomicTool::AtomicTool);

namespace AtomicTool
{

AtomicTool::AtomicTool(Context* context) :
    Application(context)
{

}

AtomicTool::~AtomicTool()
{

}

void AtomicTool::Setup()
{
    const Vector<String>& arguments = GetArguments();

    for (unsigned i = 0; i < arguments.Size(); ++i)
    {
        if (arguments[i].Length() > 1)
        {
            String argument = arguments[i].ToLower();
            String value = i + 1 < arguments.Size() ? arguments[i + 1] : String::EMPTY;

            if (argument == "--cli-data-path")
            {
                if (!value.Length())
                    ErrorExit("Unable to parse --cli-data-path");

                cliDataPath_ = value;
            }
        }

    }

    if (!cliDataPath_.Length())
        ErrorExit("Unable to parse --data-path");

    engineParameters_["Headless"] = true;
    engineParameters_["ResourcePaths"] = "";
}

void AtomicTool::HandleCommandFinished(StringHash eventType, VariantMap& eventData)
{
    GetSubsystem<Engine>()->Exit();
}

void AtomicTool::HandleCommandError(StringHash eventType, VariantMap& eventData)
{
    String error = "Command Error";

    const String& message = eventData[CommandError::P_MESSAGE].ToString();
    if (message.Length())
        error = message;

    ErrorExit(error);
}

void AtomicTool::Start()
{
    // Subscribe to events
    SubscribeToEvent(E_COMMANDERROR, HANDLER(AtomicTool, HandleCommandError));
    SubscribeToEvent(E_COMMANDFINISHED, HANDLER(AtomicTool, HandleCommandFinished));

    const Vector<String>& arguments = GetArguments();

    ToolSystem* tsystem = new ToolSystem(context_);
    context_->RegisterSubsystem(tsystem);
    tsystem->SetDataPath(cliDataPath_);

    SharedPtr<CommandParser> parser(new CommandParser(context_));
    SharedPtr<Command> cmd(parser->Parse(arguments));
    if (!cmd)
    {
        String error = "No command found";

        if (parser->GetErrorMessage().Length())
            error = parser->GetErrorMessage();

        ErrorExit(error);
    }

    // BEGIN LICENSE MANAGEMENT
    GetSubsystem<LicenseSystem>()->Initialize();
    // END LICENSE MANAGEMENT

    cmd->Run();
}

void AtomicTool::Stop()
{

}


}


