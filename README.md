# CompactLua
Compact Lua by sygh.

This is a library forked from Lua in order to use it in Windows Store Apps (WinRT-based applications).

## Development Environment (開発環境)
* Microsoft Visual Studio 2012 Update 5
* Lua 5.3.4
* Luabind 0.9.1
* Boost C++ 1.63.0

## Target Environment (ターゲット環境)
* Windows Vista/Windows 7/Windows 8.1/Windows 10 (Desktop)
* Windows 8.1/Windows 10 (WinRT)

## How to Build (ビルド方法)
1. Download Lua 5.3.4 and copy "src" folder to "lua5"
1. Download Luabind 0.9.1 and copy "luabind" and "src" folders to "luabind"
1. Build Lua by "lua5/_sln_vs2012/lua_vs2012.sln" or "lua5/_sln_vs2012_win8/lua_vs2012_win8.sln"
1. Build Luabind by "luabind/_sln_vs2012/LuabindLib_vs2012.sln" or "luabind/_sln_vs2012_win8/LuabindLib_vs2012_win8.sln"
1. Build test project by "StoreAppLuaTest1/StoreAppLuaTest1.sln"

2017-04-01, sygh.
