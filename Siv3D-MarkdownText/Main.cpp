﻿# include <Siv3D.hpp> // Siv3D v0.6.15
# include "MarkdownText.h"

void Test(String md, MarkdownTextStyle style, Vec2* pos)
{
	SimpleGUI::GetFont()(md).draw(*pos);
	pos->y += 30;

	MarkdownText(md, style).draw(*pos);
	pos->y += 50;
}

void Main()
{
	MarkdownTextStyle style;
	style.font = Font(30);
	style.italicFont = Font(30, Typeface::Regular, FontStyle::Italic);

	while (System::Update())
	{
		ClearPrint();

		Vec2 pos = { 50, 50 };

		Test(U"Hello, *Siv3D*!", style, &pos);

	}
}

//
// - Debug ビルド: プログラムの最適化を減らす代わりに、エラーやクラッシュ時に詳細な情報を得られます。
//
// - Release ビルド: 最大限の最適化でビルドします。
//
// - [デバッグ] メニュー → [デバッグの開始] でプログラムを実行すると、[出力] ウィンドウに詳細なログが表示され、エラーの原因を探せます。
//
// - Visual Studio を更新した直後は、プログラムのリビルド（[ビルド]メニュー → [ソリューションのリビルド]）が必要な場合があります。
//
