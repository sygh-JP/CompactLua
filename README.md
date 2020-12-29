# CompactLua
Compact Lua by sygh.

This is a library forked from Lua in order to use it in Windows Store Apps (WinRT-based applications).

## Development Environment (開発環境)
* Microsoft Visual Studio 2012 Update 5
* [Lua](http://www.lua.org/) 5.4.2
* [Luabind](https://www.rasterbar.com/products/luabind.html) 0.9.1
* [Boost C++](https://www.boost.org/) 1.75.0

## Target Environment (ターゲット環境)
* Windows Vista/Windows 7/Windows 8.1/Windows 10 (Desktop)
* Windows 8.1/Windows 10 (WinRT)

## How to Build (ビルド方法)
1. Download **Lua** and copy "src" folder to "lua5"
1. Download **Luabind** and copy "luabind" and "src" folders to "luabind"
1. Install **Boost C++** (not required to build) and append the global include and library directory paths to it
1. Build Lua by "lua5/_sln_vs2012/lua_vs2012.sln" or "lua5/_sln_vs2012_win8/lua_vs2012_win8.sln"
1. Build Luabind by "luabind/_sln_vs2012/LuabindLib_vs2012.sln" or "luabind/_sln_vs2012_win8/LuabindLib_vs2012_win8.sln"
1. Build test project by "StoreAppLuaTest1/StoreAppLuaTest1.sln"

You can append the global include and library directory paths above by editing "Microsoft.Cpp.Win32.user.props" or "Microsoft.Cpp.x64.user.props" in "%LocalAppData%\Microsoft\MSBuild\v4.0".

2020-12-29, sygh.
