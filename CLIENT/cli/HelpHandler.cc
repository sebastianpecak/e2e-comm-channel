#include "HelpHandler.h"
#include <cstdio>

void HelpHandler::Execute(const Tokenizer&)
{
    printf("Help:\n");
    printf("    This is E2EEM tool's client application used to send and receive messages from relay server.\n");
    printf("    It also delivers user managment tools.\n");
    printf("Usage:\n");
    printf("    [E2E_CLI_DB_PATH=/path/to/db] ./cli.bin\n");
    printf("Commands list:\n");
    printf("    login <user-id>              - Start new session as given user.\n");
    printf("    logout                       - End current user's session.\n");
    printf("    create-user <user-id>        - Create new user with gievn user-id.\n");
    printf("    export-user <user-id>        - Exports user's public key.\n");
    printf("    import-user <compact-record> - Imports user from compact-record.\n");
    printf("    send <user-id>               - Send new message to user-id.\n");
    printf("    recv                         - Get all current user's messages from server.\n");
    printf("    svr-info                     - Get server name and version.\n");
    printf("    help                         - Display commands list.\n");
    printf("    exit                         - Logout and exit gracefully.\n");
}
