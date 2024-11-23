#pragma once

class MarkdownTextStyle
{
public:
	int32 fontSize;
	FontMethod fontMethod = FontMethod::Bitmap;
	Font font;
	Font strongFont;
	Font italicFont;
	Font strongItalicFont;

	size_t mdListIndentSpaceSize = 2;
	double listIndentSize = 1;
	String listBullets = U"•◦";

	// h1(最大)サイズを1.0として、それ以外のサイズを調整する。
	// これにより Bitmap フォントかつ高解像度で表示できる。
	// あるいはBitmapが大きくなるため、SDF/MSDFフォントの方が良いかもしれない。使用する文字種数による？
	Array<double> headingScales = Array{ 0.5, 1.0, 0.8, 0.7, 0.65, 0.6, 0.55 };

	MarkdownTextStyle(const int32 _fontSize)
	{
		fontSize = int32(_fontSize / headingScales[0]);
		SetFontAuto();
	}

	void SetFontAuto()
	{
		font = Font(fontMethod, fontSize);
		strongFont = Font(fontMethod, fontSize, Typeface::Bold);
		italicFont = Font(fontMethod, fontSize, Typeface::Regular, FontStyle::Italic);
		strongItalicFont = Font(fontMethod, fontSize, Typeface::Bold, FontStyle::Italic);
	}

};
