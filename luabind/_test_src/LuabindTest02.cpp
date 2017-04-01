
#include <luabind/luabind.hpp>

#include <iostream>
#include <sstream>

///////////////////////////////////////////////////////////////////////////////
void DoLuaScriptString(lua_State* state, char const* str)
{
	if (luaL_loadbuffer(state, str, std::strlen(str), str))
	{
		std::string err(lua_tostring(state, -1));
		lua_pop(state, 2);
		throw err;
	}
	if (lua_pcall(state, 0, 0, -2))
	{
		std::string err(lua_tostring(state, -1));
		lua_pop(state, 2);
		throw err;
	}
	lua_pop(state, 1);
}

///////////////////////////////////////////////////////////////////////////////
namespace MyLuaObjects
{
	class Person
	{
	private:
		std::string name_;
		int age_;

	public:
		Person(const char* name, int age) : name_(name), age_(age) {}
		virtual ~Person() {}

		const char* getName() const { return name_.c_str(); }
		int getAge() const { return age_; }
	};

	void print(const char* txt)
	{
		std::cout << txt << std::endl;
	}
}

///////////////////////////////////////////////////////////////////////////////
void LuabindTest02(lua_State* lua)
{
	{
		using namespace MyLuaObjects;
		luabind::module(lua)[
			// 関数の登録。
			luabind::def("print", print),
				// クラスの登録。
				luabind::class_<Person>("Person")
				.def(luabind::constructor<const char*, int>())
				.def("getName", &Person::getName)
				.def("getAge", &Person::getAge)
		];
	}
	// Lua スクリプトの実行。
	DoLuaScriptString(lua,
		"me = Person('NijiharaInku', 17)\n"
		"print( me:getName() .. '(' .. me:getAge() .. ')' )\n"
		);
}
