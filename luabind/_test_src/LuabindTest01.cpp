
#include <luabind/luabind.hpp>
#include <luabind/operator.hpp>

#include <cstdio>


// Luabind を使って演算子オーバーロードをバインドするとき、
// boost::detail::is_polymorphic_imp1 が内部で使用されるらしく、
// VC++ 拡張機能の sealed を使って継承を禁止しているとコンパイル エラーになる。
// おそらく C++11 の final も同様。
// なお、バインド可能な演算子オーバーロードは、Lua 5.1 でサポートされているものに限る。
// 詳しくは Lua および Luabind のマニュアルを参照のこと。


//class Vector2D sealed
class Vector2D
{
public:
	float X, Y;
public:
	explicit Vector2D(float x = 0, float y = 0) : X(x), Y(y) {}

	void Print() const { printf("(%f, %f)\n", this->X, this->Y); }

	Vector2D& operator*=(float f)
	{
		this->X *= f;
		this->Y *= f;
		return *this;
	}

	Vector2D& operator/=(float f)
	{
		this->X /= f;
		this->Y /= f;
		return *this;
	}

	Vector2D operator+(const Vector2D& v) const
	{ return Vector2D(this->X + v.X, this->Y + v.Y); }

	Vector2D operator-(const Vector2D& v) const
	{ return Vector2D(this->X - v.X, this->Y - v.Y); }

	static int GetSize() { return sizeof(Vector2D); }
	// size_t 型は使えない。
	// Lua は 64bit 整数範囲をサポートしないので、Win64 では
	// std::runtime_error: 'Trying to use unregistered class' が発生してしまう。
	// LP64 環境で Lua/Luabind を使う場合も注意が必要となる。

	class Helper
	{
	public:
		static Vector2D Add(const Vector2D& lh, const Vector2D& rh)
		{ return Vector2D(lh.X + rh.X, lh.Y + rh.Y); }
		static Vector2D Sub(const Vector2D& lh, const Vector2D& rh)
		{ return Vector2D(lh.X - rh.X, lh.Y - rh.Y); }
	};
};

void Greet()
{
	puts("bonjour");
}

// Luabind のサンプルから流用。
bool DoScriptString(lua_State* lua, const char* script)
{
	assert(script != nullptr);
	if (luaL_loadbuffer(lua, script, std::strlen(script), script) || lua_pcall(lua, 0, 0, 0))
	{
		const char* a = lua_tostring(lua, -1);
		puts(a);
		lua_pop(lua, 1);
		return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////
void LuabindTest01(lua_State* lua)
{
	luabind::module(lua) [
		luabind::def("Greet", Greet),
			luabind::class_<Vector2D>("Vector2D")
			.def(luabind::constructor<float, float>())
			.def(luabind::constructor<const Vector2D&>()) // コンパイラが自動生成したコピーコンストラクタをバインドすることもできる。
			.def("Print", &Vector2D::Print)
			// Lua には +=, -=, *=, /=, ++, -- 演算子がないので、直接のバインドは不可能。
			// また、代入演算子もバインド不可能。
			.def("Mul", &Vector2D::operator*=)
			.def("Div", &Vector2D::operator/=)
			.def(luabind::self + luabind::other<const Vector2D&>())
			.def(luabind::self - luabind::other<const Vector2D&>())
			.scope[
				// 静的メソッドのほか、ネストされたクラスもバインドできる。
				luabind::def("GetSize", &Vector2D::GetSize),
					luabind::class_<Vector2D::Helper>("Helper")
					.scope[
						luabind::def("Add", &Vector2D::Helper::Add),
						luabind::def("Sub", &Vector2D::Helper::Sub)
					]
			]
	];

	DoScriptString(lua,
		"Greet()\n"
		//"print('こんにちは、Luaソース')\n" // S-JIS 対応 Lua のテスト。
		"v1 = Vector2D(10, 20)\n"
		"v1:Print()\n"
		"v2 = Vector2D(v1)\n" // クラス オブジェクトをコピーするとき、通常の代入演算子では、参照の代入（シャローコピー）になってしまうので注意。
		"v2:Mul(0.5)\n"
		"v2:Print()\n"
		"v3 = Vector2D.Helper.Add(v1, v2)\n"
		"v3:Print()\n"
		"v4 = Vector2D(v1)\n"
		"v4:Div(2)\n"
		"v4:Print()\n"
		"v5 = v1 + v2\n"
		"v5:Print()\n"
		"print('Size of Vector2D = ' .. Vector2D.GetSize())\n"
		);
}
