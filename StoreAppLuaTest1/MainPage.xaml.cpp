//
// MainPage.xaml.cpp
// MainPage クラスの実装。
//

#include "pch.h"
#include "MainPage.xaml.h"

using namespace StoreAppLuaTest1;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

// 空白ページのアイテム テンプレートについては、http://go.microsoft.com/fwlink/?LinkId=234238 を参照してください

MainPage::MainPage()
{
	InitializeComponent();

	m_myLua = TMyLuaPtr(new MyLua());

	m_myLua->Bind();

	m_myLua->SetOutputWriter(ref new WriteStringDelegate([this](Platform::String^ str)
	{
		if (this->textboxOutput != nullptr)
		{
			this->textboxOutput->Text += str;
		}
	}));

	m_myLua->SetErrorWriter(ref new WriteStringDelegate([this](Platform::String^ str)
	{
		if (this->textboxError != nullptr)
		{
			this->textboxError->Text += str;
		}
	}));

	// Lua へ渡すときに UTF-8 に変換しているので、日本語も自由に使用可能。
	// ビット演算ライブラリが Lua 5.2 で追加されたが、Lua 5.3 ではビット演算子が追加されたため、非標準になった模様。
	// Lua 5.3 では LUA_COMPAT_5_2 を定義してコンパイルしないとビット演算ライブラリが使えない。
	// Lua 5.4 ではビット演算ライブラリが完全削除されたため、LUA_COMPAT_5_2 を定義してコンパイルしても使えない。
#if 1
	this->textboxInput->Text =
		L"Print('hoge日本語ソース');\r\n"
		L"x = 10.1;\r\n"
		L"Print(x .. '');\r\n"
		L"--y = bit32.bor(1, 2);\r\n"
		L"--Print(y .. '');\r\n"
		L"z = 1 | 2;\r\n"
		L"Print(z .. '');\r\n"
		L"Print(2.0 .. '');\r\n"
		L"Print(math.tointeger(\"2.0\") .. '');\r\n"
		;
#endif
}

/// <summary>
/// このページがフレームに表示されるときに呼び出されます。
/// </summary>
/// <param name="e">このページにどのように到達したかを説明するイベント データ。Parameter 
/// プロパティは、通常、ページを構成するために使用します。</param>
void MainPage::OnNavigatedTo(NavigationEventArgs^ e)
{
	(void) e;	// 未使用のパラメーター
}


void StoreAppLuaTest1::MainPage::buttonExecute_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	m_myLua->DoLuaScriptString(this->textboxInput->Text->Data());
}


void StoreAppLuaTest1::MainPage::buttonClearOutput_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	this->textboxOutput->Text = L"";
}


void StoreAppLuaTest1::MainPage::buttonClearError_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	this->textboxError->Text = L"";
}
