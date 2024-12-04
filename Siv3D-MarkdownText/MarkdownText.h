﻿#pragma once

# include "MarkdownTextStyle.h"

class MarkdownText
{
public:
	String markdown;
	MarkdownTextStyle style;

	MarkdownText(String markdown, MarkdownTextStyle style);

	RectF draw(const Vec2& topLeftPos, const double width = Math::Inf);

	void build();

private:

	double m_width;

	struct GlyphInfo
	{
		Glyph glyph;
		Vec2 pos;
		double scale;
	};
	Array<GlyphInfo> m_glyphInfos;

	Vec2 addGlyph(const Font& font, const char32& ch, const Vec2& penPos, double scale, double indent);
	Vec2 addGlyphs(const Font& font, const String& str, const Vec2& penPos, double scale, double indent);

};

