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

	Vec2 addGlyph(const Font& font, const char32& ch, const Vec2& penPos, double scale);
	Vec2 addGlyphs(const Font& font, const String& str, const Vec2& penPos, double scale);

};

