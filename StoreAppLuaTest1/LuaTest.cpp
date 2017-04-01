#include "pch.h"
#include "LuaTest.h"


WriteStringDelegate^ MyLua::m_outputWriter;
WriteStringDelegate^ MyLua::m_errorWriter;


MyLua::MyLua()
	: m_lua()
{
	m_lua = lua_open();
	_ASSERTE(m_lua != nullptr);
	luaL_openlibs(m_lua);

	luabind::open(m_lua);
}

MyLua::~MyLua()
{
	if (m_lua)
	{
		lua_close(m_lua);
	}
}

// コピーコンストラクタよりもムーブ コンストラクタが優先されることを利用する。
static std::vector<char> ConvertUtf16toUtf8(const wchar_t* srcText)
{
	_ASSERTE(srcText != nullptr);
	const int textLen = static_cast<int>(wcslen(srcText));
	const int reqSize = ::WideCharToMultiByte(CP_UTF8, 0, srcText, textLen, nullptr, 0, nullptr, nullptr);
	if (reqSize > 0)
	{
		std::vector<char> buff(reqSize + 1); // 最後の + 1 は必須らしい。終端 null を含まないサイズが返るらしい。
		::WideCharToMultiByte(CP_UTF8, 0, srcText, textLen, &buff[0], reqSize, nullptr, nullptr);
		return buff;
	}
	else
	{
		return std::vector<char>();
	}
}

static std::vector<wchar_t> ConvertUtf8toUtf16(const char* srcText)
{
	_ASSERTE(srcText != nullptr);
	const int textLen = static_cast<int>(strlen(srcText));
	const int reqSize = ::MultiByteToWideChar(CP_UTF8, 0, srcText, textLen, nullptr, 0);
	if (reqSize > 0)
	{
		std::vector<wchar_t> buff(reqSize + 1); // 最後の + 1 は必須らしい。終端 null を含まないサイズが返るらしい。
		::MultiByteToWideChar(CP_UTF8, 0, srcText, textLen, &buff[0], reqSize);
		return buff;
	}
	else
	{
		return std::vector<wchar_t>();
	}
}

bool MyLua::DoLuaScriptString(const wchar_t* script)
{
	_ASSERTE(m_lua != nullptr);
	// Lua に UTF-16LE 文字列を渡すときは UTF-8 へ変換しておく。
	const auto utf8text = ConvertUtf16toUtf8(script);
	if (!utf8text.empty())
	{
		return this->DoLuaScriptString(&utf8text[0]);
	}
	else
	{
		return false;
	}
}

bool MyLua::DoLuaScriptString(const char* script)
{
	_ASSERTE(m_lua != nullptr);
	_ASSERTE(script != nullptr);
	lua_State* lua = m_lua;
	if (luaL_loadbuffer(lua, script, std::strlen(script), script) || lua_pcall(lua, 0, 0, 0))
	{
		std::string temp = lua_tostring(lua, -1);
		lua_pop(lua, 1);
		temp += "\r\n";
		const auto utf16text = ConvertUtf8toUtf16(temp.c_str());
		_ASSERTE(!utf16text.empty());
		OutputDebugStringW(&utf16text[0]);
		auto writerFunc = MyLua::m_errorWriter;
		if (writerFunc != nullptr)
		{
			writerFunc(ref new Platform::String(&utf16text[0]));
		}
		return true;
	}
	return false;
}

// Lua に公開する関数。
void MyLua::Print(const char* srcText)
{
	_ASSERTE(srcText != nullptr);
	std::string temp(srcText);
	temp += "\r\n";

	// ASCII のみのときは、Lua から受け取った UTF-8 文字列であっても OutputDebugStringA() で直接出力できる。非 ASCII が含まれる場合は不可。
	// なお、OutputDebugStringA() を直接使っていると、Windows ストア アプリの認証キットによる判定が不合格となるので、
	// デバッグ ビルドでのみ有効になるようにマクロでラップするなどの対処が必要。
	// ちなみに、OutputDebugStringW() は残っていても不合格にはならない模様。

	// Lua から文字列を受け取るときは、UTF-8 から UTF-16LE へ変換する。
	// デスクトップ アプリであれば、Lua 側の実装を修正することで
	// 内部表現に UTF-8 でなく Shift_JIS などを使うことも可能だが、
	// ストア アプリではやめておいたほうがいい。
	// ちなみに Windows ストア アプリでも一部の ANSI 用 API は使用可能だが、
	// これらはデスクトップ同様 UTF-8 のマルチバイト文字には対応していない。

	const auto utf16text = ConvertUtf8toUtf16(temp.c_str());
	_ASSERTE(!utf16text.empty());
	OutputDebugStringW(&utf16text[0]);
	auto writerFunc = MyLua::m_outputWriter;
	if (writerFunc != nullptr)
	{
		writerFunc(ref new Platform::String(&utf16text[0]));
	}
	// デスクトップ アプリではデフォルトで OutputDebugStringW() によって非 ASCII 文字を出力することができず、
	// ロケールの明示的な設定が必要だが、ストア アプリはデフォルトで出力可能になっている。
	// ただし .NET の System.Diagnostics.Debug.WriteLine() とは違って、
	// U + 2665H などの Unicode 文字を出力することは依然として不可能。
}

void MyLua::Bind()
{
	_ASSERTE(m_lua != nullptr);
	luabind::module(m_lua) [
		luabind::def("Print", Print)
	];
}
