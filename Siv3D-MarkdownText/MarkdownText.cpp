# include "MarkdownText.h"

RectF MarkdownText::draw(const Vec2& leftTop) const
{
	StringView md = markdown;

	struct {
		bool isBlock = true;

		bool heading = false;
		int headingLevel = 0;

		bool strong = false;
		bool italic = false;

		void lineBreak() {
			isBlock = true;

			heading = false;
			headingLevel = 0;

			strong = false;
			italic = false;
		}
	} state;

	Vec2 penPos = leftTop;
	RectF bound = { penPos, Vec2(0, 0) };

	while (md.length() > 0) {
		// 状態変更

		// ブロック要素
		if (state.isBlock) {
			if (const auto& m = RegExp(U"^(#{1,6}) +").match(md); not m.isEmpty()) {
				state.heading = true;
				state.headingLevel = m[1]->length();
				Print << U"heading: " << state.headingLevel;
				md = md.substr(m[0]->length());
				continue;
			}
		}

		// インライン要素
		{
			if (const auto& m = RegExp(U"\\*{2}").match(md); not m.isEmpty()) {
				state.strong ^= true;
				md = md.substr(m[0]->length());
				continue;
			}
			if (const auto& m = RegExp(U"\\*{1}").match(md); not m.isEmpty()) {
				state.italic ^= true;
				md = md.substr(m[0]->length());
				continue;
			}
		}

		state.isBlock = false;

		// 描画
		const Font& font = state.strong ?
			(state.italic ? style.strongItalicFont : style.strongFont) :
			(state.italic ? style.italicFont : style.font);
		const double scale = style.headingScales[state.headingLevel];
		
		const auto glyph = font.getGlyph(md[0]);
		const auto drawedRect = glyph.texture.resized(Vec2{ glyph.width, glyph.height } * scale).draw(penPos + glyph.getOffset(scale));
		penPos.x += glyph.xAdvance * scale;

		drawedRect.drawFrame(1);
		bound.x = Min(bound.x, drawedRect.x);
		bound.y = Min(bound.y, drawedRect.y);
		bound.w = Max(bound.x + bound.w, drawedRect.x + drawedRect.w) - bound.x;
		bound.h = Max(bound.y + bound.h, drawedRect.y + drawedRect.h) - bound.y;

		md = md.substr(1);
	}

	return bound;
}
