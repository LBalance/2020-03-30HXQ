
// dlltest1View.cpp : Cdlltest1View 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "dlltest1.h"
#endif

#include "dlltest1Doc.h"
#include "dlltest1View.h"
#include "HalconCpp.h"
#include <tlhelp32.h>

#include "../Detect/Detect.h"
#include <string>
#include <locale>
#include <codecvt>
#include <wow64apiset.h >

using namespace std;
#pragma comment(lib,R"(Detect.lib)")

using namespace HalconCpp;



//#define PERSONAL_COMPUTER
//#ifdef PERSONAL_COMPUTER
////个人电脑
//#pragma comment(lib,R"(E:\Program Files\MVTec\HALCON-17.12-Progress\lib\x64-win64\halconcpp.lib)")
//#else
////办公室
//#pragma comment(lib,R"(D:\program\halcon\lib\x64-win64\halconcpp.lib)")
//#endif PERSONAL_COMPUTER



#ifdef _DEBUG
#define new DEBUG_NEW
#endif



//定义生产线排序数
int g_lineNum = ProductionLineNum2::LineXX_DS052;

HTuple mhv_WindowHandle = 0;
HTuple ghv_WindowHandle = 0;
HWND g_Hwnd = 0;
//images path
CString m_szPicFilePath;
int g_picHeight = 0;
int g_picWidth = 0;
int g_picHeightSide = 0;//reserve
int g_picWidthSide = 0;//
BYTE g_topMark = 0;
BYTE g_SideMark = 0;
BYTE g_topMark2 = 0;
//g_runNum<0时，不自动连续，自选图片。
int g_runNum = -1;
//是否加载All标记
int g_isLoadAll = 0;
//分别传入side top检测函数的参数
double top1, top2, top3, top4, top5, top6, top7, top8;
double side1, side2, side3, side4, side5, side6, side7, side8;
double top2_1, top2_2, top2_3, top2_4, top2_5, top2_6, top2_7, top2_8;



void disp_message(HTuple hv_WindowHandle, HTuple hv_String, HTuple hv_CoordSystem,
	HTuple hv_Row, HTuple hv_Column, HTuple hv_Color, HTuple hv_Box)
{

	// Local iconic variables

	// Local control variables
	HTuple  hv_Red, hv_Green, hv_Blue, hv_Row1Part;
	HTuple  hv_Column1Part, hv_Row2Part, hv_Column2Part, hv_RowWin;
	HTuple  hv_ColumnWin, hv_WidthWin, hv_HeightWin, hv_MaxAscent;
	HTuple  hv_MaxDescent, hv_MaxWidth, hv_MaxHeight, hv_R1;
	HTuple  hv_C1, hv_FactorRow, hv_FactorColumn, hv_UseShadow;
	HTuple  hv_ShadowColor, hv_Exception, hv_Width, hv_Index;
	HTuple  hv_Ascent, hv_Descent, hv_W, hv_H, hv_FrameHeight;
	HTuple  hv_FrameWidth, hv_R2, hv_C2, hv_DrawMode, hv_CurrentColor;

	//This procedure displays text in a graphics window.
	//
	//Input parameters:
	//WindowHandle: The WindowHandle of the graphics window, where
	//   the message should be displayed
	//String: A tuple of strings containing the text message to be displayed
	//CoordSystem: If set to 'window', the text position is given
	//   with respect to the window coordinate system.
	//   If set to 'image', image coordinates are used.
	//   (This may be useful in zoomed images.)
	//Row: The row coordinate of the desired text position
	//   If set to -1, a default value of 12 is used.
	//Column: The column coordinate of the desired text position
	//   If set to -1, a default value of 12 is used.
	//Color: defines the color of the text as string.
	//   If set to [], '' or 'auto' the currently set color is used.
	//   If a tuple of strings is passed, the colors are used cyclically
	//   for each new textline.
	//Box: If Box[0] is set to 'true', the text is written within an orange box.
	//     If set to' false', no box is displayed.
	//     If set to a color string (e.g. 'white', '#FF00CC', etc.),
	//       the text is written in a box of that color.
	//     An optional second value for Box (Box[1]) controls if a shadow is displayed:
	//       'true' -> display a shadow in a default color
	//       'false' -> display no shadow (same as if no second value is given)
	//       otherwise -> use given string as color string for the shadow color
	//
	//Prepare window
	GetRgb(hv_WindowHandle, &hv_Red, &hv_Green, &hv_Blue);
	GetPart(hv_WindowHandle, &hv_Row1Part, &hv_Column1Part, &hv_Row2Part, &hv_Column2Part);
	GetWindowExtents(hv_WindowHandle, &hv_RowWin, &hv_ColumnWin, &hv_WidthWin, &hv_HeightWin);
	SetPart(hv_WindowHandle, 0, 0, hv_HeightWin - 1, hv_WidthWin - 1);
	//
	//default settings
	if (0 != (hv_Row == -1))
	{
		hv_Row = 12;
	}
	if (0 != (hv_Column == -1))
	{
		hv_Column = 12;
	}
	if (0 != (hv_Color == HTuple()))
	{
		hv_Color = "";
	}
	//
	hv_String = (("" + hv_String) + "").TupleSplit("\n");
	//
	//Estimate extentions of text depending on font size.
	GetFontExtents(hv_WindowHandle, &hv_MaxAscent, &hv_MaxDescent, &hv_MaxWidth, &hv_MaxHeight);
	if (0 != (hv_CoordSystem == HTuple("window")))
	{
		hv_R1 = hv_Row;
		hv_C1 = hv_Column;
	}
	else
	{
		//Transform image to window coordinates
		hv_FactorRow = (1.*hv_HeightWin) / ((hv_Row2Part - hv_Row1Part) + 1);
		hv_FactorColumn = (1.*hv_WidthWin) / ((hv_Column2Part - hv_Column1Part) + 1);
		hv_R1 = ((hv_Row - hv_Row1Part) + 0.5)*hv_FactorRow;
		hv_C1 = ((hv_Column - hv_Column1Part) + 0.5)*hv_FactorColumn;
	}
	//
	//Display text box depending on text size
	hv_UseShadow = 1;
	hv_ShadowColor = "gray";
	if (0 != (HTuple(hv_Box[0]) == HTuple("true")))
	{
		hv_Box[0] = "#fce9d4";
		hv_ShadowColor = "#f28d26";
	}
	if (0 != ((hv_Box.TupleLength())>1))
	{
		if (0 != (HTuple(hv_Box[1]) == HTuple("true")))
		{
			//Use default ShadowColor set above
		}
		else if (0 != (HTuple(hv_Box[1]) == HTuple("false")))
		{
			hv_UseShadow = 0;
		}
		else
		{
			hv_ShadowColor = ((const HTuple&)hv_Box)[1];
			//Valid color?
			try
			{
				SetColor(hv_WindowHandle, HTuple(hv_Box[1]));
			}
			// catch (Exception) 
			catch (HalconCpp::HException &HDevExpDefaultException)
			{
				HDevExpDefaultException.ToHTuple(&hv_Exception);
				hv_Exception = "Wrong value of control parameter Box[1] (must be a 'true', 'false', or a valid color string)";
				throw HalconCpp::HException(hv_Exception);
			}
		}
	}
	if (0 != (HTuple(hv_Box[0]) != HTuple("false")))
	{
		//Valid color?
		try
		{
			SetColor(hv_WindowHandle, HTuple(hv_Box[0]));
		}
		// catch (Exception) 
		catch (HalconCpp::HException &HDevExpDefaultException)
		{
			HDevExpDefaultException.ToHTuple(&hv_Exception);
			hv_Exception = "Wrong value of control parameter Box[0] (must be a 'true', 'false', or a valid color string)";
			throw HalconCpp::HException(hv_Exception);
		}
		//Calculate box extents
		hv_String = (" " + hv_String) + " ";
		hv_Width = HTuple();
		{
			HTuple end_val93 = (hv_String.TupleLength()) - 1;
			HTuple step_val93 = 1;
			for (hv_Index = 0; hv_Index.Continue(end_val93, step_val93); hv_Index += step_val93)
			{
				GetStringExtents(hv_WindowHandle, HTuple(hv_String[hv_Index]), &hv_Ascent,
					&hv_Descent, &hv_W, &hv_H);
				hv_Width = hv_Width.TupleConcat(hv_W);
			}
		}
		hv_FrameHeight = hv_MaxHeight*(hv_String.TupleLength());
		hv_FrameWidth = (HTuple(0).TupleConcat(hv_Width)).TupleMax();
		hv_R2 = hv_R1 + hv_FrameHeight;
		hv_C2 = hv_C1 + hv_FrameWidth;
		//Display rectangles
		GetDraw(hv_WindowHandle, &hv_DrawMode);
		SetDraw(hv_WindowHandle, "fill");
		//Set shadow color
		SetColor(hv_WindowHandle, hv_ShadowColor);
		if (0 != hv_UseShadow)
		{
			DispRectangle1(hv_WindowHandle, hv_R1 + 1, hv_C1 + 1, hv_R2 + 1, hv_C2 + 1);
		}
		//Set box color
		SetColor(hv_WindowHandle, HTuple(hv_Box[0]));
		DispRectangle1(hv_WindowHandle, hv_R1, hv_C1, hv_R2, hv_C2);
		SetDraw(hv_WindowHandle, hv_DrawMode);
	}
	//Write text.
	{
		HTuple end_val115 = (hv_String.TupleLength()) - 1;
		HTuple step_val115 = 1;
		for (hv_Index = 0; hv_Index.Continue(end_val115, step_val115); hv_Index += step_val115)
		{
			hv_CurrentColor = ((const HTuple&)hv_Color)[hv_Index % (hv_Color.TupleLength())];
			if (0 != (HTuple(hv_CurrentColor != HTuple("")).TupleAnd(hv_CurrentColor != HTuple("auto"))))
			{
				SetColor(hv_WindowHandle, hv_CurrentColor);
			}
			else
			{
				SetRgb(hv_WindowHandle, hv_Red, hv_Green, hv_Blue);
			}
			hv_Row = hv_R1 + (hv_MaxHeight*hv_Index);
			SetTposition(hv_WindowHandle, hv_Row, hv_C1);
			WriteString(hv_WindowHandle, HTuple(hv_String[hv_Index]));
		}
	}
	//Reset changed window settings
	SetRgb(hv_WindowHandle, hv_Red, hv_Green, hv_Blue);
	SetPart(hv_WindowHandle, hv_Row1Part, hv_Column1Part, hv_Row2Part, hv_Column2Part);
	return;
}

// Chapter: File / Misc
// Short Description: Get all image files under the given path 
void list_image_files(HTuple hv_ImageDirectory, HTuple hv_Extensions, HTuple hv_Options,
	HTuple *hv_ImageFiles)
{

	// Local iconic variables

	// Local control variables
	HTuple  hv_ImageDirectoryIndex, hv_ImageFilesTmp;
	HTuple  hv_CurrentImageDirectory, hv_HalconImages, hv_OS;
	HTuple  hv_Directories, hv_Index, hv_Length, hv_NetworkDrive;
	HTuple  hv_Substring, hv_FileExists, hv_AllFiles, hv_i;
	HTuple  hv_Selection;

	//This procedure returns all files in a given directory
	//with one of the suffixes specified in Extensions.
	//
	//Input parameters:
	//ImageDirectory: Directory or a tuple of directories with images.
	//   If a local directory is not found, the directory is searched
	//   under %HALCONIMAGES%/ImageDirectory. If %HALCONIMAGES% is not set,
	//   %HALCONROOT%/images is used instead.
	//Extensions: A string tuple containing the extensions to be found
	//   e.g. ['png','tif',jpg'] or others
	//If Extensions is set to 'default' or the empty string '',
	//   all image suffixes supported by HALCON are used.
	//Options: as in the operator list_files, except that the 'files'
	//   option is always used. Note that the 'directories' option
	//   has no effect but increases runtime, because only files are
	//   returned.
	//
	//Output parameter:
	//ImageFiles: A tuple of all found image file names
	//
	if (0 != (HTuple(HTuple(hv_Extensions == HTuple()).TupleOr(hv_Extensions == HTuple(""))).TupleOr(hv_Extensions == HTuple("default"))))
	{
		hv_Extensions.Clear();
		hv_Extensions[0] = "ima";
		hv_Extensions[1] = "tif";
		hv_Extensions[2] = "tiff";
		hv_Extensions[3] = "gif";
		hv_Extensions[4] = "bmp";
		hv_Extensions[5] = "jpg";
		hv_Extensions[6] = "jpeg";
		hv_Extensions[7] = "jp2";
		hv_Extensions[8] = "jxr";
		hv_Extensions[9] = "png";
		hv_Extensions[10] = "pcx";
		hv_Extensions[11] = "ras";
		hv_Extensions[12] = "xwd";
		hv_Extensions[13] = "pbm";
		hv_Extensions[14] = "pnm";
		hv_Extensions[15] = "pgm";
		hv_Extensions[16] = "ppm";
		//
	}
	(*hv_ImageFiles) = HTuple();
	//Loop through all given image directories.
	{
		HTuple end_val26 = (hv_ImageDirectory.TupleLength()) - 1;
		HTuple step_val26 = 1;
		for (hv_ImageDirectoryIndex = 0; hv_ImageDirectoryIndex.Continue(end_val26, step_val26); hv_ImageDirectoryIndex += step_val26)
		{
			hv_ImageFilesTmp = HTuple();
			hv_CurrentImageDirectory = HTuple(hv_ImageDirectory[hv_ImageDirectoryIndex]);
			if (0 != (hv_CurrentImageDirectory == HTuple("")))
			{
				hv_CurrentImageDirectory = ".";
			}
			GetSystem("image_dir", &hv_HalconImages);
			GetSystem("operating_system", &hv_OS);
			if (0 != ((hv_OS.TupleSubstr(0, 2)) == HTuple("Win")))
			{
				hv_HalconImages = hv_HalconImages.TupleSplit(";");
			}
			else
			{
				hv_HalconImages = hv_HalconImages.TupleSplit(":");
			}
			hv_Directories = hv_CurrentImageDirectory;
			{
				HTuple end_val40 = (hv_HalconImages.TupleLength()) - 1;
				HTuple step_val40 = 1;
				for (hv_Index = 0; hv_Index.Continue(end_val40, step_val40); hv_Index += step_val40)
				{
					hv_Directories = hv_Directories.TupleConcat((HTuple(hv_HalconImages[hv_Index]) + "/") + hv_CurrentImageDirectory);
				}
			}
			TupleStrlen(hv_Directories, &hv_Length);
			TupleGenConst(hv_Length.TupleLength(), 0, &hv_NetworkDrive);
			if (0 != ((hv_OS.TupleSubstr(0, 2)) == HTuple("Win")))
			{
				{
					HTuple end_val46 = (hv_Length.TupleLength()) - 1;
					HTuple step_val46 = 1;
					for (hv_Index = 0; hv_Index.Continue(end_val46, step_val46); hv_Index += step_val46)
					{
						if (0 != ((HTuple(hv_Directories[hv_Index]).TupleStrlen())>1))
						{
							TupleStrFirstN(HTuple(hv_Directories[hv_Index]), 1, &hv_Substring);
							if (0 != (HTuple(hv_Substring == HTuple("//")).TupleOr(hv_Substring == HTuple("\\\\"))))
							{
								hv_NetworkDrive[hv_Index] = 1;
							}
						}
					}
				}
			}
			hv_ImageFilesTmp = HTuple();
			{
				HTuple end_val56 = (hv_Directories.TupleLength()) - 1;
				HTuple step_val56 = 1;
				for (hv_Index = 0; hv_Index.Continue(end_val56, step_val56); hv_Index += step_val56)
				{
					FileExists(HTuple(hv_Directories[hv_Index]), &hv_FileExists);
					if (0 != hv_FileExists)
					{
						ListFiles(HTuple(hv_Directories[hv_Index]), HTuple("files").TupleConcat(hv_Options),
							&hv_AllFiles);
						hv_ImageFilesTmp = HTuple();
						{
							HTuple end_val61 = (hv_Extensions.TupleLength()) - 1;
							HTuple step_val61 = 1;
							for (hv_i = 0; hv_i.Continue(end_val61, step_val61); hv_i += step_val61)
							{
								TupleRegexpSelect(hv_AllFiles, ((".*" + HTuple(hv_Extensions[hv_i])) + "$").TupleConcat("ignore_case"),
									&hv_Selection);
								hv_ImageFilesTmp = hv_ImageFilesTmp.TupleConcat(hv_Selection);
							}
						}
						TupleRegexpReplace(hv_ImageFilesTmp, (HTuple("\\\\").Append("replace_all")),
							"/", &hv_ImageFilesTmp);
						if (0 != (HTuple(hv_NetworkDrive[hv_Index])))
						{
							TupleRegexpReplace(hv_ImageFilesTmp, (HTuple("//").Append("replace_all")),
								"/", &hv_ImageFilesTmp);
							hv_ImageFilesTmp = "/" + hv_ImageFilesTmp;
						}
						else
						{
							TupleRegexpReplace(hv_ImageFilesTmp, (HTuple("//").Append("replace_all")),
								"/", &hv_ImageFilesTmp);
						}
						break;
					}
				}
			}
			//Concatenate the output image paths.
			(*hv_ImageFiles) = (*hv_ImageFiles).TupleConcat(hv_ImageFilesTmp);
		}
	}
	return;
}

DWORD ProcesstoPid()
{
	HANDLE hProcessSnap = NULL;
	DWORD ProcessId = 0;
	PROCESSENTRY32 pe32 = { 0 };
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); //打开进程快照
	if (hProcessSnap == (HANDLE)-1)
	{
		printf("\nCreateToolhelp32Snapshot() Error: %d", GetLastError());
		return 0;
	}
	pe32.dwSize = sizeof(PROCESSENTRY32);
	if (Process32First(hProcessSnap, &pe32)) //开始枚举进程
	{
		do
		{
			if (wcsstr(pe32.szExeFile, _T("HXQ")) || wcsstr(pe32.szExeFile, _T("hxq")) || wcsstr(pe32.szExeFile, _T("Hxq")))
			{
				ProcessId = pe32.th32ProcessID;
				break;
			}
		} while (Process32Next(hProcessSnap, &pe32)); //继续枚举进程
	}
	else
	{
		//printf("\nProcess32First() Error: %d", GetLastError());
		return 0;
	}
	CloseHandle(hProcessSnap); //关闭系统进程快照的句柄
	return ProcessId;
}

//第一工位工作线程函数
void Bounce(void * parg)
{
	g_topMark = 1;
	if (g_SideMark == 1 || g_topMark2 == 1)
	{
		g_SideMark = 0;
		g_topMark2 = 0;
		g_runNum = -1;
	}

	CString pathLast;
	if (g_isLoadAll == 0)
	{
		pathLast= _T(R"(\Top\Bad\)");
	}
	else
	{
		pathLast = _T(R"(\Top\All\)");
	}


	HObject Image;
	CString path;
	HTuple fullPath, subPath, fullPathLen;
	if (g_runNum >= 0)
	{
		HTuple  hv_ImageFiles, hv_Length, hv_Index, hv_Result;
		path = m_szPicFilePath + pathLast;
		//unicode 转 gb2312
		wstring_convert<codecvt<wchar_t, char, mbstate_t>> decode(::new codecvt<wchar_t, char, mbstate_t>("CHS"));
		wstring wsz(path.GetBuffer());
		string sz = decode.to_bytes(wsz);
		list_image_files(sz.data(), "default", HTuple(), &hv_ImageFiles);
		HalconCpp::TupleLength(hv_ImageFiles, &hv_Length);
		if (hv_Length - 2 < g_runNum)
			return;
		HalconCpp::ReadImage(&Image, HTuple(hv_ImageFiles[g_runNum]));

		fullPath = hv_ImageFiles[g_runNum];
		TupleStrlen(fullPath, &fullPathLen);
		TupleSubstr(fullPath, fullPathLen - 8, fullPathLen - 1, &subPath);


		g_runNum++;
	}
	else
	{
		CString strFile = _T("");
		path = m_szPicFilePath + pathLast;
		CFileDialog    dlgFile(TRUE, NULL, path, OFN_HIDEREADONLY,
			_T("Describe Files (*.tif，*.tiff，*.jpg，*.jpeg，*.png，*.bmp)|*.tif;*.tiff;*.jpg;*.jpeg;*.png;*.bmp|All Files (*.*)|*.*||"), NULL);//（*.tif，*.tiff，*.jpg，*.jpeg，*.png，*.bmp）|*.tif;*.tiff;*.jpg;*.jpeg;*.png;*.bmp
		int a = (int)dlgFile.DoModal();
		if (a == 1)
		{
			strFile = dlgFile.GetPathName();
		}
		else
			return;

		//unicode 转 gb2312
		wstring_convert<codecvt<wchar_t, char, mbstate_t>> decode(::new codecvt<wchar_t, char, mbstate_t>("CHS"));
		wstring wsz(strFile.GetBuffer());
		string sz = decode.to_bytes(wsz);
		ReadImage(&Image, sz.c_str());

		fullPath = sz.c_str();
		TupleStrlen(fullPath, &fullPathLen);
		TupleSubstr(fullPath, fullPathLen - 8, fullPathLen - 1, &subPath);
	}
	HTuple s1, s2;
	HTuple a = 1.001/*0.0102767*/, b = 0.3, c = 0, d = 1;
	DetectModule::detectTop(Image, ghv_WindowHandle, top1, top2, top3, top4, top5, top6, top7, top8, &s1, &s2);

	disp_message(mhv_WindowHandle, subPath, "window", 0,
		0, "red", "true");


	CString sz1;
	sz1.Format(_T("%d~"), s1.I());
	SetWindowText(g_Hwnd, sz1);
}

//第二工位工作线程函数
void Bounce1(void * parg)
{
	g_SideMark = 1;
	if (g_topMark == 1 || g_topMark2 == 1)
	{
		g_topMark = 0;
		g_topMark2 = 0;
		g_runNum = -1;
	}

	CString pathLast;
	if (g_isLoadAll == 0)
	{
		pathLast = _T(R"(\Side\Bad\)");
	}
	else
	{
		pathLast = _T(R"(\Side\All\)");
	}

	HObject Image;
	CString path;
	HTuple fullPath, subPath, fullPathLen;
	if (g_runNum >= 0)
	{
		HTuple  hv_ImageFiles, hv_Length, hv_Index, hv_Result;
		path = m_szPicFilePath + pathLast;
		//unicode 转 gb2312
		wstring_convert<codecvt<wchar_t, char, mbstate_t>> decode(::new codecvt<wchar_t, char, mbstate_t>("CHS"));
		wstring wsz(path.GetBuffer());
		string sz = decode.to_bytes(wsz);
		list_image_files(sz.data(), "default", HTuple(), &hv_ImageFiles);
		//list_image_files(R"(E:\换向器\2019-2-28 Images\Top\Bad)", "default", HTuple(), &hv_ImageFiles);
		//list_image_files ('F:/program files/project files/Github/2019-1-31 换向器/标定', 'default', [], ImageFiles)
		HalconCpp::TupleLength(hv_ImageFiles, &hv_Length);
		if (hv_Length - 2 < g_runNum)
			return;
		HalconCpp::ReadImage(&Image, HTuple(hv_ImageFiles[g_runNum]));
		g_runNum++;

		fullPath = hv_ImageFiles[g_runNum];
		TupleStrlen(fullPath, &fullPathLen);
		TupleSubstr(fullPath, fullPathLen - 8, fullPathLen - 1, &subPath);

		HTuple s1, s2;
		DetectModule::detectSide(Image, ghv_WindowHandle, side1, side2, side3, side4, side5, side6, side7, side8, &s1, &s2);

		disp_message(mhv_WindowHandle, subPath, "window", 0,
			0, "red", "true");

		CString sz1;
		sz1.Format(_T("%d~"), s1.I());
		SetWindowText(g_Hwnd, sz1);
	}
	else
	{
		CString strFile = _T("");
		path = m_szPicFilePath + pathLast;
		CFileDialog    dlgFile(TRUE, NULL, path, OFN_HIDEREADONLY,
			_T("Describe Files (*.tif，*.tiff，*.jpg，*.jpeg，*.png，*.bmp)|*.tif;*.tiff;*.jpg;*.jpeg;*.png;*.bmp|All Files (*.*)|*.*||"), NULL);//（*.tif，*.tiff，*.jpg，*.jpeg，*.png，*.bmp）|*.tif;*.tiff;*.jpg;*.jpeg;*.png;*.bmp
		if (dlgFile.DoModal() == 1)
		{
			strFile = dlgFile.GetPathName();
		}
		else
			return;
		//unicode 转 gb2312
		wstring_convert<codecvt<wchar_t, char, mbstate_t>> decode(::new codecvt<wchar_t, char, mbstate_t>("CHS"));
		wstring wsz0(strFile.GetBuffer());
		string sz0 = decode.to_bytes(wsz0);
		HObject Image;
		ReadImage(&Image, sz0.c_str());

		fullPath = sz0.c_str();
		TupleStrlen(fullPath, &fullPathLen);
		TupleSubstr(fullPath, fullPathLen - 8, fullPathLen - 1, &subPath);

		HTuple s1, s2;
		DetectModule::detectSide(Image, ghv_WindowHandle, side1, side2, side3, side4, side5, side6, side7, side8, &s1, &s2);

		disp_message(mhv_WindowHandle, subPath, "window", 0,
			0, "red", "true");

		CString sz1;
		sz1.Format(_T("%d~"), s1.I());
		SetWindowText(g_Hwnd, sz1);
	}
}

//第一工位顶部检测工作线程函数
void Bounce_(void * parg)
{
	g_topMark2 = 1;
	if (g_SideMark == 1 || g_topMark == 1)
	{
		g_SideMark = 0;
		g_topMark = 0;
		g_runNum = -1;
	}

	CString pathLast;
	if (g_isLoadAll == 0)
	{
		pathLast = _T(R"(\Top2\Bad\)");
	}
	else
	{
		pathLast = _T(R"(\Top2\All\)");
	}


	HObject Image;
	CString path;
	HTuple fullPath, subPath, fullPathLen;
	if (g_runNum >= 0)
	{
		HTuple  hv_ImageFiles, hv_Length, hv_Index, hv_Result;
		path = m_szPicFilePath + pathLast;
		//unicode 转 gb2312
		wstring_convert<codecvt<wchar_t, char, mbstate_t>> decode(::new codecvt<wchar_t, char, mbstate_t>("CHS"));
		wstring wsz(path.GetBuffer());
		string sz = decode.to_bytes(wsz);
		list_image_files(sz.data(), "default", HTuple(), &hv_ImageFiles);
		HalconCpp::TupleLength(hv_ImageFiles, &hv_Length);
		if (hv_Length - 2 < g_runNum)
			return;
		HalconCpp::ReadImage(&Image, HTuple(hv_ImageFiles[g_runNum]));

		fullPath = hv_ImageFiles[g_runNum];
		TupleStrlen(fullPath, &fullPathLen);
		TupleSubstr(fullPath, fullPathLen - 8, fullPathLen - 1, &subPath);


		g_runNum++;
	}
	else
	{
		CString strFile = _T("");
		path = m_szPicFilePath + pathLast;
		CFileDialog    dlgFile(TRUE, NULL, path, OFN_HIDEREADONLY,
			_T("Describe Files (*.tif，*.tiff，*.jpg，*.jpeg，*.png，*.bmp)|*.tif;*.tiff;*.jpg;*.jpeg;*.png;*.bmp|All Files (*.*)|*.*||"), NULL);//（*.tif，*.tiff，*.jpg，*.jpeg，*.png，*.bmp）|*.tif;*.tiff;*.jpg;*.jpeg;*.png;*.bmp
		int a = (int)dlgFile.DoModal();
		if (a == 1)
		{
			strFile = dlgFile.GetPathName();
		}
		else
			return;

		//unicode 转 gb2312
		wstring_convert<codecvt<wchar_t, char, mbstate_t>> decode(::new codecvt<wchar_t, char, mbstate_t>("CHS"));
		wstring wsz(strFile.GetBuffer());
		string sz = decode.to_bytes(wsz);
		ReadImage(&Image, sz.c_str());

		fullPath = sz.c_str();
		TupleStrlen(fullPath, &fullPathLen);
		TupleSubstr(fullPath, fullPathLen - 8, fullPathLen - 1, &subPath);
	}
	HTuple s1, s2;
	HTuple a = 1.001/*0.0102767*/, b = 0.3, c = 0, d = 1;
	DetectModule::detectTop2(Image, ghv_WindowHandle, top2_1, top2_1, top2_3, top2_4, top2_5, top2_6, top2_7, top2_8, &s1, &s2);

	disp_message(mhv_WindowHandle, subPath, "window", 0,
		0, "red", "true");


	CString sz1;
	sz1.Format(_T("%d~"), s1.I());
	SetWindowText(g_Hwnd, sz1);
}

//一键删除
void deletefun(CString &path)
{
	HTuple  hv_ImageFiles, hv_Length, hv_Index, hv_Result;
	//unicode 转 gb2312
	wstring_convert<codecvt<wchar_t, char, mbstate_t>> decode(::new codecvt<wchar_t, char, mbstate_t>("CHS"));
	wstring wsz(path.GetBuffer());
	string sz = decode.to_bytes(wsz);
	list_image_files(sz.data(), "default", HTuple(), &hv_ImageFiles);
	//list_image_files ('F:/program files/project files/Github/2019-1-31 换向器/标定', 'default', [], ImageFiles)
	TupleLength(hv_ImageFiles, &hv_Length);

	HTuple end_val19 = hv_Length - 1;
	HTuple step_val19 = 1;
	for (hv_Index = 0; hv_Index.Continue(end_val19, step_val19); hv_Index += step_val19)
	{
		DeleteFile(HTuple(hv_ImageFiles[hv_Index]));
	}
}

//删除所有图片工作线程
void Bounce3(void * parg)
{
	{
		CString path0 = m_szPicFilePath + _T(R"(\Top\All)");
		deletefun(path0);
	}
	{
		CString path1 = m_szPicFilePath + _T(R"(\Top\Bad)");
		deletefun(path1);
	}
	{
		CString path2 = m_szPicFilePath + _T(R"(\Side\Bad)");
		deletefun(path2);
	}
	{
		CString path3 = m_szPicFilePath + _T(R"(\Side\All)");
		deletefun(path3);
	}
	{
		CString path4 = m_szPicFilePath + _T(R"(\Top2\All)");
		deletefun(path4);
	}
	{
		CString path5 = m_szPicFilePath + _T(R"(\Top2\Bad)");
		deletefun(path5);
	}
	AfxMessageBox(_T("delete over~"));
	return;
}




/*

888888888888888888888888888888888888888888888888

*/
// Cdlltest1View

IMPLEMENT_DYNCREATE(Cdlltest1View, CFormView)

BEGIN_MESSAGE_MAP(Cdlltest1View, CFormView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_LOAD_TOP, &Cdlltest1View::OnLoadTop)
	ON_COMMAND(ID_LOAD_SIDE, &Cdlltest1View::OnLoadSide)
	ON_COMMAND(ID_LOAD_TOP2, &Cdlltest1View::OnLoadTop2)
	ON_COMMAND(ID_DELETE_ALL, &Cdlltest1View::OnDeleteAll)
	ON_COMMAND(ID_LOGIN, &Cdlltest1View::OnLogin)
	ON_COMMAND(ID_OPEN_OSK, &Cdlltest1View::OnOpenOsk)
	ON_COMMAND(ID_EXC_MODE, &Cdlltest1View::OnExcMode)
	ON_COMMAND(ID_EDIT_OKNG, &Cdlltest1View::OnID_EDIT_OKNG)
END_MESSAGE_MAP()

// Cdlltest1View 构造/析构

Cdlltest1View::Cdlltest1View()
	: CFormView(IDD_DLLTEST1_FORM)
{
	// TODO: 在此处添加构造代码
	switch (g_lineNum)
	{
	case ProductionLineNum2::LineXX_DS052: {
		m_szPicFilePath = "D:\\Images";
		g_picHeight = 2748;
		g_picWidth = 3840;
		top1 = 28;
		top2 = 27.54;
		top3 = 1.205;
		top4 =1.0;
		top5 = 0.996;
		top6 = 60;
		top7 = 0.6;
		top8 = 12;
		side1 = 0.68;/* 0.4;*/
		side2 = 0.40;/*0.3;*/
		side3 = 1000;
		side4 = 30;
		side5 = 20;
		side6 = 25000;
		side7 = 1;
		side8 = 1;
		top2_1 = 1;
		top2_2 = 1;
		top2_3 = 1;
		top2_4 = 1;
		top2_5 = 2100;
		top2_6 = 0;
		top2_7 = 1;
		top2_8 = 1;
		break;
	}
	default:
		break;
	}
}

Cdlltest1View::~Cdlltest1View()
{
}

void Cdlltest1View::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BOOL Cdlltest1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CFormView::PreCreateWindow(cs);
}

void Cdlltest1View::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}

void Cdlltest1View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void Cdlltest1View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// Cdlltest1View 诊断

#ifdef _DEBUG
void Cdlltest1View::AssertValid() const
{
	CFormView::AssertValid();
}

void Cdlltest1View::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

Cdlltest1Doc* Cdlltest1View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cdlltest1Doc)));
	return (Cdlltest1Doc*)m_pDocument;
}
#endif //_DEBUG


// Cdlltest1View 消息处理程序

BOOL Cdlltest1View::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	BOOL ret = CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);

	// TODO: 在此添加专用代码和/或调用基类



	//GetWindowRect(&rect);

	//OpenWindow(5, 5,1100, (int)1100*8000/2048, (Hlong)m_hWnd, "", "", &mhv_WindowHandle);

	return ret;
}

void Cdlltest1View::OnLoadTop()
{
	POINT  ss;
	ss.y = 0;
	ScrollToPosition(ss);
	if (0 != mhv_WindowHandle)
		HalconCpp::CloseWindow(mhv_WindowHandle);

	//OpenWindow(5, 5, 900, (int)900 * 2748 / 3840, (Hlong)m_hWnd, "", "", &mhv_WindowHandle);
	OpenWindow(5, 5, 900, (int)(900 * g_picHeight / g_picWidth), (Hlong)m_hWnd, "", "", &mhv_WindowHandle);

	ghv_WindowHandle = mhv_WindowHandle;
	g_Hwnd = m_hWnd;

	//SetPart(mhv_WindowHandle, 0, 0, 2748 , 3840);
	SetPart(mhv_WindowHandle, 0, 0, g_picHeight, g_picWidth);
	_beginthread(Bounce, 0, (void *)0);
}

void Cdlltest1View::OnLoadSide()
{
	POINT  ss;
	ss.y = 0;
	ScrollToPosition(ss);

	if (0 != mhv_WindowHandle)
		HalconCpp::CloseWindow(mhv_WindowHandle);

	OpenWindow(5, 5, 600, (int)600 * 8000 / 2048, (Hlong)m_hWnd, "", "", &mhv_WindowHandle);

	ghv_WindowHandle = mhv_WindowHandle;
	g_Hwnd = m_hWnd;
	SetPart(mhv_WindowHandle, 0, 0, 8000, 2048);

	_beginthread(Bounce1, 0, (void *)0);
}

void Cdlltest1View::OnLoadTop2()
{
	POINT  ss;
	ss.y = 0;
	ScrollToPosition(ss);
	if (0 != mhv_WindowHandle)
		HalconCpp::CloseWindow(mhv_WindowHandle);

	//OpenWindow(5, 5, 900, (int)900 * 2748 / 3840, (Hlong)m_hWnd, "", "", &mhv_WindowHandle);
	OpenWindow(5, 5, 900, (int)(900 * g_picHeight / g_picWidth), (Hlong)m_hWnd, "", "", &mhv_WindowHandle);

	ghv_WindowHandle = mhv_WindowHandle;
	g_Hwnd = m_hWnd;

	//SetPart(mhv_WindowHandle, 0, 0, 2748 , 3840);
	SetPart(mhv_WindowHandle, 0, 0, g_picHeight, g_picWidth);
	_beginthread(Bounce_, 0, (void *)0);
}

void Cdlltest1View::OnDeleteAll()
{
	// TODO: 在此添加命令处理程序代码
	_beginthread(Bounce3, 0, (void *)0);
}

void Cdlltest1View::OnLogin()
{
	// TODO: 在此添加命令处理程序代码
	if (!ProcesstoPid())
	{
		MessageBoxW(_T("目标进程未打开，请打开包含“hxq”的进程后再执行！"));
		return;
	}
	POINT point;
	GetCursorPos(&point);
	//ClientToScreen(&point);
	SetCursorPos(20, 10);
	mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	Sleep(100);
	SetCursorPos(30, 30);
	mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	Sleep(800);
	keybd_event(0x59, 0, WM_KEYDOWN | WM_KEYUP, 0);
	Sleep(50);
	keybd_event(0x43, 0, WM_KEYDOWN | WM_KEYUP, 0);
	Sleep(50);
	keybd_event(0x47, 0, WM_KEYDOWN | WM_KEYUP, 0);
	Sleep(50);
	keybd_event(0x44, 0, WM_KEYDOWN | WM_KEYUP, 0);
	Sleep(50);
	SetCursorPos(756, 502);
	mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	Sleep(50);
	//SetCursorPos(205, 45);
	//mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	::SetCursorPos(point.x, point.y);

	//打开系统键盘 “osk.exe”
	OnOpenOsk();
}

//打开系统键盘 “osk.exe”
void Cdlltest1View::OnOpenOsk()
{
	// TODO: 在此添加命令处理程序代码
	//麻辣隔壁，360hook osk，导致不能正常开启，坑劳资
	int a = WinExec(R"(C:\Windows\System32\osk.exe)", SW_SHOWNORMAL);
}

void Cdlltest1View::OnExcMode()
{
	// TODO: 在此添加命令处理程序代码
	if (g_runNum == -1)
	{
		g_runNum = 0;
	}
	else
	{
		g_runNum = -1;
	}
	AfxMessageBox(_T("切换打开模式完成~"));
}

void Cdlltest1View::OnID_EDIT_OKNG()
{
	// TODO: 在此添加命令处理程序代码
	if (g_isLoadAll == 0)
	{
		g_isLoadAll = 1;
	}
	else
	{
		g_isLoadAll = 0;
	}
	AfxMessageBox(_T("切换路径完成~"));
}