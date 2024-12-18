# include <Siv3D.hpp> // Siv3D v0.6.15
# include "MarkdownText.h"

class ClickEffect : public IEffect
{
	Vec2 m_pos;
public:
	ClickEffect(const Vec2& pos) : m_pos(pos) {}
	bool update(double t) override
	{
		const double time = 0.5;
		const double r = Sqrt(1500 * (t / time));
		const double alpha = Pow(1 - t / time, 2);
		Circle{ m_pos, r }.draw(AlphaF(alpha));
		return t < time;
	}
};

void Main()
{
	ScreenCapture::SetShortcutKeys({ KeyControl + KeyS });
	Effect effect;

	int currentTab = 0;
	Array<String> tabs = { U"Test", U"説明書", U"フレーバーテキスト", U"リンク"};
	Array<String> texts = {
U"\
# Hello, ***Siv3D***!\n\
*ilatic* and **bold** words\n\
this is not a # heading\n\
\n\
## Listing\n\
* list1\n\
  1. list2-1\n\
    * list3\n\
  * list2-2\n\
\n\
## Link\n\
[Link](to-somewhere)\n\
",
U"\
# ブロック崩し\n\
## 遊び方\n\
1. 画面をクリックすると、**ボール**が発射されます。\n\
1. **ボール**が**ブロック**に当たると、**ブロック**が消えます。\n\
1. **マウス**を追って**パドル**が動きます。\n\
1. **パドル**に当たった**ボール**は跳ね返ります。\n\
1. **ブロック**を全て消すとクリア！\n\
\n\
## スコア\n\
* ブロックを消すと得点が入ります。\n\
* 連続でブロックを消すとボーナスが入ります。\n\
* クリアすると、残り時間がボーナスに加算されます。\n\
* 友達とスコアを競い合おう！\n\
",
U"\
## 料理包丁\n\
*一般家庭にある一般的な**料理**包丁。*\n\
*何年も研がずに使われることを想定し、**頑丈**な作りになっている。*\n\
*当然ながら戦闘に用いるには**取り回しが悪い**が、急場は凌げるだろう。*\n\
- 攻撃力 **+20**\n\
- 攻撃速度 **-30%**\n\
- 射程 **20**\n\
- 耐久値 **134/150**\n\
- 《料理》の成功率 **x2**\n\
",
U"\
# リンクテスト\n\
## 実行\n\
- [Print << U\"Hello, Siv3D!\";](Print)\n\
- [ClearPrint()](ClearPrint)\n\
\n\
## Siv3D ドキュメント\n\
- [公式サイト](Siv3D_HP)\n\
- [GitHub](Siv3D_GitHub)\n\
\n\
## その他\n\
- [トースト通知](Toast)\n\
",
	};

	Array<MarkdownTextStyle> styles = { {25}, {25}, {20, Palette::Black}, {25} };
	styles[2].italicColor = Palette::Dimgray;
	styles[2].strongItalicColor = Palette::Dimgray;
	styles[3].callbackTable[U"Print"] = []() {
		Print << U"Hello, Siv3D!";
	};
	styles[3].callbackTable[U"ClearPrint"] = []() {
		ClearPrint();
	};
	styles[3].callbackTable[U"Siv3D_HP"] = []() {
		System::LaunchBrowser(U"https://siv3d.github.io/");
	};
	styles[3].callbackTable[U"Siv3D_GitHub"] = []() {
		System::LaunchBrowser(U"https://github.com/Siv3D/OpenSiv3D");
	};
	styles[3].callbackTable[U"Toast"] = []() {
		Platform::Windows::ToastNotification::Clear();
		const ToastNotificationItem item
		{
			.title = U"Toast Notification from Siv3D",
			.message = U"Triggered in markdown styled link.",
		};
		Platform::Windows::ToastNotification::Show(item);
	};

	TextAreaEditState input;
	input.text = texts[currentTab];
	input.rebuildGlyphs();

	Texture knife{ U"🔪"_emoji };

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
			md.draw(Vec2{ 400,50 }, 350);
			break;
		case 1:
			md.draw(Vec2{ 50,70 });
			break;
		case 2:
			Vec2 iconPos = { 200, 200 };
			knife.regionAt(iconPos).rounded(5).draw(Palette::Tan).drawFrame(3, Palette::Peru);
			if (knife.drawAt(iconPos).mouseOver()) {
				Vec2 textPos = Cursor::PosF() + Vec2{ 10, 10 };
				md.region(textPos, 350).stretched(10,10).rounded(10).draw(Palette::Tan).drawFrame(3, Palette::Peru);
				md.draw(textPos, 350);
			}
			break;
		case 3:
			md.draw(Vec2{ 50,70 });
			if (MouseL.down()) {
				effect.add<ClickEffect>(Cursor::PosF());
			}
			effect.update();
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
