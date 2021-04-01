#include "Application.h"
#include "LoginHandler.h"
#include "LogoutHandler.h"
#include "CreateUserHandler.h"
#include "ExportUserHandler.h"
#include "ImportUserHandler.h"
#include "SendHandler.h"
#include "RecvHandler.h"
#include "SvrInfoHandler.h"
#include "HelpHandler.h"
#include "EmptyHandler.h"

Application::Application() :
    _cli(&_session)
{
    _cli.RegisterHandler("login"      , new LoginHandler(&_session, &_keyStore, &_cli));
    _cli.RegisterHandler("logout"     , new LogoutHandler(&_session));
    _cli.RegisterHandler("create-user", new CreateUserHandler(&_keyStore, &_cli));
    _cli.RegisterHandler("export-user", new ExportUserHandler(&_keyStore));
    _cli.RegisterHandler("import-user", new ImportUserHandler(&_keyStore));
    _cli.RegisterHandler("send"       , new SendHandler(&_session, &_keyStore, &_svrIface));
    _cli.RegisterHandler("recv"       , new RecvHandler(&_session, &_keyStore, &_svrIface));
    _cli.RegisterHandler("svr-info"   , new SvrInfoHandler(&_svrIface));
    _cli.RegisterHandler("help"       , new HelpHandler());
    _cli.RegisterHandler("exit"       , new EmptyHandler());
}

void Application::Run()
{
    // Run command line interface until exit command it typed.
    do
    {
        _cli.ProcessCommand();
    } while (_cli.GetLastCommand() != "exit");
}
