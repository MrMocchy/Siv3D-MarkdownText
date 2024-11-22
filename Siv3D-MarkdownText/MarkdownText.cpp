# include "MarkdownText.h"

RectF MarkdownText::draw(const Vec2& leftTop) const
{
	StringView md = markdown;

	struct {
		int headingLevel = 0;

		bool strong = false;
		bool italic = false;

		void lineBreak() {
			headingLevel = 0;

			strong = false;
			italic = false;
		}
	} state;

	Vec2 penPos = leftTop;
	RectF bound = { penPos, Vec2(0, 0) };

	while (md.length() > 0) {
		// 状態変更
		{
			if (const auto& m = RegExp(U"^(#{1,6}) +").match(md); not m.isEmpty()) {
				state.headingLevel = m[1]->length();
				md.remove_prefix(m[0]->length());
				continue;
			}

			if (const auto& m = RegExp(U"\\n").match(md); not m.isEmpty()) {
				penPos.x = leftTop.x;
				penPos.y += style.font.height() * style.headingScales[state.headingLevel];
				state.lineBreak();
				md.remove_prefix(m[0]->length());
				continue;
			}
			if (const auto& m = RegExp(U"\\*{2}").match(md); not m.isEmpty()) {
				state.strong ^= true;
				md.remove_prefix(m[0]->length());
				continue;
			}
			if (const auto& m = RegExp(U"\\*{1}").match(md); not m.isEmpty()) {
				state.italic ^= true;
				md.remove_prefix(m[0]->length());
				continue;
			}
		}

		// 描画
		{
			const Font& font = state.strong ?
				(state.italic ? style.strongItalicFont : style.strongFont) :
				(state.italic ? style.italicFont : style.font);
			const double scale = style.headingScales[state.headingLevel];

			const auto glyph = font.getGlyph(md[0]);
			const auto drawedRect = glyph.texture.resized(glyph.texture.size * scale).draw(penPos + glyph.getOffset(scale));
			penPos.x += glyph.xAdvance * scale;

			//drawedRect.drawFrame(1);
			bound.x = Min(bound.x, drawedRect.x);
			bound.y = Min(bound.y, drawedRect.y);
			bound.w = Max(bound.x + bound.w, drawedRect.x + drawedRect.w) - bound.x;
			bound.h = Max(bound.y + bound.h, drawedRect.y + drawedRect.h) - bound.y;

			md.remove_prefix(1);
		}
	}

	return bound;
}
