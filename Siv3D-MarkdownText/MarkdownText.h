#pragma once

# include "MarkdownTextStyle.h"

class MarkdownText
{
public:
	String markdown;
	MarkdownTextStyle style;

	MarkdownText(String markdown, MarkdownTextStyle style);

	RectF draw(const Vec2& topLeftPos, const double width = Math::Inf);

	RectF region(const Vec2& topLeftPos, const double width = Math::Inf);

	void build();

private:

	double m_width = -1;

	struct GlyphInfo
	{
		Glyph glyph;
		Vec2 pos;
		double scale;
		Color color;
		std::function<void()> callback;
		RectF callbackHitbox;
	};
	Array<GlyphInfo> m_glyphInfos;

	Vec2 addGlyph(const Font& font, const Color& color, const char32& ch, const Vec2& penPos, double scale, double indent, const std::function<void()>& callback = nullptr);
	Vec2 addGlyphs(const Font& font, const Color& color, const StringView& str, const Vec2& penPos, double scale, double indent, const std::function<void()>& callback = nullptr);

};

