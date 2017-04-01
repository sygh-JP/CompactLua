//
// MainPage.xaml.h
// MainPage クラスの宣言。
//

#pragma once

#include "MainPage.g.h"
#include "LuaTest.h"

namespace StoreAppLuaTest1
{
	/// <summary>
	/// それ自体で使用できる空白ページまたはフレーム内に移動できる空白ページ。
	/// </summary>
	public ref class MainPage sealed
	{
	private:
		TMyLuaPtr m_myLua;

	public:
		MainPage();

	protected:
		virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
	private:
		void buttonExecute_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void buttonClearOutput_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void buttonClearError_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
