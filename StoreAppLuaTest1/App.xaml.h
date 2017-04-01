//
// App.xaml.h
// App クラスの宣言。
//

#pragma once

#include "App.g.h"

namespace StoreAppLuaTest1
{
	/// <summary>
	/// 既定の Application クラスを補完するアプリケーション固有の動作を提供します。
	/// </summary>
	ref class App sealed
	{
	public:
		App();
		virtual void OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ args) override;

	private:
		void OnSuspending(Platform::Object^ sender, Windows::ApplicationModel::SuspendingEventArgs^ e);
	};
}
