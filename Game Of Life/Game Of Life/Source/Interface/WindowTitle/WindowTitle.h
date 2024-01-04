#pragma once

#include <string>
#include <memory>

using std::string, std::unique_ptr;

class WindowTitle
{
	public:
		WindowTitle();
		WindowTitle(string title);
		void SetText(string text);
		unique_ptr<WindowTitle>& SetSubText(string text);
		unique_ptr<WindowTitle>& GetSubTitle();
		string ToString() const;
		bool IsOutdated() const;
		void Update();

	private:
		string text;
		bool modified = false;
		unique_ptr<WindowTitle> subTitle;
};
