# include "MarkdownText.h"

MarkdownText::MarkdownText(String markdown, MarkdownTextStyle style)
	: markdown(markdown)
	, style(style) {}

void MarkdownText::build()
{
	StringView md = markdown;
	double regularFontScale = style.headingScales[0];

	enum class ListType {
		Unordered,
		Ordered,
	};
	struct ListInfo {
		ListType type;
		size_t mdIndentSize;
		size_t listLevel;
		int order;
	};
	
	struct {
		bool headOfLine = true;

		size_t headingLevel = 0;
		Array<ListInfo> listNest;

		double indent = 0;

		bool strong = false;
		bool italic = false;

		void lineBreak() {
			headOfLine = true;

			headingLevel = 0;

			strong = false;
			italic = false;
		}
	} state;
	
	Vec2 penPos{ 0,0 };

	m_glyphInfos.clear();
	while (md.length() > 0) {
		// 状態変更
		if (state.headOfLine)
		{
			state.headOfLine = false;
			if (const auto& m = RegExp(U"^\n").match(md); not m.isEmpty()) {
				state.lineBreak();
				state.listNest.clear();
				state.indent = 0;
				penPos.x = 0;
				penPos.y += style.font.height() * regularFontScale * 0.2;
				md.remove_prefix(m[0]->length());
				continue;
			}
			if (const auto& m = RegExp(U"^(#{1,6}) +").match(md); not m.isEmpty()) {
				state.headingLevel = m[1]->length();
				md.remove_prefix(m[0]->length());
				continue;
			}
			if (const auto& m = RegExp(U"^( *)(\\-|\\+|\\*|\\d\\.) +").match(md); not m.isEmpty()) {
				ListType type = m[2]->ends_with(U'.') ? ListType::Ordered : ListType::Unordered;
				size_t mdIndentSize = m[1]->length();
				size_t listLevel = 1;
				int order = 1;
				if (state.listNest.size() == 0) {
					state.listNest.push_back({ type, mdIndentSize, listLevel, order });
				}
				else {
					for (auto itr = state.listNest.rbegin(); itr != state.listNest.rend(); ++itr) {
						if (mdIndentSize == itr->mdIndentSize) {
							type = itr->type;
							listLevel = itr->listLevel;
							order = itr->order + 1;
							itr->order = order;
							state.listNest.pop_back_N(state.listNest.rend() - itr - 1);
							break;
						}
						if (mdIndentSize > itr->mdIndentSize) {
							listLevel = itr->listLevel + 1;
							state.listNest.push_back({ type, mdIndentSize, listLevel, order });
							break;
						}
					}
				}
				/*
				String listLevels;
				for (const auto& l : state.listWrap) {
					listLevels += U"{} "_fmt(l.listLevel);
				}
				Print << U"ListLevels: " << listLevels;
				*/
				state.indent = style.fontSize * regularFontScale * style.listIndentSize * listLevel;
				penPos.x = state.indent - style.fontSize * regularFontScale * style.listIndentSize * (type == ListType::Ordered ? 1 : 0.66);
				const String bulletChar =
					type == ListType::Ordered ? U"{}."_fmt(order) :
					listLevel < style.listBullets.length() ? style.listBullets.substr(listLevel - 1, 1)
					: style.listBullets.substr(style.listBullets.length() - 1);
				addGlyphs(style.font, bulletChar, penPos, regularFontScale, state.indent);
				penPos.x = style.fontSize * regularFontScale * style.listIndentSize * listLevel;
				md.remove_prefix(m[0]->length());
				continue;
			}
			state.headOfLine = false;
		}

		{
			if (const auto& m = RegExp(U"^\\n").match(md); not m.isEmpty()) {
				penPos.x = state.indent;
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

			penPos = addGlyph(font, md[0], penPos, scale, state.indent);

			md.remove_prefix(1);
		}
	}
}

Vec2 MarkdownText::addGlyph(const Font& font, const char32& ch, const Vec2& _penPos, double scale, double indent)
{
	const auto glyph = font.getGlyph(ch);
	auto penPos = _penPos;
	auto glyphPos = penPos + glyph.getOffset(scale);
	const auto glyphRightX = glyphPos.x + glyph.texture.size.x * scale;
	if (m_width < glyphRightX) {
		penPos.x = indent;
		penPos.y += font.height() * scale;
		glyphPos = penPos + glyph.getOffset(scale);
	}
	m_glyphInfos.push_back({ glyph, glyphPos, scale });

	return penPos + Vec2{ glyph.xAdvance * scale, 0 };
}

Vec2 MarkdownText::addGlyphs(const Font& font, const String& str, const Vec2& penPos, double scale, double indent)
{
	Vec2 pos = penPos;
	for (const auto& ch : str) {
		pos = addGlyph(font, ch, pos, scale, indent);
	}
	return pos;
}

RectF MarkdownText::draw(const Vec2& topLeftPos, const double width)
{
	if (width < 0) {
		m_width = Math::Inf;
	}
	if (m_width != width) {
		m_width = width;
		build();
	}

	if (m_glyphInfos.isEmpty()) {
		return RectF{ topLeftPos, 0, 0 };
	}
	RectF bound{ m_glyphInfos[0].pos + topLeftPos, 0, 0 };
	for (const auto& g : m_glyphInfos) {
		auto rect = g.glyph.texture.resized(Vec2(g.glyph.texture.size) * g.scale).draw(g.pos + topLeftPos);
		// rect.drawFrame(1);
		bound.x = Min(bound.x, rect.x);
		bound.y = Min(bound.y, rect.y);
		bound.w = Max(bound.x + bound.w, rect.x + rect.w) - bound.x;
		bound.h = Max(bound.y + bound.h, rect.y + rect.h) - bound.y;
	}
	return bound;
}
