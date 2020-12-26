#pragma once

// lua_strlen() and lua_equal() are used in Luabind, but these have been removed in Lua 5.4.
// Still we can enable the compatible macros by defining LUA_COMPAT_5_3 before including "luaconf.h".

#include "lua.hpp"

#if (LUA_VERSION_NUM >= 502)
// Lua 5.1 compatible functions.
inline void lua_setfenv(lua_State* L, int idx) { lua_setuservalue(L, idx); }
inline void lua_getfenv(lua_State* L, int idx) { lua_getuservalue(L, idx); }
inline lua_State* lua_open() { return luaL_newstate(); }

#if (LUA_VERSION_NUM >= 504)
inline int lua_resume(lua_State* L, int nargs)
{
	int nresults = 0;
	return lua_resume(L, nullptr, nargs, &nresults);
}
#else
inline int lua_resume(lua_State* L, int nargs) { return lua_resume(L, nullptr, nargs); }
#endif

struct DummyClassForLegacyLuaGlobalsIndex {};
const DummyClassForLegacyLuaGlobalsIndex LUA_GLOBALSINDEX;
inline void lua_pushvalue(lua_State* L, const DummyClassForLegacyLuaGlobalsIndex&) { lua_pushglobaltable(L); }
inline void lua_settable(lua_State* L, const DummyClassForLegacyLuaGlobalsIndex&) { lua_settable(L, LUA_REGISTRYINDEX); }
inline void lua_gettable(lua_State* L, const DummyClassForLegacyLuaGlobalsIndex&) { lua_gettable(L, LUA_REGISTRYINDEX); }
#else
// Lua 5.2 compatible functions.
inline void lua_pushglobaltable(lua_State* L) { lua_pushvalue(L, LUA_GLOBALSINDEX); }
#endif

// Luabind 0.9.1 uses "boost::operator" but it has been moved to "boost::iterators::operator" in Boost 1.57.0 or later.
// As a result, many compilation errors will occur at the macro "LUABIND_OPERATOR_ADL_WKND" in "luabind/object.hpp".
// One of the best and wisest solutions is to modify the source code of Luabind directly.
// As an alternative way, the following workaround can avoid modifying it but is unbeautiful and pollutes the namespace "boost".

#include <boost/operators.hpp>
namespace luabind
{
	namespace detail
	{
		// Forward declaration
		template<typename T> class basic_iterator;
	}
}
namespace boost
{
	template<typename T> bool operator ==(
		const luabind::detail::basic_iterator<T>& x,
		const luabind::detail::basic_iterator<T>& y)
	{ return boost::iterators::operator ==(x, y); }

	template<typename T> bool operator !=(
		const luabind::detail::basic_iterator<T>& x,
		const luabind::detail::basic_iterator<T>& y)
	{ return boost::iterators::operator !=(x, y); }
}
