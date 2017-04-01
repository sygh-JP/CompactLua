#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#include <cassert>
#include <iostream>
#include <conio.h>

// 後方互換ワークアラウンドを記述したヘッダー "lua_back_compat.h", "luabind_back_compat.hpp" を先にインクルードしておくこと。
// Visual C++ では "/FI" オプションを使うのが便利。
#include <luabind/luabind.hpp>

///////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	auto* lua = lua_open();
	assert(lua != nullptr);
	luaL_openlibs(lua);

	luabind::open(lua);

	extern void LuabindTest01(lua_State* lua);
	LuabindTest01(lua);
	extern void LuabindTest02(lua_State* lua);
	LuabindTest02(lua);

	lua_close(lua);
	lua = nullptr;

	std::cout << "\nScript finished. Press any...\n";
	_getch();

	return 0;
}
