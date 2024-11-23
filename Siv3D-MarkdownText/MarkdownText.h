#pragma once

# include "MarkdownTextStyle.h"

class MarkdownText
{
public:
	MarkdownText(String markdown, MarkdownTextStyle style);

	RectF draw(const Vec2& topLeft) const;

private:

	struct GlyphInfo
	{
		Glyph glyph;
		Vec2 pos;
		double scale;
	};
	Array<GlyphInfo> glyphInfos;

};

