#include <iostream>

#include "IDisk.h"

#include <sstream>

#include "ITerminal.h"
#include "CLI.h"
#include "DeviceManager.h"
#include "Command.h"

int main()
{
    ITerminal *t = DeviceManager::instance().getTerminal("term 1");
    CLI c(t);
	c.registerCommand(new Dir("dir"));
	c.registerCommand(new PrintEnv("printenv"));
	c.registerCommand(new Exit("exit"));
	c.registerCommand(new Cd("cd"));
	c.registerCommand(new Cat("cat"));
    t->inputPrompt();
    c.disposeCommands();
    return (0);
}

