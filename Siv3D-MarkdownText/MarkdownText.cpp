# include "MarkdownText.h"

RectF MarkdownText::draw(const Vec2& leftTop) const
{
	struct {
		bool italic = false;
	} state;

	Vec2 penPos = leftTop;
	RectF bound = { penPos, Vec2(0, 0) };

	const size_t mdLen = md.size();
	for (size_t i = 0; i < mdLen; i++) {

		// 状態変更
		if (md[i] == U'*') {
			state.italic ^= true;
			continue;
		}

		// 描画
		const Font& font = state.italic ? style.italicFont : style.font;

		const auto glyph = font.getGlyph(md[i]);
		const auto drawedRect = glyph.texture.draw(penPos + glyph.getOffset());
		penPos.x += glyph.xAdvance;

		bound.w = Max(bound.x + bound.w, drawedRect.x + drawedRect.w) - bound.x;
		bound.h = Max(bound.y + bound.h, drawedRect.y + drawedRect.h) - bound.y;
	}

	return bound;
}
