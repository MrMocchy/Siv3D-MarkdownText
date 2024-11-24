# include "MarkdownText.h"

MarkdownText::MarkdownText(String markdown, MarkdownTextStyle style)
{
	StringView md = markdown;
	double regularFontScale = style.headingScales[0];

	struct {
		bool headOfLine = true;

		size_t headingLevel = 0;
		int listLevel = 0;

		bool strong = false;
		bool italic = false;

		void lineBreak() {
			headOfLine = true;

			headingLevel = 0;
			listLevel = 0;

			strong = false;
			italic = false;
		}
	} state;
	
	Vec2 penPos{ 0,0 };

	while (md.length() > 0) {
		// 状態変更
		if (state.headOfLine)
		{
			if (const auto& m = RegExp(U"^(#{1,6}) +").match(md); not m.isEmpty()) {
				state.headingLevel = m[1]->length();
				md.remove_prefix(m[0]->length());
				continue;
			}
			if (const auto& m = RegExp(U"^( *)[\\-\\+\\*] +").match(md); not m.isEmpty()) {
				size_t indentSize = m[1]->length();
				size_t listLevel = indentSize / style.mdListIndentSpaceSize + 1;
				penPos.x = style.fontSize * regularFontScale * style.listIndentSize * (listLevel - 0.66);
				const auto bulletChar = listLevel < style.listBullets.length() ? style.listBullets[listLevel - 1] : style.listBullets.back();
				addGlyph(style.font, bulletChar, penPos, regularFontScale);
				penPos.x = style.fontSize * regularFontScale * style.listIndentSize * listLevel;
				md.remove_prefix(m[0]->length());
				continue;
			}
		}

		state.headOfLine = false;
		{
			if (const auto& m = RegExp(U"^\\n").match(md); not m.isEmpty()) {
				penPos.x = 0;
				penPos.y += style.font.height() * style.headingScales[state.headingLevel];
				state.lineBreak();
				md.remove_prefix(m[0]->length());
				continue;
			}
			if (const auto& m = RegExp(U"^\\*{2}").match(md); not m.isEmpty()) {
				state.strong ^= true;
				md.remove_prefix(m[0]->length());
				continue;
			}
			if (const auto& m = RegExp(U"^\\*{1}").match(md); not m.isEmpty()) {
				state.italic ^= true;
				md.remove_prefix(m[0]->length());
				continue;
			}
		}

		// グリフ情報の追加
		{
			const Font& font = state.strong ?
				(state.italic ? style.strongItalicFont : style.strongFont) :
				(state.italic ? style.italicFont : style.font);
			const double scale = style.headingScales[state.headingLevel];

			penPos = addGlyph(font, md[0], penPos, scale);

			md.remove_prefix(1);
		}
	}
}

Vec2 MarkdownText::addGlyph(const Font& font, const char32& ch, const Vec2& penPos, double scale)
{
	const auto glyph = font.getGlyph(ch);
	const auto pos = penPos + glyph.getOffset(scale);
	glyphInfos.push_back({ glyph, pos, scale });

	return penPos + Vec2{ glyph.xAdvance * scale, 0 };
}

RectF MarkdownText::draw(const Vec2& topLeft) const
{
	if (glyphInfos.isEmpty()) {
		return RectF{ topLeft, 0, 0 };
	}
	RectF bound{ glyphInfos[0].pos + topLeft, 0, 0 };
	for (const auto& g : glyphInfos) {
		auto rect = g.glyph.texture.resized(Vec2(g.glyph.texture.size) * g.scale).draw(g.pos + topLeft);
		// rect.drawFrame(1);
		bound.x = Min(bound.x, rect.x);
		bound.y = Min(bound.y, rect.y);
		bound.w = Max(bound.x + bound.w, rect.x + rect.w) - bound.x;
		bound.h = Max(bound.y + bound.h, rect.y + rect.h) - bound.y;
	}
	return bound;
}
