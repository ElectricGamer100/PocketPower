#pragma once

#include <vector>

namespace Touch {
	class StartMenuScreen {
	public:
		static int currentSplash;
		static std::vector<std::string> mSplashes;
		
		StartMenuScreen(void);
		virtual ~StartMenuScreen();
		void chooseRandomSplash();
	};
}
