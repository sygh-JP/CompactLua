#pragma once

delegate void WriteStringDelegate(Platform::String^ str);

// Luabind のテスト用クラス。
// Lua ステートの RAII を兼ねる。
class MyLua
{
	lua_State* m_lua;
private:
	static WriteStringDelegate^ m_outputWriter;
	static WriteStringDelegate^ m_errorWriter;
	// property もしくは event にしたければ、ref クラス内に含める必要がある。
public:
	MyLua();
	~MyLua();
	void Bind();
	bool DoLuaScriptString(const char* script);
	bool DoLuaScriptString(const wchar_t* script);
public:
	static void SetOutputWriter(WriteStringDelegate^ writer)
	{ m_outputWriter = writer; }
	static void SetErrorWriter(WriteStringDelegate^ writer)
	{ m_errorWriter = writer; }
private:
	static void Print(const char* srcText);
};

typedef std::shared_ptr<MyLua> TMyLuaPtr;
