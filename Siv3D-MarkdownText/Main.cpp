# include <Siv3D.hpp> // Siv3D v0.6.15
# include "MarkdownText.h"

void Main()
{
	int currentTab = 0;
	Array<String> tabs = { U"Test", U"説明書", U"フレーバーテキスト"};
	Array<String> texts = {
U"\
# Hello, *Siv3D*!\n\
*ilatic* and **bold** words\n\
this is not a # heading\n\
\n\
## Listing\n\
* list1\n\
  1. list2-1\n\
    * list3\n\
  * list2-2\n\
",
U"\
# ブロック崩し\n\
## 遊び方\n\
1. 画面をクリックすると、**ボール**が発射されます。\n\
2. **ボール**が**ブロック**に当たると、**ブロック**が消えます。\n\
3. **マウス**を追って**パドル**が動きます。\n\
3. **パドル**に当たった**ボール**は跳ね返ります。\n\
4. **ブロック**を全て消すとクリア！\n\
\n\
## スコア\n\
* ブロックを消すと得点が入ります。\n\
* 連続でブロックを消すとボーナスが入ります。\n\
* クリアすると、残り時間がボーナスに加算されます。\n\
* 友達とスコアを競い合おう！\n\
",
U"\
## 骨のナイフ\n\
*中型の動物の骨から作られたと思しき***古びた***ナイフ。*\n\
*刃は***鋭く***、柄は滑り止めのために皮で巻かれている。*\n\
*かつての持ち主の手入れが不十分だったのだろう、***汚れ***がこびりついている。*\n\
- 耐久性 -20%\n\
- 攻撃力 +10%\n\
- 感染確率 +5%\n\
",
	};

	Array<MarkdownTextStyle> styles = { {25}, {25}, {20} };

	TextAreaEditState input;
	input.text = texts[currentTab];
	input.rebuildGlyphs();

	MarkdownText md{ input.text, styles[currentTab]};

	while (System::Update())
	{

		Vec2 tabPos{ 50, 10 };
		for (auto [i,label] : Indexed(tabs)) {
			auto text = SimpleGUI::GetFont()(label);
			auto region = text.region(tabPos).stretched(10,5);
			if (currentTab == i) region.draw(Palette::Dimgrey);
			if (region.leftClicked()) {
				currentTab = i;
				input.text = texts[currentTab];
				input.rebuildGlyphs();
				md.markdown = texts[currentTab];
				md.style = styles[currentTab];
				md.build();
			}
			tabPos.x += text.draw(tabPos).w + 20;
		}

		switch (currentTab)
		{
		case 0:
			if (SimpleGUI::TextArea(input, Vec2{ 50, 70 }, Size{ 300,500 })) {
				ClearPrint();
				texts[currentTab] = input.text;
				md.markdown = texts[currentTab];
				md.build();
			}
			md.draw(Vec2{ 400,50 });
			break;
		case 1:
			md.draw(Vec2{ 50,70 });
			break;
		case 2:
			md.draw(Vec2{ 50,70 });
			break;
		}


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
