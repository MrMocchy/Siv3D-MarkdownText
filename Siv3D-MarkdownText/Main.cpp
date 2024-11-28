# include <Siv3D.hpp> // Siv3D v0.6.15
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
	MarkdownTextStyle style{30};

	TextAreaEditState input;
	input.text =
U"\
# Hello, *Siv3D*!\n\
*ilatic* and **bold** words\n\
this is not a #heading\n\
* list1\n\
  1. list2-1\n\
    * list3\n\
  * list2-2\n\
";
	input.rebuildGlyphs();

	MarkdownText md{ input.text, style };

	while (System::Update())
	{

		Vec2 pos = { 50, 50 };

		//Test(U"# Hello, *Siv3D*!", style, &pos);

		if (SimpleGUI::TextArea(input, Vec2{ 50, 50 }, Size{ 300,500 })) {
			ClearPrint();
			md.markdown = input.text;
			md.build();
		}
		md.draw(Vec2{400,50}).drawFrame(1, Palette::Yellow);

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
