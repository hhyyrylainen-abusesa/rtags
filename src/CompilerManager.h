#ifndef CompilerManager_h
#define CompilerManager_h

#include <rct/Map.h>
#include <rct/List.h>
#include <rct/Path.h>
#include <rct/String.h>
#include "Server.h"

namespace CompilerManager
{
void init(const Server::Options &options);
List<Path> compilers();
List<String> flags(const Path &compiler);
void addAliases(const Set<Path> &paths);
Set<Path> aliases(const Path &path);
}

#endif
