#include <iostream>
#include <string>
#include <unistd.h>

#include "plugbot.hpp"
#include "plugin_manager.hpp"

using namespace std;

int main()
{
    PlugBot bot{};

    bot.add_server("freenode", "irc.freenode.org","6667", "sasasa", "guest dbg fbbas :abba sabbT");
    bot.connect_to_server("freenode");

    string channel = "#67ajutasd";

    bot.exec_command("freenode", Command{ "JOIN", { channel, "" }});
    bot.exec_command("freenode", Command{ "PRIVMSG", { channel, "YO! MR. WHITE!" }});

    sleep(25);

    bot.exec_command("freenode", Command{ "PART", { channel }});

    sleep(5);
}

