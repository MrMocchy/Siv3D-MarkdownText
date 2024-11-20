#pragma once

# include "MarkdownTextStyle.h"

class MarkdownText
{
public:
	MarkdownText(String markdown, MarkdownTextStyle style)
		: md{ markdown }
		, style{ style }
	{};

	RectF draw(const Vec2& topLeft) const;

private:

	String md;
	MarkdownTextStyle style;

};

