#include "WindowTitle.h"

WindowTitle::WindowTitle() : WindowTitle("") { }

WindowTitle::WindowTitle(string title)
{
	text = title;
}

void WindowTitle::SetText(string text)
{
	modified = true;
	this->text = text;
}

unique_ptr<WindowTitle>& WindowTitle::SetSubText(string text)
{
	unique_ptr<WindowTitle>& subTitle = GetSubTitle();
	subTitle->SetText(text);
	return subTitle;
}

unique_ptr<WindowTitle>& WindowTitle::GetSubTitle()
{
	if (!subTitle)
		subTitle = make_unique<WindowTitle>();

	return subTitle;
}

string WindowTitle::ToString() const
{
	return text + (subTitle ? " - " + subTitle->ToString() : "");
}

bool WindowTitle::IsOutdated() const
{
	return modified || (subTitle && subTitle->IsOutdated());
}

void WindowTitle::Update()
{
	if (subTitle)
		subTitle->Update();

	modified = false;
}
