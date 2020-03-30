#include "Detect.h"
#include "outfun.h"



////**************************************************************************************************
//
//top
//
////**************************************************************************************************
//用于记录各个检测函数返回不良的累积数量
HTuple fun1NgNum=0, fun2NgNum = 0, fun3NgNum = 0, fun4NgNum = 0;


void funCorrectImage(HObject ho_image, HObject *ho_imageCorrected, HObject *ho_imageCorrected1)
{

	// Local iconic variables
	HObject  ho_Regions, ho_RegionClosing, ho_ConnectedRegions;
	HObject  ho_RegionFillUp, ho_SelectedRegions, ho_RegionUnion;
	HObject  ho_RegionDilation;

	// Local control variables
	HTuple  hv_Number;


	//copy_image (image, imageCorrected)
	//copy_image (image, imageCorrected1)

	//return ()


	Threshold(ho_image, &ho_Regions, 0, 150);
	ClosingCircle(ho_Regions, &ho_RegionClosing, 3.5);
	Connection(ho_RegionClosing, &ho_ConnectedRegions);
	FillUp(ho_ConnectedRegions, &ho_RegionFillUp);
	SelectShape(ho_RegionFillUp, &ho_SelectedRegions, "inner_radius", "and", 25, 747.22);
	Union1(ho_SelectedRegions, &ho_RegionUnion);
	DilationCircle(ho_RegionUnion, &ho_RegionDilation, 35);

	CountObj(ho_RegionDilation, &hv_Number);
	if (0 != (hv_Number == 0))
	{
		GenRegionPoints(&ho_RegionDilation, 100, 100);
	}

	PaintRegion(ho_RegionDilation, ho_image, &(*ho_imageCorrected), 255, "fill");

	CopyImage((*ho_imageCorrected), &(*ho_imageCorrected1));


	return;
}

int g_isRead = 0;
HTuple   hv_ModelID;
HObject  ho_RegionSTD;
void funMatchDifference(HObject ho_Image, HTuple hv_hwnd)
{
	// Local iconic variables
	HObject  ho_Regions, ho_ConnectedRegions, ho_SelectedRegions;
	HObject  /*ho_RegionSTD,*/ ho_RegionTrans, ho_ImageReduced2;
	HObject  ho_Regions1, ho_ConnectedRegions1, ho_SelectedRegions1;
	HObject  ho_RegionErosion, ho_RegionDifference, ho_RegionDilation;
	HObject  ho_ImageReduced1, ho_RegionAffineTrans, ho_RegionDifference1;
	HObject  ho_RegionDilation1, ho_RegionIntersection, ho_RegionOpening1;
	HObject  ho_RegionDilation2, ho_RegionDifference2, ho_RegionOpening2;
	HObject  ho_RegionDilation3;

	// Local control variables
	HTuple  hv_isCreate, /*hv_ModelID,*/ hv_Area1, hv_Row0;
	HTuple  hv_Column0, hv_areaValue, hv_Number, hv_Area;
	HTuple  hv_Row1, hv_Column1, hv_Row, hv_Column, hv_Angle;
	HTuple  hv_Score, hv_Matrix1, hv_Matrix3, hv_Matrix4;
	if (g_isRead == 0)
	{
		g_isRead = 1;
		ReadShapeModel("mode.shm", &hv_ModelID);//E:/换向器/halcon/
		ReadRegion(&ho_RegionSTD, "stdreg.reg");//E:/换向器/halcon/
	}
	AreaCenter(ho_RegionSTD, &hv_Area1, &hv_Row0, &hv_Column0);


	Threshold(ho_Image, &ho_Regions, 0, 200);
	Connection(ho_Regions, &ho_ConnectedRegions);
	SelectShapeStd(ho_ConnectedRegions, &ho_SelectedRegions, "max_area", 70);
	RegionFeatures(ho_SelectedRegions, "area", &hv_areaValue);
	if (0 != (HTuple(hv_areaValue<(3950000 - 500000)).TupleOr(hv_areaValue>(3950000 + 500000))))
	{
		disp_message(hv_hwnd, "matchDifference 选择整体区域面积eorror", "window", 1500,
			20, "red", "true");
		return;
	}
	ShapeTrans(ho_SelectedRegions, &ho_RegionTrans, "convex");
	ReduceDomain(ho_Image, ho_RegionTrans, &ho_ImageReduced2);
	Threshold(ho_ImageReduced2, &ho_Regions1, 153, 255);
	Connection(ho_Regions1, &ho_ConnectedRegions1);
	SelectShape(ho_ConnectedRegions1, &ho_SelectedRegions1, (HTuple("circularity").Append("area")),
		"and", (HTuple(0.94722).Append(709259)), (HTuple(1).Append(831481)));
	CountObj(ho_SelectedRegions1, &hv_Number);
	if (0 != (hv_Number != 1))
	{
		disp_message(hv_hwnd, "matchDifference 选择中心圆eorror", "window", 1500, 20,
			"red", "true");
		return;
	}
	AreaCenter(ho_SelectedRegions1, &hv_Area, &hv_Row1, &hv_Column1);

	//match
	ErosionCircle(ho_RegionTrans, &ho_RegionErosion, 400);
	Difference(ho_RegionTrans, ho_RegionErosion, &ho_RegionDifference);
	DilationCircle(ho_RegionDifference, &ho_RegionDilation, 100);
	ReduceDomain(ho_Image, ho_RegionDilation, &ho_ImageReduced1);
	FindShapeModel(ho_ImageReduced1, hv_ModelID, -3.14, 6.29, 0.7, 1, 0.5, "least_squares",
		0, 0.9, &hv_Row, &hv_Column, &hv_Angle, &hv_Score);
	if (0 != ((hv_Score.TupleLength()) != 1))
	{
		disp_message(hv_hwnd, "matchDifference 匹配结果eorror", "window", 1500, 20, "red",
			"true");
		return;
	}
	//move
	HomMat2dIdentity(&hv_Matrix1);
	HomMat2dRotate(hv_Matrix1, hv_Angle, hv_Row0, hv_Column0, &hv_Matrix3);
	HomMat2dTranslate(hv_Matrix3, hv_Row1 - hv_Row0, hv_Column1 - hv_Column0, &hv_Matrix4);
	AffineTransRegion(ho_RegionSTD, &ho_RegionAffineTrans, hv_Matrix4, "nearest_neighbor");

	//difference
	Difference(ho_SelectedRegions, ho_RegionAffineTrans, &ho_RegionDifference1);
	DilationCircle(ho_SelectedRegions1, &ho_RegionDilation1, 640);
	Intersection(ho_RegionDifference1, ho_RegionDilation1, &ho_RegionIntersection);
	OpeningCircle(ho_RegionIntersection, &ho_RegionOpening1, 25);
	DilationCircle(ho_RegionOpening1, &ho_RegionDilation2, 15);

	Difference(ho_RegionAffineTrans, ho_SelectedRegions, &ho_RegionDifference2);
	OpeningCircle(ho_RegionDifference2, &ho_RegionOpening2, 40);
	DilationCircle(ho_RegionOpening2, &ho_RegionDilation3, 15);

	DispObj(ho_RegionDilation2, hv_hwnd);
	DispObj(ho_RegionDilation3, hv_hwnd);
	return;
}

void fun1(HObject ho_Image, HObject *ho_region1, HTuple hv_hwnd, HTuple *hv_ret1)
{

	// Local iconic variables
	HObject  ho_EmptyRegion, ho_Regions, ho_ConnectedRegions;
	HObject  ho_SelectedRegions, ho_RegionClosing, ho_RegionFillUp;
	HObject  ho_RegionOpening1, ho_RegionDilation, ho_RegionDifference;
	HObject  ho_RegionClosing1, ho_RegionOpening, ho_ConnectedRegions1;
	HObject  ho_SelectedRegions1, ho_ObjectSelected, ho_RegionTrans;
	HObject  ho_RegionMoved, ho_Rectangle, ho_RegionErosion;
	HObject  ho_ImageReduced, ho_Regions1, ho_Rectangle1, ho_RegionClosing2;
	HObject  ho_ConnectedRegions2, ho_SelectedRegions2, ho_RegionDilation2;
	HObject  ho_ImageReduced1, ho_Regions2, ho_ConnectedRegions3;
	HObject  ho_SelectedRegions3, ho_RegionFillUp1, ho_ImageReduced2;
	HObject  ho_Regions3, ho_RegionDilation1;

	// Local control variables
	HTuple  hv_graythreshold, hv_areaoffset, hv_tongPianRadRadius;
	HTuple  hv_caoCount, hv_Row, hv_Column, hv_Width, hv_Height;
	HTuple  hv_Width1, hv_Height1, hv_w, hv_h, hv_areaValue;
	HTuple  hv_calculatedArea, hv_Area, hv_Row0, hv_Column0;
	HTuple  hv_Number, hv_debugMark, hv_Index, hv_Value, hv_Area1;
	HTuple  hv_Row1, hv_Column1, hv_phi, hv_maxRgnphi, hv_Value1;
	HTuple  hv_Value2, hv_areathreshold, hv_areaValue1, hv_IsSubset;
	HTuple  hv_Value3, hv_errorRgnArea;


	(*hv_ret1) = 0;
	hv_graythreshold = 80;
	hv_areaoffset = 500000;
	hv_tongPianRadRadius = 998;
	hv_caoCount = 22;

	 
		SetColor(hv_hwnd, "green");
	GetWindowExtents(hv_hwnd, &hv_Row, &hv_Column, &hv_Width, &hv_Height);
	GetImageSize(ho_Image, &hv_Width1, &hv_Height1);
	TupleReal(hv_Width, &hv_Width);
	TupleReal(hv_Height, &hv_Height);
	hv_w = hv_Width1 / hv_Width;
	hv_h = hv_Height1 / hv_Height;

	GenEmptyRegion(&ho_EmptyRegion);

	//提取槽区域
	Threshold(ho_Image, &ho_Regions, 0, hv_graythreshold);
	Connection(ho_Regions, &ho_ConnectedRegions);
	SelectShapeStd(ho_ConnectedRegions, &ho_SelectedRegions, "max_area", 70);
	//面积判断
	RegionFeatures(ho_SelectedRegions, "area", &hv_areaValue);
	hv_calculatedArea = (3.14*hv_tongPianRadRadius)*hv_tongPianRadRadius;
	if (0 != (HTuple(hv_areaValue<(hv_calculatedArea - hv_areaoffset)).TupleOr(hv_areaValue>(hv_calculatedArea + hv_areaoffset))))
	{
		disp_message(hv_hwnd, "topfun1 选择整体区域面积eorror", "window", (hv_Height1 - 400) / hv_h,
			20, "red", "true");
		(*hv_ret1) = -1;
		return;
	}
	ClosingCircle(ho_SelectedRegions, &ho_RegionClosing, 50);
	FillUp(ho_RegionClosing, &ho_RegionFillUp);
	OpeningCircle(ho_RegionFillUp, &ho_RegionOpening1, 800);
	DilationCircle(ho_RegionOpening1, &ho_RegionDilation, 4);
	AreaCenter(ho_RegionDilation, &hv_Area, &hv_Row0, &hv_Column0);

	Difference(ho_RegionDilation, ho_SelectedRegions, &ho_RegionDifference);
	ClosingCircle(ho_RegionDifference, &ho_RegionClosing1, 5);
	OpeningCircle(ho_RegionClosing1, &ho_RegionOpening, 10);
	Connection(ho_RegionOpening, &ho_ConnectedRegions1);
	SelectShape(ho_ConnectedRegions1, &ho_SelectedRegions1, "area", "and", 2000, 20000);

	CountObj(ho_SelectedRegions1, &hv_Number);
	if (0 != (hv_Number != hv_caoCount))
	{
		disp_message(hv_hwnd, "topfun1 提取槽区域数量 ！=" + hv_caoCount, "window", (hv_Height1 - 400) / hv_h,
			20, "red", "true");
		(*hv_ret1) = -2;
		return;
	}


	hv_debugMark = 0;
	//循环每个槽
	{
		HTuple end_val51 = hv_caoCount;
		HTuple step_val51 = 1;
		for (hv_Index = 1; hv_Index.Continue(end_val51, step_val51); hv_Index += step_val51)
		{
			SelectObj(ho_SelectedRegions1, &ho_ObjectSelected, hv_Index);
			//确认每个槽的宽、高、面积
			RegionFeatures(ho_ObjectSelected, ((HTuple("rect2_len1").Append("rect2_len2")).Append("area")),
				&hv_Value);
			if (0 != (HTuple(HTuple(hv_Value[0])>90).TupleOr(HTuple(hv_Value[0])<70)))
			{
				if (0 != (hv_debugMark == 1))
				{
					// stop(...); only in hdevelop
				}
				ConcatObj(ho_ObjectSelected, ho_EmptyRegion, &ho_EmptyRegion);
			}
			else if (0 != (HTuple(HTuple(hv_Value[1])>29).TupleOr(HTuple(hv_Value[1])<20)))
			{
				if (0 != (hv_debugMark == 1))
				{
					// stop(...); only in hdevelop
				}
				ConcatObj(ho_ObjectSelected, ho_EmptyRegion, &ho_EmptyRegion);
			}
			else if (0 != (HTuple(HTuple(hv_Value[2])>9000).TupleOr(HTuple(hv_Value[2])<6000)))
			{
				if (0 != (hv_debugMark == 1))
				{
					// stop(...); only in hdevelop
				}
				ConcatObj(ho_ObjectSelected, ho_EmptyRegion, &ho_EmptyRegion);
			}

			//阈值判断槽区域黑色区 毛刺高度
			ShapeTrans(ho_ObjectSelected, &ho_RegionTrans, "rectangle2");
			AreaCenter(ho_RegionTrans, &hv_Area1, &hv_Row1, &hv_Column1);
			MoveRegion(ho_RegionTrans, &ho_RegionMoved, (hv_Row1 - hv_Row0)*0.006, (hv_Column1 - hv_Column0)*0.006);
			//union2 (RegionTrans, RegionMoved, RegionTrans)
			RegionFeatures(ho_RegionTrans, "phi", &hv_phi);
			if (0 != (HTuple(hv_Value[1]) <= 18))
			{
				continue;
			}
			GenRectangle2(&ho_Rectangle, 100, 100, hv_phi, 0.5, HTuple(hv_Value[1]) - 18);
			Erosion1(ho_RegionMoved, ho_Rectangle, &ho_RegionErosion, 1);
			ReduceDomain(ho_Image, ho_RegionErosion, &ho_ImageReduced);
			Threshold(ho_ImageReduced, &ho_Regions1, 0, hv_graythreshold + 30);

			GenRectangle2(&ho_Rectangle1, 100, 100, hv_phi, 10, 2);
			Closing(ho_Regions1, ho_Rectangle1, &ho_RegionClosing2);
			//如果中间最大黑色区域方向与槽方向接近，则可能是槽壁反光所致
			Connection(ho_RegionClosing2, &ho_ConnectedRegions2);
			SelectShapeStd(ho_ConnectedRegions2, &ho_SelectedRegions2, "max_area", 70);
			RegionFeatures(ho_SelectedRegions2, "phi", &hv_maxRgnphi);
			RegionFeatures(ho_SelectedRegions2, "rect2_len1", &hv_Value1);
			RegionFeatures(ho_SelectedRegions2, "rect2_len2", &hv_Value2);
			if (0 != (HTuple(((hv_phi.TupleDeg()) - 20)<(hv_maxRgnphi.TupleDeg())).TupleAnd((hv_maxRgnphi.TupleDeg())<((hv_phi.TupleDeg()) + 20))))
			{
				//中间黑色区域 同向于槽方向 长和窄时，可能时反光区域
				if (0 != (hv_Value2<8))
				{
					hv_areathreshold = (hv_Value1 * 15) + 100;
				}
				else
				{
					hv_areathreshold = (hv_Value1 * 10) - 50;
				}
			}
			else if (0 != (HTuple(((hv_phi.TupleDeg()) - 70)<(hv_maxRgnphi.TupleDeg())).TupleAnd((hv_maxRgnphi.TupleDeg())<((hv_phi.TupleDeg()) + 110))))
			{
				//中间黑色区域 垂直于槽方向 长和窄时，粘铜
				hv_areathreshold = hv_Value2 * 20;
			}
			else
			{
				if (0 != (hv_Value2<8))
				{
					hv_areathreshold = (hv_Value1 * 10) + 80;
				}
				else
				{
					hv_areathreshold = (hv_Value1 * 10) - 50;
				}
			}
			if (0 != (hv_areathreshold<60))
			{
				hv_areathreshold = 60;
			}
			RegionFeatures(ho_SelectedRegions2, "area", &hv_areaValue1);
			if (0 != (hv_areaValue1>hv_areathreshold))
			{
				//判断是否为孤立的点，排除玻璃粘连污点
				DilationCircle(ho_SelectedRegions2, &ho_RegionDilation2, 30);
				ReduceDomain(ho_Image, ho_RegionDilation2, &ho_ImageReduced1);
				Threshold(ho_ImageReduced1, &ho_Regions2, 200, 255);
				Connection(ho_Regions2, &ho_ConnectedRegions3);
				SelectShapeStd(ho_ConnectedRegions3, &ho_SelectedRegions3, "max_area", 70);
				FillUp(ho_SelectedRegions3, &ho_RegionFillUp1);
				TestSubsetRegion(ho_SelectedRegions2, ho_RegionFillUp1, &hv_IsSubset);
				//如果外圈白区域围成一个圆，则斑点为大圆子集,为污点
				if (0 != (hv_IsSubset == 0))
				{
					//现在（2019-4-12）焦点升高，污点灰度降低，且粘铜检测侧面稳定，可
					//适度降低检测标准，以降低误判数量
					ReduceDomain(ho_ImageReduced1, ho_SelectedRegions2, &ho_ImageReduced2);
					Threshold(ho_ImageReduced2, &ho_Regions3, 0, 80);
					RegionFeatures(ho_Regions3, "area", &hv_Value3);
					if (0 != (hv_Value3<60))
					{
						continue;
					}
					if (0 != (hv_debugMark == 1))
					{
						// stop(...); only in hdevelop
					}
					ConcatObj(ho_RegionTrans, ho_EmptyRegion, &ho_EmptyRegion);
				}
			}

		}
	}

	Union1(ho_EmptyRegion, &(*ho_region1));
	RegionFeatures((*ho_region1), "area", &hv_errorRgnArea);
	if (0 != (HTuple(hv_errorRgnArea>0).TupleAnd((*hv_ret1) == 0)))
	{
		(*hv_ret1) = -10;
	}

	 
		SetColor(hv_hwnd, "red");
	 
		SetLineWidth(hv_hwnd, 2);
	DilationCircle((*ho_region1), &ho_RegionDilation1, 30);
	 
		DispObj(ho_RegionDilation1, hv_hwnd);
	 
		SetLineWidth(hv_hwnd, 1);

	return;
}

void fun4(HObject ho_Image, HObject *ho_region4, HTuple hv_hwnd, HTuple hv_input_Scale_1,
	HTuple hv_gouchangMin, HTuple hv_gouchangMax, HTuple *hv_ret4)
{

	// Local iconic variables
	HObject  ho_EmptyRegion, ho_Regions, ho_ConnectedRegions;
	HObject  ho_SelectedRegions, ho_RegionFillUp, ho_ImageReduced;
	HObject  ho_Regions1, ho_ConnectedRegions1, ho_SelectedRegions2;
	HObject  ho_ImageResult, ho_RegionErosion, ho_Circle, ho_RegionDifference;
	HObject  ho_Region0, ho_RegionOpening, ho_ConnectedRegions2;
	HObject  ho_RegionOpening1, ho_CircleSTD0, ho_CircleSTD1;
	HObject  ho_gouRadiusLineRight, ho_gouRadiusLineLeft, ho_ObjectSelected1;
	HObject  ho_RegionTrans, ho_circleMudle, ho_GouOuterCircle0;
	HObject  ho_GouOuterCircle1, ho_GouOuterCircle, ho_TwoSymmetryGous;
	HObject  ho_TwoSymmetryGousConvex, ho_TwoSymmetryGousOpened;
	HObject  ho_RegionIntersection, ho_RegionLines, ho_RegionIntersectionRight;
	HObject  ho_RegionIntersectionLeft, ho_diameterRegion1, ho_gouRadiusLineAll;
	HObject  ho_ObjectSelected;

	// Local control variables
	HTuple  hv_graythreshold, hv_areathreshold, hv_diameterMax;
	HTuple  hv_diameterMin, hv_areaoffset, hv_tongPianRadRadius;
	HTuple  hv_gouCount, hv_Row, hv_Column, hv_Width, hv_Height;
	HTuple  hv_Width1, hv_Height1, hv_w, hv_h, hv_areaValue;
	HTuple  hv_calculatedArea, hv_Area, hv_Number, hv_circleTimes;
	HTuple  hv_twoOutGouDiameterChangedAll, hv_GouRadiusRight;
	HTuple  hv_GouRadiusLeft, hv_Index, hv_HomMat2DIdentity;
	HTuple  hv_HomMat2DRotate, hv_TwoSymmetryGousArea, hv_twoOutGouDiameter;
	HTuple  hv_JiaoZhengValue, hv_Area1, hv_Row1, hv_Column1;
	HTuple  hv_aaa, hv_bbb, hv_cccc, hv_sina, hv_cosa, hv_rowOffset;
	HTuple  hv_columnOffset, hv_rowDispPoint, hv_columnDispPoint;
	HTuple  hv_Value1, hv_twoOutGouDiameterChanged, hv_msgShowColor;
	HTuple  hv_Area3, hv_Row3, hv_Column3, hv_SZtwoOutGouDiameterChanged;
	HTuple  hv_SZJiaoZhengValue, hv_errorRgnArea, hv_GouRadiusAll;
	HTuple  hv_GouRadiusMedian, hv_Index1, hv_Index1Last, hv_Index1Next;
	HTuple  hv_a205;


	(*hv_ret4) = 0;
	hv_graythreshold = 160;
	hv_areathreshold = 1500;
	hv_diameterMax = 2800;
	hv_diameterMin = 1365 * 2;
	hv_areaoffset = 500000;
	hv_tongPianRadRadius = 998;
	hv_gouCount = 22;



	 
		SetColor(hv_hwnd, "blue");
	GetWindowExtents(hv_hwnd, &hv_Row, &hv_Column, &hv_Width, &hv_Height);
	GetImageSize(ho_Image, &hv_Width1, &hv_Height1);
	TupleReal(hv_Width, &hv_Width);
	TupleReal(hv_Height, &hv_Height);
	hv_w = hv_Width1 / hv_Width;
	hv_h = hv_Height1 / hv_Height;

	GenEmptyRegion(&ho_EmptyRegion);

	Threshold(ho_Image, &ho_Regions, 0, hv_graythreshold);
	Connection(ho_Regions, &ho_ConnectedRegions);
	SelectShapeStd(ho_ConnectedRegions, &ho_SelectedRegions, "max_area", 70);
	//面积判断
	RegionFeatures(ho_SelectedRegions, "area", &hv_areaValue);
	hv_calculatedArea = (3.14*hv_tongPianRadRadius)*hv_tongPianRadRadius;
	if (0 != (HTuple(hv_areaValue<(hv_calculatedArea - hv_areaoffset)).TupleOr(hv_areaValue>(hv_calculatedArea + hv_areaoffset))))
	{
		disp_message(hv_hwnd, "topfun4 选择整体区域面积eorror", "window", ((hv_Height1 - 400) + 300) / hv_h,
			20, "red", "true");
		(*hv_ret4) = -1;
		return;
	}
	FillUp(ho_SelectedRegions, &ho_RegionFillUp);
	ReduceDomain(ho_Image, ho_RegionFillUp, &ho_ImageReduced);
	Threshold(ho_ImageReduced, &ho_Regions1, hv_graythreshold, 255);
	Connection(ho_Regions1, &ho_ConnectedRegions1);
	SelectShapeStd(ho_ConnectedRegions1, &ho_SelectedRegions2, "max_area", 70);
	AreaCenter(ho_SelectedRegions2, &hv_Area, &hv_Row, &hv_Column);
	//2019-07-2改 有钩顶面反光造成白色 填充为黑色
	//dilation_circle (SelectedRegions2, RegionDilation, 80)
	//paint_region (RegionDilation, Image, ImageResult, 30, 'fill')
	//*gen_circle(Circle0, Row, Column, 5)
	ErosionCircle(ho_RegionFillUp, &ho_RegionErosion, 30);
	PaintRegion(ho_RegionErosion, ho_Image, &ho_ImageResult, 10, "fill");

	GenCircle(&ho_Circle, hv_Row, hv_Column, hv_tongPianRadRadius + 27);
	Difference(ho_SelectedRegions, ho_Circle, &ho_RegionDifference);
	GenRegionPoints(&ho_Region0, hv_Row, hv_Column);

	OpeningCircle(ho_RegionDifference, &ho_RegionOpening, 8);
	Connection(ho_RegionOpening, &ho_ConnectedRegions2);
	SelectShape(ho_ConnectedRegions2, &ho_ConnectedRegions2, "area", "and", 9000, 28000);
	CountObj(ho_ConnectedRegions2, &hv_Number);
	hv_circleTimes = hv_gouCount / 2;
	if (0 != (hv_Number != hv_gouCount))
	{
		disp_message(hv_hwnd, "topfun4 提取钩部区域数量 ！=" + hv_gouCount, "window", ((hv_Height1 - 400) + 300) / hv_h,
			20, "red", "true");
		(*hv_ret4) = -2;
		if (0 != ((hv_Number % 2) == 1))
		{
			hv_circleTimes = (hv_Number + 1) / 2;
		}
		//return ()
	}



	OpeningCircle(ho_RegionFillUp, &ho_RegionOpening1, 800);

	GenCircle(&ho_CircleSTD0, hv_Row, hv_Column, hv_tongPianRadRadius - 2);
	GenCircle(&ho_CircleSTD1, hv_Row, hv_Column, hv_tongPianRadRadius + 202);

	 
		DispObj(ho_CircleSTD1, hv_hwnd);

	hv_twoOutGouDiameterChangedAll = HTuple();
	hv_GouRadiusRight = HTuple();
	hv_GouRadiusLeft = HTuple();
	GenEmptyRegion(&ho_gouRadiusLineRight);
	GenEmptyRegion(&ho_gouRadiusLineLeft);



	SelectObj(ho_ConnectedRegions2, &ho_ObjectSelected1, 1);
	ShapeTrans(ho_ObjectSelected1, &ho_RegionTrans, "outer_circle");
	DilationCircle(ho_RegionTrans, &ho_circleMudle, 50);

	{
		HTuple end_val85 = hv_circleTimes;
		HTuple step_val85 = 1;
		for (hv_Index = 1; hv_Index.Continue(end_val85, step_val85); hv_Index += step_val85)
		{
			HomMat2dIdentity(&hv_HomMat2DIdentity);
			HomMat2dRotate(hv_HomMat2DIdentity, -(((360 / 22)*(hv_Index - 1)).TupleRad()), hv_Row,
				hv_Column, &hv_HomMat2DRotate);
			AffineTransRegion(ho_circleMudle, &ho_GouOuterCircle0, hv_HomMat2DRotate, "constant");

			HomMat2dIdentity(&hv_HomMat2DIdentity);
			HomMat2dRotate(hv_HomMat2DIdentity, HTuple(180).TupleRad(), hv_Row, hv_Column,
				&hv_HomMat2DRotate);
			AffineTransRegion(ho_GouOuterCircle0, &ho_GouOuterCircle1, hv_HomMat2DRotate,
				"constant");

			Union2(ho_GouOuterCircle0, ho_GouOuterCircle1, &ho_GouOuterCircle);
			Intersection(ho_GouOuterCircle, ho_ConnectedRegions2, &ho_TwoSymmetryGous);
			RegionFeatures(ho_TwoSymmetryGous, "area", &hv_TwoSymmetryGousArea);
			if (0 != (hv_TwoSymmetryGousArea<22000))
			{
				disp_message(hv_hwnd, "topfun4 未找到对称区域", "window", ((hv_Height1 - 400) + 300) / hv_h,
					20, "red", "true");
				(*hv_ret4) = -3;
			}
			ShapeTrans(ho_TwoSymmetryGous, &ho_TwoSymmetryGousConvex, "convex");
			OpeningCircle(ho_TwoSymmetryGousConvex, &ho_TwoSymmetryGousOpened, 30);
			RegionFeatures(ho_TwoSymmetryGousOpened, "max_diameter", &hv_twoOutGouDiameter);

			//矫正
			//intersection (TwoSymmetryGousConvex, RegionOpening1, RegionIntersection1)
			//*     region_features (RegionIntersection1, 'max_diameter', Value)
			//*     JiaoZhengValue := 994*2-Value
			hv_JiaoZhengValue = 0;

			//半径线条及其长度
			//right-up
			Intersection(ho_TwoSymmetryGous, ho_GouOuterCircle0, &ho_RegionIntersection);
			AreaCenter(ho_RegionIntersection, &hv_Area1, &hv_Row1, &hv_Column1);
			hv_aaa = hv_Row - hv_Row1;
			hv_bbb = hv_Column - hv_Column1;
			TupleSqrt((hv_aaa*hv_aaa) + (hv_bbb*hv_bbb), &hv_cccc);
			hv_sina = hv_aaa / hv_cccc;
			hv_cosa = hv_bbb / hv_cccc;
			hv_rowOffset = 1250 * hv_sina;
			hv_columnOffset = 1250 * hv_cosa;
			hv_rowDispPoint = hv_Row - hv_rowOffset;
			hv_columnDispPoint = hv_Column - hv_columnOffset;
			GenRegionLine(&ho_RegionLines, hv_Row, hv_Column, hv_rowDispPoint, hv_columnDispPoint);
			Intersection(ho_RegionLines, ho_TwoSymmetryGousOpened, &ho_RegionIntersectionRight
			);
			if (0 != (hv_Index == 1))
			{
				Union1(ho_RegionIntersectionRight, &ho_gouRadiusLineRight);
			}
			else
			{
				ConcatObj(ho_gouRadiusLineRight, ho_RegionIntersectionRight, &ho_gouRadiusLineRight
				);
			}
			RegionFeatures(ho_RegionIntersectionRight, "max_diameter", &hv_Value1);
			hv_GouRadiusRight = hv_GouRadiusRight.TupleConcat(hv_Value1);


			//left-down
			Intersection(ho_TwoSymmetryGous, ho_GouOuterCircle1, &ho_RegionIntersection);
			AreaCenter(ho_RegionIntersection, &hv_Area1, &hv_Row1, &hv_Column1);
			hv_aaa = hv_Row - hv_Row1;
			hv_bbb = hv_Column - hv_Column1;
			TupleSqrt((hv_aaa*hv_aaa) + (hv_bbb*hv_bbb), &hv_cccc);
			hv_sina = hv_aaa / hv_cccc;
			hv_cosa = hv_bbb / hv_cccc;
			hv_rowOffset = 1250 * hv_sina;
			hv_columnOffset = 1250 * hv_cosa;
			hv_rowDispPoint = hv_Row - hv_rowOffset;
			hv_columnDispPoint = hv_Column - hv_columnOffset;
			GenRegionLine(&ho_RegionLines, hv_Row, hv_Column, hv_rowDispPoint, hv_columnDispPoint);
			Intersection(ho_RegionLines, ho_TwoSymmetryGousOpened, &ho_RegionIntersectionLeft
			);
			if (0 != (hv_Index == 1))
			{
				Union1(ho_RegionIntersectionLeft, &ho_gouRadiusLineLeft);
			}
			else
			{
				ConcatObj(ho_gouRadiusLineLeft, ho_RegionIntersectionLeft, &ho_gouRadiusLineLeft
				);
			}
			RegionFeatures(ho_RegionIntersectionLeft, "max_diameter", &hv_Value1);
			hv_GouRadiusLeft = hv_GouRadiusLeft.TupleConcat(hv_Value1);

			Union2(ho_RegionIntersectionRight, ho_RegionIntersectionLeft, &ho_diameterRegion1
			);

			//直径长度判断
			hv_twoOutGouDiameterChanged = (((hv_twoOutGouDiameter + hv_JiaoZhengValue) + 0.0)*hv_input_Scale_1)*0.011736;
			if (0 != (HTuple(hv_twoOutGouDiameterChanged<(hv_gouchangMin - 0.01)).TupleOr(hv_twoOutGouDiameterChanged>(hv_gouchangMax + 0.01))))
			{
				hv_msgShowColor = "red";
				ConcatObj(ho_EmptyRegion, ho_diameterRegion1, &ho_EmptyRegion);
			}
			else
			{
				if (0 != (HTuple((hv_gouchangMin - hv_twoOutGouDiameterChanged) <= 0.01).TupleAnd((hv_gouchangMin - hv_twoOutGouDiameterChanged)>0)))
				{
					hv_twoOutGouDiameterChanged = hv_gouchangMin;
				}
				else if (0 != (HTuple((hv_twoOutGouDiameterChanged - hv_gouchangMax) <= 0.01).TupleAnd((hv_twoOutGouDiameterChanged - hv_gouchangMax)>0)))
				{
					hv_twoOutGouDiameterChanged = hv_gouchangMax;
				}
				 
					SetColor(hv_hwnd, "green");
				hv_msgShowColor = "blue";
				 
					DispObj(ho_diameterRegion1, hv_hwnd);
			}
			hv_twoOutGouDiameterChangedAll = hv_twoOutGouDiameterChangedAll.TupleConcat(hv_twoOutGouDiameterChanged);


			//外径数值显示
			AreaCenter(ho_GouOuterCircle0, &hv_Area3, &hv_Row3, &hv_Column3);
			TupleSqrt(((hv_Row - hv_Row3)*(hv_Row - hv_Row3)) + ((hv_Column - hv_Column3)*(hv_Column - hv_Column3)),
				&hv_cccc);
			hv_sina = (hv_Row - hv_Row3) / hv_cccc;
			hv_cosa = (hv_Column - hv_Column3) / hv_cccc;
			hv_rowOffset = 1250 * hv_sina;
			hv_columnOffset = (1250 * hv_cosa) + (hv_Index * 6);
			hv_rowDispPoint = (hv_Row - hv_rowOffset) - 30;
			hv_columnDispPoint = hv_Column - hv_columnOffset;


			hv_SZtwoOutGouDiameterChanged = hv_twoOutGouDiameterChanged.TupleString(".4");
			hv_SZJiaoZhengValue = hv_JiaoZhengValue.TupleString(".3");
			disp_message(hv_hwnd, (hv_SZtwoOutGouDiameterChanged + " ") + hv_SZJiaoZhengValue,
				"window", hv_rowDispPoint / hv_h, hv_columnDispPoint / hv_w, hv_msgShowColor,
				"false");
		}
	}

	//当直径大于且靠近最小标准值时，与它相邻两半径都明显大于它时也判为凹角（钩外径小）
	Union1(ho_EmptyRegion, &(*ho_region4));
	RegionFeatures((*ho_region4), "area", &hv_errorRgnArea);
	if (0 != (HTuple(hv_errorRgnArea == 0).TupleAnd((*hv_ret4) == 0)))
	{
		TupleConcat(hv_GouRadiusRight, hv_GouRadiusLeft, &hv_GouRadiusAll);
		TupleMedian(hv_GouRadiusAll, &hv_GouRadiusMedian);
		ConcatObj(ho_gouRadiusLineRight, ho_gouRadiusLineLeft, &ho_gouRadiusLineAll);
		{
			HTuple end_val200 = hv_gouCount - 1;
			HTuple step_val200 = 1;
			for (hv_Index1 = 0; hv_Index1.Continue(end_val200, step_val200); hv_Index1 += step_val200)
			{
				if (0 != (HTuple(HTuple(hv_GouRadiusAll[hv_Index1])<(hv_GouRadiusMedian - 5)).TupleOr(HTuple(hv_GouRadiusAll[hv_Index1])>(hv_GouRadiusMedian + 5))))
				{
					hv_Index1Last = ((hv_Index1 - 1) + 22) % 22;
					hv_Index1Next = (hv_Index1 + 1) % 22;
					hv_a205 = 10;
					if (0 != (HTuple(HTuple(HTuple((HTuple(hv_GouRadiusAll[hv_Index1]) - HTuple(hv_GouRadiusAll[hv_Index1Last]))>hv_a205).TupleOr((HTuple(hv_GouRadiusAll[hv_Index1]) - HTuple(hv_GouRadiusAll[hv_Index1Last]))<(-hv_a205))).TupleOr((HTuple(hv_GouRadiusAll[hv_Index1]) - HTuple(hv_GouRadiusAll[hv_Index1Next]))>hv_a205)).TupleOr((HTuple(hv_GouRadiusAll[hv_Index1]) - HTuple(hv_GouRadiusAll[hv_Index1Next]))<(-hv_a205))))
					{
						SelectObj(ho_gouRadiusLineAll, &ho_ObjectSelected, hv_Index1 + 1);
						ConcatObj(ho_EmptyRegion, ho_ObjectSelected, &ho_EmptyRegion);
						(*hv_ret4) = -9;
					}
				}
			}
		}
	}

	Union1(ho_EmptyRegion, &(*ho_region4));
	RegionFeatures((*ho_region4), "area", &hv_errorRgnArea);
	if (0 != (HTuple(hv_errorRgnArea>0).TupleAnd((*hv_ret4) == 0)))
	{
		(*hv_ret4) = -10;
	}
	if (0 != ((*hv_ret4) == -9))
	{
		 
			SetColor(hv_hwnd, "magenta");
	}
	else
	{
		 
			SetColor(hv_hwnd, "red");
	}

	 
		DispObj((*ho_region4), hv_hwnd);
	//hv_hwnd
	return;

}

void fun3(HObject ho_Image, HObject ho_gouTopRegions, HObject *ho_region3, HTuple hv_hwnd,
	HTuple hv_GouTaoWide, HTuple hv_GouGouTaoIntersectionInner_radius, HTuple *hv_ret3)
{

	// Local iconic variables
	HObject  ho_EmptyRegion, ho_Regions, ho_ConnectedRegions;
	HObject  ho_SelectedRegions, ho_RegionFillUp, ho_RegionOpening;
	HObject  ho_CircleCheckBase, ho_CircleCheckBaseDilation;
	HObject  ho_CircleCheckBaseCircle, ho_gouTopConnectedRegions;
	HObject  ho_ObjectSelectedBase, ho_RegionLinesBase, ho_gouRegionSTD0;
	HObject  ho_RegionLinesBaseaffined, ho_gouRegionSTD1, ho_RegionDifference;
	HObject  ho_gouTaoBaseIn, ho_gouTaoBaseOut, ho_gouTaoOutCircle;
	HObject  ho_gouTao, ho_useToShow, ho_gouTaoNow, ho_ImageReduced;
	HObject  ho_Regions1, ho_ConnectedRegions1, ho_SelectedRegions1;
	HObject  ho_RegionDilation;

	// Local control variables
	HTuple  hv_graythreshold, hv_tongPianRadRadius;
	HTuple  hv_gouCount, hv_Row, hv_Column, hv_Width, hv_Height;
	HTuple  hv_Width1, hv_Height1, hv_w, hv_h, hv_Area, hv_LengthJudgeNum;
	HTuple  hv_Area1, hv_Row1, hv_Column1, hv_cccc, hv_sina;
	HTuple  hv_cosa, hv_rowOffset, hv_columnOffset, hv_Index;
	HTuple  hv_ratateNum, hv_HomMat2DIdentity, hv_HomMat2DRotate;
	HTuple  hv_isOkMark, hv_isDebugMark, hv_ValueIR, hv_Max;
	HTuple  hv_Indices, hv_Value, hv_errorRgnArea;


	(*hv_ret3) = 0;
	hv_graythreshold = 160;
	if (0 != (hv_GouTaoWide <= 0))
	{
		hv_GouTaoWide = 1;
	}
	hv_tongPianRadRadius = 998;
	hv_gouCount = 22;



	 
		SetColor(hv_hwnd, "green");
	GetWindowExtents(hv_hwnd, &hv_Row, &hv_Column, &hv_Width, &hv_Height);
	GetImageSize(ho_Image, &hv_Width1, &hv_Height1);
	TupleReal(hv_Width, &hv_Width);
	TupleReal(hv_Height, &hv_Height);
	hv_w = hv_Width1 / hv_Width;
	hv_h = hv_Height1 / hv_Height;

	GenEmptyRegion(&ho_EmptyRegion);

	//提取圆心，此处与其他几个函数不同，通过外圆确认圆心
	Threshold(ho_Image, &ho_Regions, 0, hv_graythreshold);
	Connection(ho_Regions, &ho_ConnectedRegions);
	SelectShapeStd(ho_ConnectedRegions, &ho_SelectedRegions, "max_area", 70);
	FillUp(ho_SelectedRegions, &ho_RegionFillUp);
	OpeningCircle(ho_RegionFillUp, &ho_RegionOpening, 800);
	AreaCenter(ho_RegionOpening, &hv_Area, &hv_Row, &hv_Column);

	//执行到此处时钩宽度一定合格
	//长度判断缩放阀值
	hv_LengthJudgeNum = hv_GouTaoWide;
	//钩偏检测改良版，通过钩间距离（钩间区域的宽度）来判断还是不行，改用套钩工具的原理检测
	GenCircle(&ho_CircleCheckBase, hv_Row, hv_Column, hv_tongPianRadRadius + 102);
	DilationCircle(ho_CircleCheckBase, &ho_CircleCheckBaseDilation, 110);
	Difference(ho_CircleCheckBaseDilation, ho_CircleCheckBase, &ho_CircleCheckBaseCircle
	);
	Connection(ho_gouTopRegions, &ho_gouTopConnectedRegions);

	//建立外框
	SelectObj(ho_gouTopConnectedRegions, &ho_ObjectSelectedBase, 1);
	AreaCenter(ho_ObjectSelectedBase, &hv_Area1, &hv_Row1, &hv_Column1);

	TupleSqrt(((hv_Row - hv_Row1)*(hv_Row - hv_Row1)) + ((hv_Column - hv_Column1)*(hv_Column - hv_Column1)),
		&hv_cccc);

	hv_sina = (hv_Row - hv_Row1) / hv_cccc;
	hv_cosa = ((hv_Column - hv_Column1) - 10) / hv_cccc;
	hv_rowOffset = 1250 * hv_sina;
	hv_columnOffset = 1250 * hv_cosa;

	GenRegionLine(&ho_RegionLinesBase, hv_Row, hv_Column, hv_Row - hv_rowOffset, hv_Column - hv_columnOffset);
	GenEmptyRegion(&ho_gouRegionSTD0);
	ConcatObj(ho_gouRegionSTD0, ho_RegionLinesBase, &ho_gouRegionSTD0);
	{
		HTuple end_val52 = hv_gouCount - 1;
		HTuple step_val52 = 1;
		for (hv_Index = 1; hv_Index.Continue(end_val52, step_val52); hv_Index += step_val52)
		{
			hv_ratateNum = ((HTuple(360).TupleRad()) / 22)*hv_Index;
			HomMat2dIdentity(&hv_HomMat2DIdentity);
			HomMat2dRotate(hv_HomMat2DIdentity, hv_ratateNum, hv_Row, hv_Column, &hv_HomMat2DRotate);
			AffineTransRegion(ho_RegionLinesBase, &ho_RegionLinesBaseaffined, hv_HomMat2DRotate,
				"constant");
			ConcatObj(ho_gouRegionSTD0, ho_RegionLinesBaseaffined, &ho_gouRegionSTD0);
		}
	}
	Union1(ho_gouRegionSTD0, &ho_gouRegionSTD1);
	Difference(ho_gouRegionSTD1, ho_CircleCheckBase, &ho_RegionDifference);
	//钩套内孔宽度
	DilationCircle(ho_RegionDifference, &ho_gouTaoBaseIn, hv_GouTaoWide);
	DilationCircle(ho_gouTaoBaseIn, &ho_gouTaoBaseOut, 20);
	Difference(ho_gouTaoBaseOut, ho_gouTaoBaseIn, &ho_gouTaoOutCircle);
	Intersection(ho_gouTaoOutCircle, ho_CircleCheckBaseCircle, &ho_gouTao);

	hv_isOkMark = 0;
	hv_isDebugMark = 0;
	GenEmptyRegion(&ho_useToShow);
	for (hv_Index = 0; hv_Index <= 1.2; hv_Index += 0.05)
	{
		hv_ratateNum = hv_Index.TupleRad();
		HomMat2dIdentity(&hv_HomMat2DIdentity);
		HomMat2dRotate(hv_HomMat2DIdentity, hv_ratateNum, hv_Row, hv_Column, &hv_HomMat2DRotate);
		AffineTransRegion(ho_gouTao, &ho_gouTaoNow, hv_HomMat2DRotate, "nearest_neighbor");

		ReduceDomain(ho_Image, ho_gouTaoNow, &ho_ImageReduced);
		Threshold(ho_ImageReduced, &ho_Regions1, 0, 150);
		Connection(ho_Regions1, &ho_ConnectedRegions1);
		RegionFeatures(ho_ConnectedRegions1, "inner_radius", &hv_ValueIR);
		TupleMax(hv_ValueIR, &hv_Max);
		if (0 != (hv_Max >= hv_GouGouTaoIntersectionInner_radius))
		{
			TupleFind(hv_ValueIR, hv_Max, &hv_Indices);
			SelectObj(ho_ConnectedRegions1, &ho_SelectedRegions1, hv_Indices + 1);
			SelectShapeStd(ho_SelectedRegions1, &ho_SelectedRegions1, "max_area", 70);
		}
		else
		{
			SelectShapeStd(ho_ConnectedRegions1, &ho_SelectedRegions1, "max_area", 70);
		}
		RegionFeatures(ho_SelectedRegions1, ((HTuple("area").Append("inner_radius")).Append("max_diameter")),
			&hv_Value);

		if (0 != hv_isDebugMark)
		{
			 
				ClearWindow(hv_hwnd);
			 
				SetColor(hv_hwnd, "green");
			 
				DispObj(ho_Image, hv_hwnd);
			 
				DispObj(ho_gouTaoNow, hv_hwnd);
			 
				SetColor(hv_hwnd, "red");
			DilationCircle(ho_SelectedRegions1, &ho_RegionDilation, 15);
			 
				DispObj(ho_RegionDilation, hv_hwnd);
			// stop(...); only in hdevelop
		}
		//判断
		if (0 != (HTuple(HTuple(HTuple(hv_Value[0])<200).TupleAnd(HTuple(hv_Value[1])<hv_GouGouTaoIntersectionInner_radius)).TupleAnd(HTuple(hv_Value[2])<70)))
		{
			hv_isOkMark = 1;
			if (0 != hv_isDebugMark)
			{
				disp_message(hv_hwnd, HTuple("钩居中,退出循环------>>>>>"), "window", ((hv_Height1 - 400) + 100) / hv_h,
					20, "blue", "true");
				// stop(...); only in hdevelop
			}

			disp_message(hv_hwnd, (((("topfun3 钩套交Msg area/inR/D=" + HTuple(hv_Value[0])) + "/") + HTuple(hv_Value[1])) + "/") + HTuple(hv_Value[2]),
				"window", ((hv_Height1 - 400) + 100) / hv_h, 20, "blue", "true");
			break;
		}
	}

	if (0 != hv_isDebugMark)
	{
		disp_message(hv_hwnd, "退出循环------>>>>>", "window", ((hv_Height1 - 400) + 100) / hv_h,
			20, "blue", "true");
		// stop(...); only in hdevelop
	}

	if (0 != (hv_isOkMark == 0))
	{
		hv_ratateNum = HTuple(0.5).TupleRad();
		HomMat2dIdentity(&hv_HomMat2DIdentity);
		HomMat2dRotate(hv_HomMat2DIdentity, hv_ratateNum, hv_Row, hv_Column, &hv_HomMat2DRotate);
		AffineTransRegion(ho_gouTao, &ho_useToShow, hv_HomMat2DRotate, "nearest_neighbor");

		ConcatObj(ho_EmptyRegion, ho_useToShow, &ho_EmptyRegion);
		//disp_message (hwnd, 'topfun3 钩不居中', 'window', (Height1-400+100)/h, 20, 'red', 'true')
		//ret3 := -1
		//return ()
	}

	Union1(ho_EmptyRegion, &(*ho_region3));
	RegionFeatures((*ho_region3), "area", &hv_errorRgnArea);
	if (0 != (HTuple(hv_errorRgnArea>0).TupleAnd((*hv_ret3) == 0)))
	{
		(*hv_ret3) = -10;
		 
			SetColor(hv_hwnd, "red");
		 
			DispObj((*ho_region3), hv_hwnd);
	}

	return;
}

void fun2(HObject ho_Image, HObject *ho_gouTopRegions, HObject *ho_region2, HTuple hv_hwnd,
	HTuple hv_goukuanMin, HTuple hv_goukuanMax, HTuple *hv_ret2)
{

	// Local iconic variables
	HObject  ho_EmptyRegion, ho_Regions, ho_ConnectedRegions;
	HObject  ho_SelectedRegions, ho_RegionFillUp, ho_ImageReduced;
	HObject  ho_Regions1, ho_ConnectedRegions1, ho_SelectedRegions2;
	HObject  ho_Region0, ho_Circle, ho_RegionDifference, ho_RegionOpening;
	HObject  ho_ConnectedRegions2, ho_RegionErosion1, ho_RegionErosion2;
	HObject  ho_RegionDifference1, ho_ImageReduced1, ho_Regions2;
	HObject  ho_RegionOpening1, ho_ConnectedRegions3, ho_ObjectSelected;
	HObject  ho_RegionUnion;

	// Local control variables
	HTuple  hv_graythreshold, hv_areathreshold, hv_areaoffset;
	HTuple  hv_goukuanoffset, hv_tongPianRadRadius, hv_gouCount;
	HTuple  hv_Row, hv_Column, hv_Width, hv_Height, hv_Width1;
	HTuple  hv_Height1, hv_w, hv_h, hv_areaValue, hv_calculatedArea;
	HTuple  hv_Area, hv_Number, hv_Number1, hv_tuple0, hv_tuple1;
	HTuple  hv_Index, hv_len2Value, hv_errClear, hv_errorRgnArea;


	(*hv_ret2) = 0;
	hv_graythreshold = 160;
	hv_areathreshold = 1500;
	hv_areaoffset = 500000;
	hv_goukuanoffset = 15;
	hv_tongPianRadRadius = 998;
	hv_gouCount = 22;

	 
		SetColor(hv_hwnd, "green");
	GetWindowExtents(hv_hwnd, &hv_Row, &hv_Column, &hv_Width, &hv_Height);
	GetImageSize(ho_Image, &hv_Width1, &hv_Height1);
	TupleReal(hv_Width, &hv_Width);
	TupleReal(hv_Height, &hv_Height);
	hv_w = hv_Width1 / hv_Width;
	hv_h = hv_Height1 / hv_Height;

	GenEmptyRegion(&ho_EmptyRegion);

	Threshold(ho_Image, &ho_Regions, 0, hv_graythreshold);
	Connection(ho_Regions, &ho_ConnectedRegions);
	SelectShapeStd(ho_ConnectedRegions, &ho_SelectedRegions, "max_area", 70);
	//面积判断
	RegionFeatures(ho_SelectedRegions, "area", &hv_areaValue);
	hv_calculatedArea = (3.14*hv_tongPianRadRadius)*hv_tongPianRadRadius;
	if (0 != (HTuple(hv_areaValue<(hv_calculatedArea - hv_areaoffset)).TupleOr(hv_areaValue>(hv_calculatedArea + hv_areaoffset))))
	{
		disp_message(hv_hwnd, "topfun2 选择整体区域面积eorror", "window", ((hv_Height1 - 400) + 100) / hv_h,
			20, "red", "true");
		(*hv_ret2) = -1;
		return;
	}
	FillUp(ho_SelectedRegions, &ho_RegionFillUp);
	ReduceDomain(ho_Image, ho_RegionFillUp, &ho_ImageReduced);
	Threshold(ho_ImageReduced, &ho_Regions1, hv_graythreshold, 255);
	Connection(ho_Regions1, &ho_ConnectedRegions1);
	SelectShapeStd(ho_ConnectedRegions1, &ho_SelectedRegions2, "max_area", 70);
	AreaCenter(ho_SelectedRegions2, &hv_Area, &hv_Row, &hv_Column);
	GenRegionPoints(&ho_Region0, hv_Row, hv_Column);
	GenCircle(&ho_Circle, hv_Row, hv_Column, hv_tongPianRadRadius + 112);

	Difference(ho_SelectedRegions, ho_Circle, &ho_RegionDifference);
	OpeningCircle(ho_RegionDifference, &ho_RegionOpening, 9);
	Connection(ho_RegionOpening, &ho_ConnectedRegions2);
	SelectShape(ho_ConnectedRegions2, &ho_ConnectedRegions2, "area", "and", 1500, 12000);
	CountObj(ho_ConnectedRegions2, &hv_Number);
	if (0 != (hv_Number != hv_gouCount))
	{
		disp_message(hv_hwnd, "topfun2 提取钩顶区域数量 ！=" + hv_gouCount, "window", ((hv_Height1 - 400) + 100) / hv_h,
			20, "red", "true");
		(*hv_ret2) = -2;
		return;
	}
	Union1(ho_ConnectedRegions2, &(*ho_gouTopRegions));

	ErosionCircle(ho_Circle, &ho_RegionErosion1, 70);
	ErosionCircle(ho_Circle, &ho_RegionErosion2, 125);
	Difference(ho_RegionErosion1, ho_RegionErosion2, &ho_RegionDifference1);
	ReduceDomain(ho_Image, ho_RegionDifference1, &ho_ImageReduced1);
	Threshold(ho_ImageReduced1, &ho_Regions2, 0, 120);
	OpeningCircle(ho_Regions2, &ho_RegionOpening1, 20);
	Connection(ho_RegionOpening1, &ho_ConnectedRegions3);
	SelectShape(ho_ConnectedRegions3, &ho_ConnectedRegions3, "area", "and", 4000, 7000);
	CountObj(ho_ConnectedRegions3, &hv_Number1);
	if (0 != (hv_Number1 != hv_gouCount))
	{
		disp_message(hv_hwnd, "topfun2 提取钩跟区域数量 ！=" + hv_gouCount, "window", ((hv_Height1 - 400) + 100) / hv_h,
			20, "red", "true");
		(*hv_ret2) = -3;
		return;
	}

	//钩宽范围判断
	hv_tuple0 = HTuple();
	hv_tuple1 = HTuple();

	{
		HTuple end_val70 = hv_gouCount;
		HTuple step_val70 = 1;
		for (hv_Index = 1; hv_Index.Continue(end_val70, step_val70); hv_Index += step_val70)
		{
			//钩顶宽
			SelectObj(ho_ConnectedRegions2, &ho_ObjectSelected, hv_Index);
			Union2(ho_ObjectSelected, ho_Region0, &ho_RegionUnion);
			RegionFeatures(ho_RegionUnion, "rect2_len2", &hv_len2Value);
			//len2Value := round(len2Value)*2*0.011736
			hv_len2Value = (hv_len2Value * 2)*0.011736;
			hv_tuple0 = hv_len2Value.TupleConcat(hv_tuple0);
			hv_errClear = 0.05;
			if (0 != (HTuple(hv_len2Value<(hv_goukuanMin - hv_errClear)).TupleOr(hv_len2Value>(hv_goukuanMax + hv_errClear))))
			{
				ConcatObj(ho_EmptyRegion, ho_ObjectSelected, &ho_EmptyRegion);
			}
			//钩底宽
			SelectObj(ho_ConnectedRegions3, &ho_ObjectSelected, hv_Index);
			Union2(ho_ObjectSelected, ho_Region0, &ho_RegionUnion);
			RegionFeatures(ho_RegionUnion, "rect2_len2", &hv_len2Value);
			hv_len2Value = ((hv_len2Value.TupleRound()) * 2)*0.011736;
			hv_tuple1 = hv_len2Value.TupleConcat(hv_tuple1);
			if (0 != (HTuple(hv_len2Value<((hv_goukuanMin - hv_errClear) + 0.12)).TupleOr(hv_len2Value>((hv_goukuanMax + hv_errClear) + 0.25))))
			{
				ConcatObj(ho_EmptyRegion, ho_ObjectSelected, &ho_EmptyRegion);
			}
		}
	}

	Union1(ho_EmptyRegion, &(*ho_region2));
	RegionFeatures((*ho_region2), "area", &hv_errorRgnArea);
	if (0 != (HTuple(hv_errorRgnArea>0).TupleAnd((*hv_ret2) == 0)))
	{
		(*hv_ret2) = -10;
	}

	 
		SetColor(hv_hwnd, "red");
	 
		DispObj((*ho_region2), hv_hwnd);
	return;
}

/*
input_Scale_1  钩外径像素比例调节（1为不变）
input_Scale_2  钩居中程度调节  （0-1（比例））越大检测越精确，不良率也随之变大
*/
void DetectModule::detectTop(HalconCpp::HObject& input_Image, const HTuple& hv_WindowHandle,
	const HTuple& input_Param_BaoluoDiameterUp, const HTuple& input_Param_BaoluoDiameterDown,
	const HTuple& input_Param_GouWidthUp, const HTuple& input_Param_GouWidthDown,
	const HTuple& input_Scale_1, const HTuple& input_Scale_2,
	const HTuple& input_Scale_3, const HTuple& input_Scale_4,
	HTuple* hv_Result, HTuple* output_ExceptionInformtion)
{
	// 面阵图片检测
	HString errorInformation("");
	HTuple hv_hwnd = hv_WindowHandle;
	HObject ho_imageCorrected, ho_imageCorrected1;

	try
	{
		SetColor(hv_hwnd, "green");
		SetDraw(hv_hwnd, "margin");
		SetLineWidth(hv_hwnd, 1);
		set_display_font(hv_hwnd, 11, "mono", "false", "false");

		funCorrectImage(input_Image, &ho_imageCorrected, &ho_imageCorrected1);

		DispObj(ho_imageCorrected, hv_hwnd);


		disp_message(hv_hwnd,
			((((((((((((((("argv:" + input_Scale_1) + " ") + input_Scale_2) + " ") + input_Scale_3) + " ") + input_Scale_4) + " ") + input_Param_BaoluoDiameterUp) + " ") + input_Param_BaoluoDiameterDown) + " ") + input_Param_GouWidthUp) + " ") + input_Param_GouWidthDown) ,
			"window", 0, 200, "black", "true");
		disp_message(hv_hwnd, (((((("粘" + fun1NgNum) + "钩宽") + fun2NgNum) + "中") + fun3NgNum) + "经") + fun4NgNum,
			"window", 20, 0, "black", "true");
		
		HObject  ho_region1, ho_region2, ho_region3, ho_region4, ho_gouTopRegions;
		HTuple  hv_ret1=0, hv_ret2 = 0, hv_ret3 = 0, hv_ret4 = 0;


		//检测槽内异物，堵槽不检
		//fun1(ho_imageCorrected1, &ho_region1, hv_hwnd, &hv_ret1);

		////钩宽，钩变形
		fun2(ho_imageCorrected1, &ho_gouTopRegions, &ho_region2, hv_hwnd, input_Param_GouWidthDown, input_Param_GouWidthUp, &hv_ret2);

		//钩宽不合格，钩间距离检测无意义，直接跳过
		if (0 != (hv_ret2 == 0))
		{
			//钩不居中、钩偏，在堵槽情况下，改由 钩间距离+侧边 来检测
			fun3(ho_imageCorrected1, ho_gouTopRegions, &ho_region3, hv_hwnd, input_Scale_2, input_Scale_3,&hv_ret3);
		}

		//包络线直径
		fun4(ho_imageCorrected1, &ho_region4, hv_hwnd, input_Scale_1, input_Param_BaoluoDiameterDown, input_Param_BaoluoDiameterUp,
			&hv_ret4);

		(*hv_Result) = 0;
		set_display_font(hv_WindowHandle, 14, "mono", "true", "false");
		//**此处第二版粘铜区分下料注意注意：同时有多种不良时最后反馈结果为粘铜
		//**2个工位检测有多种不良结果时，最后反应为粘铜
		if (0 != (hv_ret2<0))
		{
			if (0 != (hv_ret2>-10))
			{
				//funMatchDifference (Image, WindowHandle)
			}
			(*hv_Result) = 2;
			disp_message(hv_hwnd, "NG 钩宽度", "window", 60, 10, "red", "true");
			fun2NgNum= fun2NgNum+1;
		}
		if (0 != (hv_ret3<0))
		{
			if (0 != (HTuple(hv_ret3>-10).TupleAnd(hv_ret3 != -3)))
			{
				//funMatchDifference (Image, WindowHandle)
			}
			(*hv_Result) = 2;
			disp_message(hv_hwnd, "NG 钩偏钩扭", "window", 90, 10, "red", "true");
			fun3NgNum = fun3NgNum + 1;
		}
		if (0 != (hv_ret4<0))
		{
			if (0 != (HTuple(hv_ret4>-10).TupleAnd(hv_ret4 != -4)))
			{
				//funMatchDifference (Image, WindowHandle)
			}
			(*hv_Result) = 2;
			disp_message(hv_hwnd, "NG 钩长度", "window", 120, 10, "red", "true");
			fun4NgNum = fun4NgNum + 1;
		}
		if (0 != (hv_ret1<0))
		{
			if (0 != (hv_ret1>-10))
			{
				//funMatchDifference (Image, WindowHandle)
			}
			(*hv_Result) = 4;
			disp_message(hv_hwnd, "NG 槽内异物", "window", 30, 10, "red", "true");
			fun1NgNum = fun1NgNum + 1;
		}
		if (0 != ((((hv_ret1 + hv_ret2) + hv_ret3) + hv_ret4) == 0))
		{
			set_display_font(hv_hwnd, 20, "mono", "false", "false");
			disp_message(hv_hwnd, "OK-良品", "window", 40, 30, "blue", "true");
		}
		set_display_font(hv_hwnd, 11, "mono", "false", "false");
		*output_ExceptionInformtion = errorInformation;
	}
	catch (const HException& e)
	{
		*output_ExceptionInformtion = e.ErrorMessage();
		HTuple errorMsg;
		e.ToHTuple(&errorMsg);
		disp_message(hv_hwnd, errorMsg, "window", 60, 30, "red","true");
		//异常情况下返回 bad
		(*hv_Result) = 1;
	}
	//等于0时只出良品
	if (input_Scale_4 == 0)
	{
		(*hv_Result) = 0;
	}
}





////**************************************************************************************************
//
//side
//
////**************************************************************************************************
//用于记录各个检测函数返回不良的累积数量
HTuple Sidefun1NgNum = 0, Sidefun2NgNum = 0, Sidefun3NgNum = 0, Sidefun4NgNum = 0, Sidefun5NgNum = 0;

void SideCHH_PingJie(HObject ho_Image, HObject *ho_TileImage, HTuple hv_MearsureColCenter,
	HTuple hv_EdgeOffset, HTuple hv_Offset, HTuple hv_HorProjectionGrayVal, HTuple *hv_DownRow)
{

	// Local iconic variables
	HObject  ho_Rectangle, ho_ImagePartdown, ho_ImagePartup;
	HObject  ho_Images;

	// Local control variables
	HTuple  hv_Width, hv_Height, hv_TmpCtrl_Row, hv_TmpCtrl_Phi;
	HTuple  hv_TmpCtrl_Len1, hv_TmpCtrl_Len2, hv_HorProjection;
	HTuple  hv_VertProjection, hv_Index, hv_CutRow;


	GetImageSize(ho_Image, &hv_Width, &hv_Height);

	hv_TmpCtrl_Row = 800;
	//MearsureColCenter := 900
	hv_TmpCtrl_Phi = HTuple(0).TupleRad();
	hv_TmpCtrl_Len1 = 500;
	hv_TmpCtrl_Len2 = 800;
	//******
	//gen_rectangle2 (Rectangle, TmpCtrl_Row, MearsureColCenter, TmpCtrl_Phi, TmpCtrl_Len1, TmpCtrl_Len2)
	GenRectangle1(&ho_Rectangle, 0, 793.5, 1605.5, 1420);
	GrayProjections(ho_Rectangle, ho_Image, "simple", &hv_HorProjection, &hv_VertProjection);

	//***************
	//HorProjectionGrayVal 传入的参数
	{
		HTuple end_val15 = hv_HorProjection.TupleLength();
		HTuple step_val15 = 1;
		for (hv_Index = 0; hv_Index.Continue(end_val15, step_val15); hv_Index += step_val15)
		{
			if (0 != (HTuple(hv_HorProjection[hv_Index])<hv_HorProjectionGrayVal))
			{
				hv_Index += hv_EdgeOffset;
				if (0 != (hv_Index>((hv_HorProjection.TupleLength()) - 1)))
				{
					return;
				}
				if (0 != (HTuple(hv_HorProjection[hv_Index])<hv_HorProjectionGrayVal))
				{
					break;
				}
			}
			if (0 != (hv_Index == ((hv_HorProjection.TupleLength()) - 1)))
			{
				break;
			}
		}
	}

	hv_CutRow = hv_Index;
	if (0 != (hv_CutRow <= 100))
	{
		hv_CutRow = (hv_CutRow + 30) + 125;
	}
	else if (0 != (HTuple(hv_CutRow>100).TupleAnd(hv_CutRow<240)))
	{
		hv_CutRow = hv_CutRow - 100;
	}
	else
	{
		hv_CutRow = hv_CutRow - 180;
	}
	CropRectangle1(ho_Image, &ho_ImagePartdown, hv_CutRow, 0, hv_Height - 1, hv_Width - 1);
	CropRectangle1(ho_Image, &ho_ImagePartup, 0, 0, (hv_CutRow - 1) + hv_Offset, hv_Width - 1);
	GenEmptyObj(&ho_Images);
	ConcatObj(ho_Images, ho_ImagePartdown, &ho_Images);
	ConcatObj(ho_Images, ho_ImagePartup, &ho_Images);
	(*hv_DownRow) = (hv_Height - hv_CutRow) - hv_Offset;
	TileImagesOffset(ho_Images, &(*ho_TileImage), HTuple(0).TupleConcat((*hv_DownRow)),
		(HTuple(0).Append(0)), (HTuple(-1).Append(-1)), (HTuple(-1).Append(-1)), (HTuple(-1).Append(-1)),
		(HTuple(-1).Append(-1)), hv_Width, hv_Height);



	return;
}

void Sidefun1(HObject ho_Image, HObject *ho_region1, HObject *ho_outRegion1, HTuple hv_hwnd,
	HTuple hv_CaoWidthMin, HTuple hv_CaoWidthMax, HTuple *hv_ret1)
{

	// Local iconic variables
	HObject  ho_EmptyRegion, ho_outEmptyRegion, ho_ROI_0;
	HObject  ho_ImageReduced1, ho_Regions1, ho_RegionClosing2;
	HObject  ho_RegionOpening3, ho_RectangleAllPian, ho_ImageReduced;
	HObject  ho_Regions, ho_RegionClosing1, ho_RegionOpening;
	HObject  ho_RegionFillUp, ho_RegionClosing, ho_RegionOpening1;
	HObject  ho_ConnectedRegions, ho_SelectedRegions, ho_ObjectSelected;
	HObject  ho_Contours, ho_Rectangle1, ho_RegionDilation, ho_RegionDifference;
	HObject  ho_ImageReduced2, ho_Regions2, ho_RegionOpening2;
	HObject  ho_RegionErosion, ho_RegionDifference1, ho_ImageReduced3;
	HObject  ho_Regions3, ho_RegionOpening4;

	// Local control variables
	HTuple  hv_chipHeight, hv_chipWidth0, hv_Row;
	HTuple  hv_Column, hv_Width, hv_Height, hv_Width1, hv_Height1;
	HTuple  hv_w, hv_h, hv_PianColumn2, hv_Number, hv_caoHeight;
	HTuple  hv_caoHeightsTuple, hv_Index, hv_Area, hv_Row1;
	HTuple  hv_Column1, hv_Phi, hv_Length1, hv_Length2, hv_PointOrder;
	HTuple  hv_len1Value0, hv_addoffset, hv_len1Value, hv_Value;
	HTuple  hv_Value1, hv_len1Valuesz, hv_errorRgnArea;



	(*hv_ret1) = 0;
	//单片高度,用于判断槽的连续性
	hv_chipHeight = 240;
	hv_chipWidth0 = 950;
	 
		SetColor(hv_hwnd, "green");

	GetWindowExtents(hv_hwnd, &hv_Row, &hv_Column, &hv_Width, &hv_Height);
	GetImageSize(ho_Image, &hv_Width1, &hv_Height1);
	TupleReal(hv_Width, &hv_Width);
	TupleReal(hv_Height, &hv_Height);
	hv_w = hv_Width1 / hv_Width;
	hv_h = hv_Height1 / hv_Height;

	GenEmptyRegion(&ho_EmptyRegion);
	GenEmptyRegion(&ho_outEmptyRegion);

	GenRectangle1(&ho_ROI_0, 21.5, 21.5, 2277.5, 2009.5);
	ReduceDomain(ho_Image, ho_ROI_0, &ho_ImageReduced1);
	Threshold(ho_ImageReduced1, &ho_Regions1, 137, 255);
	ClosingRectangle1(ho_Regions1, &ho_RegionClosing2, 20, 150);
	OpeningRectangle1(ho_RegionClosing2, &ho_RegionOpening3, 500, 500);
	RegionFeatures(ho_RegionOpening3, "column2", &hv_PianColumn2);
	if (0 != (HTuple(0 == hv_PianColumn2).TupleOr(hv_PianColumn2>1900)))
	{
		disp_message(hv_hwnd, "sidefun1 初步提取片区column1 error", "window", 900 / hv_h,
			30, "red", "false");
		(*hv_ret1) = -1;
		return;
	}

	GenRectangle1(&ho_RectangleAllPian, 0, hv_PianColumn2 - 925, hv_Height1 - hv_chipHeight,
		hv_PianColumn2);
	ReduceDomain(ho_Image, ho_RectangleAllPian, &ho_ImageReduced);
	Threshold(ho_ImageReduced, &ho_Regions, 0, 150);
	ClosingCircle(ho_Regions, &ho_RegionClosing1, 3.5);
	OpeningRectangle1(ho_RegionClosing1, &ho_RegionOpening, 50, 10);
	FillUp(ho_RegionOpening, &ho_RegionFillUp);
	ClosingRectangle1(ho_RegionOpening, &ho_RegionClosing, 50, 10);
	OpeningRectangle1(ho_RegionClosing, &ho_RegionOpening1, 500, 10);
	Connection(ho_RegionOpening1, &ho_ConnectedRegions);

	SelectShape(ho_ConnectedRegions, &ho_SelectedRegions, "area", "and", 40093.5, 99345.8);
	CountObj(ho_SelectedRegions, &hv_Number);
	if (0 != (hv_Number<23))
	{
		disp_message(hv_hwnd, "sidefun1 提取槽区域数量 <23", "window", 900 / hv_h, 30,
			"red", "false");
		(*hv_ret1) = -2;
		return;
	}

	hv_caoHeight = 0;
	hv_caoHeightsTuple = 0;
	for (hv_Index = 1; hv_Index <= 23; hv_Index += 1)
	{
		SelectObj(ho_SelectedRegions, &ho_ObjectSelected, hv_Index);
		AreaCenter(ho_ObjectSelected, &hv_Area, &hv_Row, &hv_Column);
		if (0 != (hv_Index != 1))
		{
			if (0 != (HTuple((hv_Row - hv_caoHeight)>(hv_chipHeight*1.5)).TupleOr((hv_Row - hv_caoHeight)<(hv_chipHeight*0.7))))
			{
				disp_message(hv_hwnd, "sidefun1 槽不连续", "window", 900 / hv_h, 30, "red",
					"true");
				(*hv_ret1) = -3;
				return;
			}
		}
		hv_caoHeight = hv_Row;
		GenContourRegionXld(ho_ObjectSelected, &ho_Contours, "border");
		FitRectangle2ContourXld(ho_Contours, "regression", -1, 0, 0, 3, 2, &hv_Row1,
			&hv_Column1, &hv_Phi, &hv_Length1, &hv_Length2, &hv_PointOrder);
		GenRectangle2(&ho_Rectangle1, hv_Row1, hv_Column1, hv_Phi, hv_Length1, hv_Length2);
		//传出拟合的槽区供下一函数使用
		ConcatObj(ho_outEmptyRegion, ho_Rectangle1, &ho_outEmptyRegion);
		if (0 != (hv_Index == 1))
		{
			continue;
		}
		RegionFeatures(ho_Rectangle1, "rect2_len2", &hv_len1Value0);
		hv_caoHeightsTuple = hv_caoHeightsTuple.TupleConcat(hv_len1Value0);
		//对宽度缩放
		hv_addoffset = 12;
		hv_len1Value = ((hv_len1Value0 - hv_addoffset) * 2)*0.01046;
		//宽度效准
		if (0 != (hv_len1Value<hv_CaoWidthMin))
		{
			DilationRectangle1(ho_Rectangle1, &ho_RegionDilation, 0.5, 7);
			Difference(ho_RegionDilation, ho_Rectangle1, &ho_RegionDifference);
			ReduceDomain(ho_Image, ho_RegionDifference, &ho_ImageReduced2);
			Threshold(ho_ImageReduced2, &ho_Regions2, 0, 79);
			OpeningCircle(ho_Regions2, &ho_RegionOpening2, 1);
			RegionFeatures(ho_RegionOpening2, "area", &hv_Value);
			if (0 != (hv_Value>100))
			{
				hv_len1Value = hv_CaoWidthMin;
			}
		}
		else if (0 != (hv_len1Value>hv_CaoWidthMax))
		{
			ErosionRectangle1(ho_Rectangle1, &ho_RegionErosion, 0.5, 8);
			Difference(ho_Rectangle1, ho_RegionErosion, &ho_RegionDifference1);
			ReduceDomain(ho_Image, ho_RegionDifference1, &ho_ImageReduced3);
			Threshold(ho_ImageReduced3, &ho_Regions3, 125, 255);
			OpeningCircle(ho_Regions3, &ho_RegionOpening4, 1);
			RegionFeatures(ho_RegionOpening4, "area", &hv_Value1);
			if (0 != (hv_Value1>100))
			{
				hv_len1Value = hv_CaoWidthMax;
			}
		}

		//判断宽度
		hv_len1Valuesz = hv_len1Value.TupleString(".2");
		if (0 != (HTuple(hv_len1Value<hv_CaoWidthMin).TupleOr(hv_len1Value>hv_CaoWidthMax)))
		{
			//在槽宽大时，当上下边缘有白色亮点时可能是误判
			disp_message(hv_hwnd, (((hv_Index - 1) + " 槽宽：") + hv_len1Valuesz) + "mm", "window",
				(hv_Row + 60) / hv_h, hv_Column / hv_w, "red", "true");
			ConcatObj(ho_EmptyRegion, ho_Rectangle1, &ho_EmptyRegion);
		}
		else if (0 != 1)
		{
			disp_message(hv_hwnd, (((hv_Index - 1) + " 槽宽：") + hv_len1Valuesz) + "mm", "window",
				(hv_Row + 60) / hv_h, hv_Column / hv_w, "blue", "true");
			 
				DispObj(ho_Rectangle1, hv_hwnd);
		}
	}

	Union1(ho_EmptyRegion, &(*ho_region1));
	Union1(ho_outEmptyRegion, &(*ho_outRegion1));
	RegionFeatures((*ho_region1), "area", &hv_errorRgnArea);
	if (0 != (HTuple(hv_errorRgnArea>0).TupleAnd((*hv_ret1) == 0)))
	{
		(*hv_ret1) = -10;
	}

	 
		SetColor(hv_hwnd, "red");
	 
		DispObj((*ho_region1), hv_hwnd);

	return;
}

void Sidefun2(HObject ho_Image, HObject ho_inRegion2, HObject *ho_region2, HTuple hv_hwnd,
	HTuple hv_MaociWidth, HTuple hv_MaociHeight, HTuple *hv_ret2)
{

	// Local iconic variables
	HObject  ho_EmptyRegion, ho_ConnectedRegions;
	HObject  ho_RegionTrans, ho_ObjectSelected, ho_ImageReduced;
	HObject  ho_Regions, ho_RegionClosing, ho_RegionOpening;
	HObject  ho_ConnectedRegions1, ho_SelectedRegions, ho_RegionUnion;
	HObject  ho_ObjectSelected2, ho_RegionDilation, ho_RegionDifference1;
	HObject  ho_ImageReduced1, ho_ImageReduced2, ho_Regions1;
	HObject  ho_RegionErosion, ho_RegionMoved, ho_RegionDifference2;
	HObject  ho_ConnectedRegions3, ho_SelectedRegionsRight, ho_SelectedRegionsLeft;
	HObject  ho_ImageReduced3, ho_Regions2, ho_RegionClosing1;
	HObject  ho_ConnectedRegions2, ho_ImageReduced4, ho_Regions3;

	// Local control variables
	HTuple  hv_heightThreshold, hv_Row, hv_Column;
	HTuple  hv_Width, hv_Height, hv_Width1, hv_Height1, hv_w;
	HTuple  hv_h, hv_Value5, hv_Index, hv_ObjectSelectedcolumn1;
	HTuple  hv_Value2, hv_xxxxx, hv_Number1, hv_Index2, hv_Value;
	HTuple  hv_removeThresholdNum, hv_Min, hv_Max, hv_Range;
	HTuple  hv_Value1, hv_Mean, hv_Deviation, hv_Value4, hv_Max1;
	HTuple  hv_Value3, hv_errorRgnArea;


	(*hv_ret2) = 0;
	 
		SetColor(hv_hwnd, "green");
	hv_heightThreshold = (hv_MaociHeight / 0.00833) - 10;

	GetWindowExtents(hv_hwnd, &hv_Row, &hv_Column, &hv_Width, &hv_Height);
	GetImageSize(ho_Image, &hv_Width1, &hv_Height1);
	TupleReal(hv_Width, &hv_Width);
	TupleReal(hv_Height, &hv_Height);
	hv_w = hv_Width1 / hv_Width;
	hv_h = hv_Height1 / hv_Height;

	GenEmptyRegion(&ho_EmptyRegion);

	if (0 != (99<11))
	{
		disp_message(hv_hwnd, "sidefun2 提取槽区域数量 <11", "window", 1000 / hv_h, 30,
			"red", "true");
		(*hv_ret2) = -1;
		return;
	}

	RegionFeatures(ho_inRegion2, "column2", &hv_Value5);
	Connection(ho_inRegion2, &ho_ConnectedRegions);
	ShapeTrans(ho_ConnectedRegions, &ho_RegionTrans, "rectangle2");
	for (hv_Index = 1; hv_Index <= 22; hv_Index += 1)
	{
		SelectObj(ho_RegionTrans, &ho_ObjectSelected, hv_Index);
		RegionFeatures(ho_ObjectSelected, "column1", &hv_ObjectSelectedcolumn1);
		RegionFeatures(ho_ObjectSelected, "rect2_len2", &hv_Value2);
		hv_xxxxx = 15;
		ErosionRectangle1(ho_ObjectSelected, &ho_ObjectSelected, 1, hv_xxxxx);
		ReduceDomain(ho_Image, ho_ObjectSelected, &ho_ImageReduced);

		Threshold(ho_ImageReduced, &ho_Regions, 180, 255);
		ClosingRectangle1(ho_Regions, &ho_RegionClosing, 10, 10);
		OpeningRectangle1(ho_RegionClosing, &ho_RegionOpening, 3, 3);
		Connection(ho_RegionOpening, &ho_ConnectedRegions1);
		//判断
		SelectShape(ho_ConnectedRegions1, &ho_SelectedRegions, (HTuple("width").Append("height")),
			"and", HTuple(3).TupleConcat(hv_heightThreshold), (HTuple(2600).Append(100)));
		CountObj(ho_SelectedRegions, &hv_Number1);
		Union1(ho_SelectedRegions, &ho_RegionUnion);
		//多块区域时，判断是否几块区域临近，临近合并
		if (0 != (hv_Number1>1))
		{
			ClosingCircle(ho_RegionUnion, &ho_RegionUnion, 10);
			Connection(ho_RegionUnion, &ho_SelectedRegions);
			CountObj(ho_SelectedRegions, &hv_Number1);
		}
		//判断每一块是否为粘铜
		{
			HTuple end_val46 = hv_Number1;
			HTuple step_val46 = 1;
			for (hv_Index2 = 1; hv_Index2.Continue(end_val46, step_val46); hv_Index2 += step_val46)
			{
				SelectObj(ho_SelectedRegions, &ho_ObjectSelected2, hv_Index2);
				RegionFeatures(ho_ObjectSelected2, (((HTuple("circularity").Append("height")).Append("width")).Append("area")),
					&hv_Value);

				if (0 != (HTuple(HTuple(hv_Value[3])>400).TupleOr(HTuple(hv_Value[1])>40)))
				{
					hv_removeThresholdNum = 0;
					//排除中间浅色小圆点干扰
					//通过高度确认排除阀值
					if (0 != (HTuple(hv_Value[0])>0.05))
					{
						if (0 != (HTuple(hv_Value[1]) >= 40))
						{
							hv_removeThresholdNum = 180;
						}
						else if (0 != (HTuple(hv_Value[1]) >= 30))
						{
							hv_removeThresholdNum = 190;
						}
						else if (0 != (HTuple(hv_Value[1]) >= 20))
						{
							hv_removeThresholdNum = 200;
						}
						else if (0 != (HTuple(hv_Value[1]) >= 10))
						{
							hv_removeThresholdNum = 220;
						}
						else
						{
							hv_removeThresholdNum = 250;
						}
					}
					//判断是否为与片连接的小区域，是阀值缩小
					if (0 != (HTuple(HTuple(hv_Value[1])<22).TupleAnd(HTuple(hv_Value[3])<300)))
					{
						DilationCircle(ho_ObjectSelected2, &ho_RegionDilation, 2);
						Difference(ho_RegionDilation, ho_ObjectSelected2, &ho_RegionDifference1
						);
						ReduceDomain(ho_ImageReduced, ho_RegionDifference1, &ho_ImageReduced1);
						MinMaxGray(ho_RegionDifference1, ho_ImageReduced1, 0, &hv_Min, &hv_Max,
							&hv_Range);
						if (0 != (hv_Max>230))
						{
							hv_removeThresholdNum = 150;
						}
					}

					ReduceDomain(ho_ImageReduced, ho_ObjectSelected2, &ho_ImageReduced2);
					Threshold(ho_ImageReduced2, &ho_Regions1, hv_removeThresholdNum, 255);
					RegionFeatures(ho_Regions1, "area", &hv_Value1);
					if (0 != (hv_Value1<200))
					{
						continue;
					}
					ConcatObj(ho_EmptyRegion, ho_ObjectSelected2, &ho_EmptyRegion);
				}
			}
		}

		//胜克电木粉提取
		ErosionRectangle1(ho_ObjectSelected, &ho_ObjectSelected, 30, 0.5);
		ErosionRectangle1(ho_ObjectSelected, &ho_RegionErosion, 350, 0.5);
		MoveRegion(ho_RegionErosion, &ho_RegionMoved, 0, -40);
		Difference(ho_ObjectSelected, ho_RegionMoved, &ho_RegionDifference2);
		OpeningCircle(ho_RegionDifference2, &ho_RegionDifference2, 3.5);
		Connection(ho_RegionDifference2, &ho_ConnectedRegions3);
		SelectShapeStd(ho_ConnectedRegions3, &ho_SelectedRegionsRight, "max_area", 70);
		SelectShape(ho_ConnectedRegions3, &ho_SelectedRegionsLeft, "area", "and", 7027.52,
			10844);
		//中间段
		ReduceDomain(ho_ImageReduced, ho_RegionMoved, &ho_ImageReduced3);
		Intensity(ho_RegionMoved, ho_ImageReduced3, &hv_Mean, &hv_Deviation);
		Threshold(ho_ImageReduced3, &ho_Regions2, 0, hv_Mean - 20);
		ClosingRectangle1(ho_Regions2, &ho_RegionClosing1, 5, 10);
		Connection(ho_RegionClosing1, &ho_ConnectedRegions2);
		RegionFeatures(ho_ConnectedRegions2, "height", &hv_Value4);
		TupleMax(hv_Value4, &hv_Max1);
		if (0 != (hv_Max1>50))
		{
			ConcatObj(ho_EmptyRegion, ho_ObjectSelected, &ho_EmptyRegion);
		}
		//槽底部段
		ReduceDomain(ho_ImageReduced, ho_SelectedRegionsRight, &ho_ImageReduced3);
		Intensity(ho_SelectedRegionsRight, ho_ImageReduced3, &hv_Mean, &hv_Deviation);
		Threshold(ho_ImageReduced3, &ho_Regions2, 0, hv_Mean - 20);
		ClosingRectangle1(ho_Regions2, &ho_RegionClosing1, 5, 10);
		Connection(ho_RegionClosing1, &ho_ConnectedRegions2);
		RegionFeatures(ho_ConnectedRegions2, "height", &hv_Value4);
		TupleMax(hv_Value4, &hv_Max1);
		if (0 != (hv_Max1>50))
		{
			ConcatObj(ho_EmptyRegion, ho_ObjectSelected, &ho_EmptyRegion);
		}
		//斜坡处正常情况下为黑色，有异物时灰度增高
		ReduceDomain(ho_Image, ho_SelectedRegionsLeft, &ho_ImageReduced4);
		Threshold(ho_ImageReduced4, &ho_Regions3, 31, 255);
		RegionFeatures(ho_Regions3, "area", &hv_Value3);
		if (0 != (hv_Value3>3500))
		{
			ConcatObj(ho_EmptyRegion, ho_SelectedRegionsLeft, &ho_EmptyRegion);
		}

	}

	Union1(ho_EmptyRegion, &(*ho_region2));
	RegionFeatures((*ho_region2), "area", &hv_errorRgnArea);
	if (0 != (HTuple(hv_errorRgnArea>0).TupleAnd((*hv_ret2) == 0)))
	{
		(*hv_ret2) = -10;
		 
			SetColor(hv_hwnd, "red");
		//dilation_rectangle1 (region2, RegionDilation1, 20, 20)
		 
			DispObj((*ho_region2), hv_hwnd);
	}
	return;

}

void Sidefun3(HObject ho_Image, HObject ho_inRegion3, HObject *ho_region3, HTuple hv_hwnd,
	HTuple hv_blackArea, HTuple *hv_ret3)
{

	// Local iconic variables
	HObject  ho_EmptyRegion, ho_ConnectedRegions;
	HObject  ho_ObjectSelected, ho_ObjectSelected1, ho_RegionUnion;
	HObject  ho_RegionTrans, ho_RegionDifference, ho_RegionOpening;
	HObject  ho_ImageReduced2, ho_Regions3, ho_RegionOpening1;
	HObject  ho_RegionTrans1, ho_RegionErosion, ho_RegionMoved;
	HObject  ho_ImageReduced, ho_Regions, ho_Regions2, ho_ConnectedRegions1;
	HObject  ho_SelectedRegions, ho_ImageReduced1, ho_Regions1;
	HObject  ho_ConnectedRegions2, ho_SelectedRegions1;

	// Local control variables
	HTuple  hv_Row, hv_Column, hv_Width, hv_Height;
	HTuple  hv_Width1, hv_Height1, hv_w, hv_h, hv_Index, hv_Value;
	HTuple  hv_Maxarea, hv_Maxwidth, hv_Value1, hv_Value2, hv_Value3;
	HTuple  hv_errorRgnArea;


	(*hv_ret3) = 0;
	 
		SetColor(hv_hwnd, "green");

	GetWindowExtents(hv_hwnd, &hv_Row, &hv_Column, &hv_Width, &hv_Height);
	GetImageSize(ho_Image, &hv_Width1, &hv_Height1);
	TupleReal(hv_Width, &hv_Width);
	TupleReal(hv_Height, &hv_Height);
	hv_w = hv_Width1 / hv_Width;
	hv_h = hv_Height1 / hv_Height;

	GenEmptyRegion(&ho_EmptyRegion);

	if (0 != (99<10))
	{
		disp_message(hv_hwnd, "sidefun3 提取槽区域数量 <10", "window", 1100 / hv_h, 30,
			"red", "true");
		(*hv_ret3) = -1;
		return;
	}

	Connection(ho_inRegion3, &ho_ConnectedRegions);
	for (hv_Index = 1; hv_Index <= 22; hv_Index += 1)
	{
		SelectObj(ho_ConnectedRegions, &ho_ObjectSelected, hv_Index);
		SelectObj(ho_ConnectedRegions, &ho_ObjectSelected1, hv_Index + 1);
		Union2(ho_ObjectSelected, ho_ObjectSelected1, &ho_RegionUnion);
		ShapeTrans(ho_RegionUnion, &ho_RegionTrans, "convex");
		Difference(ho_RegionTrans, ho_RegionUnion, &ho_RegionDifference);
		//
		OpeningCircle(ho_RegionDifference, &ho_RegionOpening, 40);
		ReduceDomain(ho_Image, ho_RegionOpening, &ho_ImageReduced2);
		Threshold(ho_ImageReduced2, &ho_Regions3, 110, 255);
		OpeningCircle(ho_Regions3, &ho_RegionOpening1, 3.5);
		ShapeTrans(ho_RegionOpening1, &ho_RegionTrans1, "rectangle2");

		ErosionCircle(ho_RegionTrans1, &ho_RegionErosion, 6);
		MoveRegion(ho_RegionErosion, &ho_RegionMoved, 3, 0);
		ReduceDomain(ho_Image, ho_RegionMoved, &ho_ImageReduced);
		Threshold(ho_ImageReduced, &ho_Regions, 0, 161);
		RegionFeatures(ho_Regions, "area", &hv_Value);


		Threshold(ho_ImageReduced, &ho_Regions2, 0, 85);
		Connection(ho_Regions2, &ho_ConnectedRegions1);
		SelectShapeStd(ho_ConnectedRegions1, &ho_SelectedRegions, "max_area", 70);
		RegionFeatures(ho_SelectedRegions, "area", &hv_Maxarea);
		RegionFeatures(ho_SelectedRegions, "width", &hv_Maxwidth);
		RegionFeatures(ho_SelectedRegions, (HTuple("row").Append("column")), &hv_Value1);
		RegionFeatures(ho_RegionErosion, ((HTuple("row1").Append("row2")).Append("column2")),
			&hv_Value2);


		if (0 != (HTuple(hv_Value>hv_blackArea).TupleOr(hv_Maxarea>(hv_blackArea / 10))))
		{
			//消除上下边缘黑区造成的误判
			if (0 != (HTuple(hv_Maxarea<700).TupleAnd(hv_Maxwidth>80)))
			{
				if (0 != (HTuple(HTuple(hv_Value1[0])<(HTuple(hv_Value2[0]) + 10)).TupleOr(HTuple(hv_Value1[0])>(HTuple(hv_Value2[1]) - 10))))
				{
					continue;
				}
			}
			//消除底部小碰伤造成的误判
			if (0 != (HTuple(HTuple(hv_Value1[1])>(HTuple(hv_Value2[2]) - 20)).TupleAnd(hv_Value<(hv_blackArea + 3000))))
			{
				ReduceDomain(ho_ImageReduced, ho_SelectedRegions, &ho_ImageReduced1);
				Threshold(ho_ImageReduced1, &ho_Regions1, 0, 55);
				Connection(ho_Regions1, &ho_ConnectedRegions2);
				SelectShapeStd(ho_ConnectedRegions2, &ho_SelectedRegions1, "max_area", 70);
				RegionFeatures(ho_SelectedRegions1, (HTuple("area").Append("width")), &hv_Value3);
				if (0 != (HTuple(HTuple(hv_Value3[0])<400).TupleAnd(HTuple(hv_Value3[1])<10)))
				{
					continue;
				}
			}
			if (0 != (HTuple(hv_Maxarea<500).TupleAnd(hv_Value<(hv_blackArea + 3000))))
			{
				continue;
			}

			ConcatObj(ho_EmptyRegion, ho_Regions, &ho_EmptyRegion);
		}
	}

	Union1(ho_EmptyRegion, &(*ho_region3));
	RegionFeatures((*ho_region3), "area", &hv_errorRgnArea);
	if (0 != (HTuple(hv_errorRgnArea>0).TupleAnd((*hv_ret3) == 0)))
	{
		(*hv_ret3) = -10;
	}

	 
		SetColor(hv_hwnd, "red");
	 
		DispObj((*ho_region3), hv_hwnd);

	return;

}

void Sidefun4(HObject ho_Image, HObject ho_inRegion4, HObject *ho_region4, HTuple hv_input_Scale_1Goupian,
	HTuple hv_hwnd, HTuple *hv_ret4, HTuple *hv_ret5)
{

	// Local iconic variables
	HObject  ho_EmptyRegion, ho_RectangleSTD, ho_RegionLinesSTD;
	HObject  ho_inRegion4Moved, ho_inRegion4ConnectedRegions;
	HObject  ho_GouDing, ho_caoUp, ho_caoDown, ho_caoUpDownRegionUnion;
	HObject  ho_RegionClosing2, ho_RegionDifference, ho_RegionIntersectionSTD;
	HObject  ho_RegionErosion, ho_ImageReduced6, ho_Regions;
	HObject  ho_RegionOpening1, ho_RegionFillUp, ho_RegionOpening;
	HObject  ho_ConnectedRegions3, ho_SelectedRegions2, ho_RegionDilation;
	HObject  ho_ImageReduced, ho_Regions1, ho_RegionClosing;
	HObject  ho_gouDingSingle, ho_RegionUnion, ho_ConnectedRegions;
	HObject  ho_ObjectSelected;

	// Local control variables
	HTuple  hv_Row, hv_Column, hv_Width, hv_Height;
	HTuple  hv_Width1, hv_Height1, hv_w, hv_h, hv_ChipColumn2;
	HTuple  hv_gouMin, hv_gouMax, hv_gouMiddle, hv_gouPianDif;
	HTuple  hv_Index0, hv_chipValueSTD, hv_Value2, hv_Value1;
	HTuple  hv_Value, hv_xxx, hv_SingleGouDingArea, hv_SingleGouDingHeight;
	HTuple  hv_SingleGouDingRow, hv_chipSTDRows, hv_upDistance;
	HTuple  hv_downDitance, hv_distanceDif, hv_Number, hv_gouPianDifMean;
	HTuple  hv_GouDingColumn2Values, hv_GouHeightsValues, hv_Index;
	HTuple  hv_Selected, hv_errorRgnArea;


	(*hv_ret4) = 0;
	(*hv_ret5) = 0;
	 
		SetColor(hv_hwnd, "green");

	GetWindowExtents(hv_hwnd, &hv_Row, &hv_Column, &hv_Width, &hv_Height);
	GetImageSize(ho_Image, &hv_Width1, &hv_Height1);
	TupleReal(hv_Width, &hv_Width);
	TupleReal(hv_Height, &hv_Height);
	hv_w = hv_Width1 / hv_Width;
	hv_h = hv_Height1 / hv_Height;

	GenEmptyRegion(&ho_EmptyRegion);

	//2019-09-18改 由于片的上（左）边缘提取不稳定（上边缘形状不稳定），利用下边缘来作为钩区域定位参照
	RegionFeatures(ho_inRegion4, "column2", &hv_ChipColumn2);
	hv_gouMin = hv_ChipColumn2 - 1070;
	hv_gouMax = (hv_ChipColumn2 - 1050) + 80;
	GenRectangle1(&ho_RectangleSTD, 0, hv_gouMin, hv_Height1, hv_gouMax);
	hv_gouMiddle = hv_ChipColumn2 - 980;
	GenRegionLine(&ho_RegionLinesSTD, 0, hv_gouMiddle, hv_Height1, hv_gouMiddle);
	 
		DispObj(ho_RegionLinesSTD, hv_hwnd);

	MoveRegion(ho_inRegion4, &ho_inRegion4Moved, 0, -150);
	Connection(ho_inRegion4Moved, &ho_inRegion4ConnectedRegions);

	GenEmptyRegion(&ho_GouDing);
	hv_gouPianDif = HTuple();

	//钩顶区域提取
	for (hv_Index0 = 1; hv_Index0 <= 22; hv_Index0 += 1)
	{
		SelectObj(ho_inRegion4ConnectedRegions, &ho_caoUp, hv_Index0);
		SelectObj(ho_inRegion4ConnectedRegions, &ho_caoDown, hv_Index0 + 1);
		Union2(ho_caoUp, ho_caoDown, &ho_caoUpDownRegionUnion);
		ClosingRectangle1(ho_caoUpDownRegionUnion, &ho_RegionClosing2, 2, 300);
		Difference(ho_RegionClosing2, ho_caoUpDownRegionUnion, &ho_RegionDifference);
		Intersection(ho_RegionDifference, ho_RectangleSTD, &ho_RegionIntersectionSTD);
		RegionFeatures(ho_RegionIntersectionSTD, "row", &hv_chipValueSTD);

		//钩顶区域提取
		ErosionRectangle1(ho_RegionIntersectionSTD, &ho_RegionErosion, 0.5, 90);
		ReduceDomain(ho_Image, ho_RegionErosion, &ho_ImageReduced6);
		Threshold(ho_ImageReduced6, &ho_Regions, 0, 110);
		OpeningCircle(ho_Regions, &ho_RegionOpening1, 3.5);

		FillUp(ho_RegionOpening1, &ho_RegionFillUp);
		RegionFeatures(ho_RegionFillUp, "inner_width", &hv_Value2);
		OpeningCircle(ho_RegionFillUp, &ho_RegionOpening, (hv_Value2 / 2) - 1);
		Connection(ho_RegionOpening, &ho_ConnectedRegions3);
		SelectShapeStd(ho_ConnectedRegions3, &ho_SelectedRegions2, "max_area", 70);
		RegionFeatures(ho_SelectedRegions2, "height", &hv_Value1);
		if (0 != (hv_Value1>110))
		{
			OpeningRectangle1(ho_RegionOpening, &ho_RegionOpening, 30, 1);
		}
		if (0 != (hv_Value1<70))
		{
			OpeningCircle(ho_RegionFillUp, &ho_RegionOpening, 8);
		}


		Connection(ho_RegionOpening, &ho_ConnectedRegions3);
		SelectShapeStd(ho_ConnectedRegions3, &ho_SelectedRegions2, "max_area", 70);
		RegionFeatures(ho_SelectedRegions2, (HTuple("height").Append("width")), &hv_Value);
		if (0 != (HTuple(HTuple(hv_Value[0])>110).TupleOr(HTuple(hv_Value[0])<70)))
		{
			disp_message(hv_hwnd, "sidefun4 钩顶上侧宽度异常", "window", 1200 / hv_h, 30,
				"red", "true");
			(*hv_ret4) = -1;
			//concat_obj (RegionErosion, EmptyRegion, EmptyRegion)
			 
				SetColor(hv_hwnd, "red");
			 
				DispObj(ho_RegionErosion, hv_hwnd);
			return;
		}

		hv_xxx = 150;
		if (0 != (HTuple(hv_Value[1])>70))
		{
			hv_xxx = 50;
		}
		DilationRectangle1(ho_SelectedRegions2, &ho_RegionDilation, hv_xxx, 0.5);
		ReduceDomain(ho_ImageReduced6, ho_RegionDilation, &ho_ImageReduced);
		Threshold(ho_ImageReduced, &ho_Regions1, 0, 160);
		ClosingRectangle1(ho_Regions1, &ho_RegionClosing, 35, 3);
		OpeningRectangle1(ho_RegionClosing, &ho_gouDingSingle, 40, 45);


		RegionFeatures(ho_gouDingSingle, "area", &hv_SingleGouDingArea);
		if (0 != (HTuple(hv_SingleGouDingArea[0]) == 0))
		{
			disp_message(hv_hwnd, "sidefun4 钩顶区域未找到钩", "window", 1200 / hv_h, 30,
				"red", "true");
			(*hv_ret4) = -2;
			//concat_obj (RegionErosion, EmptyRegion, EmptyRegion)
			 
				SetColor(hv_hwnd, "red");
			 
				DispObj(ho_RegionErosion, hv_hwnd);
			return;
		}
		else
		{
			//一、钩顶宽度检测
			RegionFeatures(ho_gouDingSingle, "height", &hv_SingleGouDingHeight);
			if (0 != (HTuple(hv_SingleGouDingHeight>110).TupleOr(hv_SingleGouDingHeight<70)))
			{
				disp_message(hv_hwnd, "sidefun4 钩顶宽度异常", "window", 1200 / hv_h, 30, "red",
					"true");
				(*hv_ret4) = -3;
				//concat_obj (RegionErosion, EmptyRegion, EmptyRegion)
				 
					SetColor(hv_hwnd, "red");
				 
					DispObj(ho_gouDingSingle, hv_hwnd);
				return;
			}
			//提取到钩后，判断钩偏（通过判断到相邻上下槽的距离差判断）
			//二、距离差
			RegionFeatures(ho_gouDingSingle, "row", &hv_SingleGouDingRow);
			RegionFeatures(ho_RegionIntersectionSTD, (HTuple("row1").Append("row2")), &hv_chipSTDRows);
			hv_upDistance = hv_SingleGouDingRow - HTuple(hv_chipSTDRows[0]);
			hv_downDitance = HTuple(hv_chipSTDRows[1]) - hv_SingleGouDingRow;
			hv_distanceDif = hv_upDistance - hv_downDitance;
			hv_gouPianDif = hv_gouPianDif.TupleConcat(hv_distanceDif);


			//因为旋转是否居中对距离差影响巨大，容易误判，所以不能对每个钩进行钩偏判断
			//if (distanceDif> 30 or distanceDif<-30)
			//disp_message (hwnd, 'sidefun4 钩偏', 'window', (1200)/h, 30, 'red', 'true')
			//ret5 := -3
			//concat_obj (RegionErosion, EmptyRegion, EmptyRegion)
			//dev_set_color ('red')
			//dev_display (gouDingSingle)
			//dev_display (RegionIntersectionSTD)
			//return ()
			//endif

			ConcatObj(ho_GouDing, ho_gouDingSingle, &ho_GouDing);
		}
	}

	Union1(ho_GouDing, &ho_RegionUnion);
	Connection(ho_RegionUnion, &ho_ConnectedRegions);
	CountObj(ho_ConnectedRegions, &hv_Number);
	if (0 != (hv_Number != 22))
	{
		disp_message(hv_hwnd, "sidefun4 提取钩顶区域数量 !=22", "window", 1200 / hv_h,
			30, "red", "true");
		(*hv_ret4) = -4;
		return;
	}

	//钩偏判断
	//因为顶部相机已做钩间距判断(单个钩偏已排除)，剩下所有钩都偏情况。侧面理论上只需判断有一处居中，则全部钩居中
	//以上结论在槽间距离相等的条件下成立，此条件未确认
	TupleMean(hv_gouPianDif, &hv_gouPianDifMean);
	if (0 != (HTuple(hv_gouPianDifMean>hv_input_Scale_1Goupian).TupleOr(hv_gouPianDifMean<(-hv_input_Scale_1Goupian))))
	{
		(*hv_ret5) = -10;
		return;
	}

	//钩高度判断
	RegionFeatures(ho_ConnectedRegions, "column2", &hv_GouDingColumn2Values);
	TupleSub(hv_ChipColumn2, hv_GouDingColumn2Values, &hv_GouHeightsValues);

	for (hv_Index = 0; hv_Index <= 21; hv_Index += 1)
	{
		TupleSelect(hv_GouHeightsValues, hv_Index, &hv_Selected);
		//判断
		if (0 != (hv_Selected>1020))
		{
			SelectObj(ho_ConnectedRegions, &ho_ObjectSelected, hv_Index + 1);
			ConcatObj(ho_ObjectSelected, ho_EmptyRegion, &ho_EmptyRegion);
		}
		else if (0 != (hv_Selected<970))
		{
			SelectObj(ho_ConnectedRegions, &ho_ObjectSelected, hv_Index + 1);
			ConcatObj(ho_ObjectSelected, ho_EmptyRegion, &ho_EmptyRegion);
		}
	}
	Union1(ho_EmptyRegion, &(*ho_region4));
	RegionFeatures((*ho_region4), "area", &hv_errorRgnArea);
	if (0 != (HTuple(hv_errorRgnArea>0).TupleAnd((*hv_ret4) == 0)))
	{
		(*hv_ret4) = -10;
	}

	 
		SetColor(hv_hwnd, "red");
	 
		DispObj((*ho_region4), hv_hwnd);

	return;
}


void Sidefun5(HObject ho_Image, HObject ho_inRegion5, HObject *ho_region5, HTuple hv_hwnd,
	HTuple hv_shoulderValue, HTuple *hv_ret5)
{

	// Local iconic variables
	HObject  ho_EmptyRegion, ho_ConnectedRegions;
	HObject  ho_ObjectSelected, ho_ObjectSelected1, ho_RegionUnion;
	HObject  ho_RegionTrans, ho_RegionDifference, ho_RegionOpening;
	HObject  ho_ConnectedRegions2, ho_SelectedRegions, ho_RegionMoved;
	HObject  ho_RegionDifference1, ho_ConnectedRegions1, ho_SelectedRegions1;
	HObject  ho_RegionErosion, ho_RegionDifference2, ho_RegionOpening1;
	HObject  ho_shoulderRegion0, ho_ConnectedRegions3, ho_ObjectSelected2;
	HObject  ho_ImageReduced, ho_RegionTrans1, ho_RegionMoved1;
	HObject  ho_RegionDifference3, ho_RegionOpening3, ho_RegionMoved2;
	HObject  ho_RegionDilation, ho_Regions, ho_RegionOpening2;

	// Local control variables
	HTuple  hv_Row, hv_Column, hv_Width, hv_Height;
	HTuple  hv_Width1, hv_Height1, hv_w, hv_h, hv_Index, hv_Value1;
	HTuple  hv_erosionValue, hv_xxxxx, hv_Number, hv_Index1;
	HTuple  hv_Mean, hv_Deviation, hv_judgeValue, hv_Value;
	HTuple  hv_errorRgnArea;



	(*hv_ret5) = 0;
	 
		SetColor(hv_hwnd, "green");

	GetWindowExtents(hv_hwnd, &hv_Row, &hv_Column, &hv_Width, &hv_Height);
	GetImageSize(ho_Image, &hv_Width1, &hv_Height1);
	TupleReal(hv_Width, &hv_Width);
	TupleReal(hv_Height, &hv_Height);
	hv_w = hv_Width1 / hv_Width;
	hv_h = hv_Height1 / hv_Height;

	GenEmptyRegion(&ho_EmptyRegion);

	if (0 != (99<18))
	{
		disp_message(hv_hwnd, "sidefun3 提取槽区域数量 <18", "window", 1300 / hv_h, 30,
			"red", "true");
		(*hv_ret5) = -1;
		return;
	}

	Connection(ho_inRegion5, &ho_ConnectedRegions);
	for (hv_Index = 1; hv_Index <= 18; hv_Index += 1)
	{
		SelectObj(ho_ConnectedRegions, &ho_ObjectSelected, hv_Index);
		SelectObj(ho_ConnectedRegions, &ho_ObjectSelected1, hv_Index + 1);
		Union2(ho_ObjectSelected, ho_ObjectSelected1, &ho_RegionUnion);
		ShapeTrans(ho_RegionUnion, &ho_RegionTrans, "convex");
		Difference(ho_RegionTrans, ho_RegionUnion, &ho_RegionDifference);
		OpeningCircle(ho_RegionDifference, &ho_RegionOpening, 5);
		Connection(ho_RegionOpening, &ho_ConnectedRegions2);
		SelectShapeStd(ho_ConnectedRegions2, &ho_SelectedRegions, "max_area", 70);
		MoveRegion(ho_SelectedRegions, &ho_RegionMoved, 0, -50);
		Difference(ho_RegionMoved, ho_SelectedRegions, &ho_RegionDifference1);
		Connection(ho_RegionDifference1, &ho_ConnectedRegions1);
		SelectShapeStd(ho_ConnectedRegions1, &ho_SelectedRegions1, "max_area", 70);
		//片宽度不等，通过实际宽度腐蚀
		RegionFeatures(ho_SelectedRegions1, "height", &hv_Value1);
		hv_erosionValue = 50 + (hv_Value1 - 250);
		//erosion_rectangle1 (SelectedRegions1, SelectedRegions1, 0.5, 8)
		ErosionRectangle1(ho_SelectedRegions1, &ho_RegionErosion, 0.5, hv_erosionValue);
		Difference(ho_SelectedRegions1, ho_RegionErosion, &ho_RegionDifference2);
		OpeningCircle(ho_RegionDifference2, &ho_RegionOpening1, 2.5);

		hv_xxxxx = 82;
		GenEmptyRegion(&ho_shoulderRegion0);
		Connection(ho_RegionOpening1, &ho_ConnectedRegions3);
		SelectShape(ho_ConnectedRegions3, &ho_ConnectedRegions3, "area", "and", 1143.52,
			5000);
		CountObj(ho_ConnectedRegions3, &hv_Number);
		if (0 != (hv_Number != 2))
		{
			disp_message(hv_hwnd, "sidefun5 肩区数量！=2", "window", 1300 / hv_h, 30, "red",
				"true");
			(*hv_ret5) = -2;
			//stop ()
			return;
		}
		for (hv_Index1 = 1; hv_Index1 <= 2; hv_Index1 += 1)
		{
			SelectObj(ho_ConnectedRegions3, &ho_ObjectSelected2, hv_Index1);
			ReduceDomain(ho_Image, ho_ObjectSelected2, &ho_ImageReduced);
			//获取片上面熟料区灰度值
			if (0 != (hv_Index1 == 1))
			{
				ShapeTrans(ho_ObjectSelected, &ho_RegionTrans1, "rectangle2");
				MoveRegion(ho_RegionTrans1, &ho_RegionMoved1, 0, -70);
				Difference(ho_RegionMoved1, ho_RegionTrans1, &ho_RegionDifference3);
				OpeningCircle(ho_RegionDifference3, &ho_RegionOpening3, 20);
				MoveRegion(ho_RegionOpening3, &ho_RegionMoved2, 40, -100);
				DilationCircle(ho_RegionMoved2, &ho_RegionDilation, 5);
				Intensity(ho_RegionDilation, ho_Image, &hv_Mean, &hv_Deviation);
			}
			else
			{
				ShapeTrans(ho_ObjectSelected1, &ho_RegionTrans1, "rectangle2");
				MoveRegion(ho_RegionTrans1, &ho_RegionMoved1, 0, -70);
				Difference(ho_RegionMoved1, ho_RegionTrans1, &ho_RegionDifference3);
				OpeningCircle(ho_RegionDifference3, &ho_RegionOpening3, 20);
				MoveRegion(ho_RegionOpening3, &ho_RegionMoved2, -40, -100);
				DilationCircle(ho_RegionMoved2, &ho_RegionDilation, 20);
				Intensity(ho_RegionDilation, ho_Image, &hv_Mean, &hv_Deviation);
			}
			Threshold(ho_ImageReduced, &ho_Regions, 0, hv_Mean + 40);
			ClosingRectangle1(ho_Regions, &ho_Regions, 1, 25);
			//下值越大越精确
			hv_judgeValue = hv_shoulderValue - (((330 - hv_Value1) / 2) / 2);
			if (0 != (hv_judgeValue>50))
			{
				hv_judgeValue = 50;
			}
			if (0 != (hv_judgeValue <= 1))
			{
				hv_judgeValue = 1;
			}
			OpeningRectangle1(ho_Regions, &ho_RegionOpening2, 10, hv_judgeValue);
			RegionFeatures(ho_RegionOpening2, "area", &hv_Value);
			if (0 != (hv_Value<500))
			{
				//dev_display (Regions)
				//stop ()
				ConcatObj(ho_EmptyRegion, ho_Regions, &ho_EmptyRegion);
			}
		}
		//dev_clear_window ()
		//dev_display (Image)
		//dev_display (RegionOpening1)


	}

	Union1(ho_EmptyRegion, &(*ho_region5));
	RegionFeatures((*ho_region5), "area", &hv_errorRgnArea);
	if (0 != (HTuple(hv_errorRgnArea>0).TupleAnd((*hv_ret5) == 0)))
	{
		(*hv_ret5) = -10;
	}

	 
		SetColor(hv_hwnd, "red");
	 
		DispObj((*ho_region5), hv_hwnd);

	return;


}



void DetectModule::detectSide(HalconCpp::HObject& input_Image, const HTuple& hv_WindowHandle,
	const HTuple& input_Param_CaoWidthUp, const HTuple& input_Param_CaoWidthDown,
	const HTuple& input_Param_MaociWidth, const HTuple& input_Param_MaociHeight,
	const HTuple& hv_input_Scale_1Goupian, const HTuple& input_Scale_2,
	const HTuple& input_Scale_3, const HTuple& input_Scale_4,
	HTuple* hv_Result, HTuple* output_ExceptionInformtion)
{
	// 线阵图片检测
	HString errorInformation("");
	HTuple hv_hwnd = hv_WindowHandle;
	try
	{
		SetColor(hv_hwnd, "green");
		SetDraw(hv_hwnd, "margin");
		SetLineWidth(hv_hwnd, 1);
		//HObject  ho_region1, ho_outRegion1, ho_region2, ho_region3;
		//HObject  ho_TileImage;
		//HTuple  hv_DownRow,hv_ret1 = 0, hv_ret2 = 0, hv_ret3 = 0;

		// Local iconic variables
		HObject  ho_ImagePJ,ho_region1, ho_outRegion1, ho_region2, ho_region3,ho_region4, ho_region5;

		// Local control variables
		HTuple  hv_DownRow, hv_ret1, hv_ret2, hv_ret3, hv_ret4, hv_ret5;
		ClearWindow(hv_hwnd);

		hv_ret1 = 0;
		hv_ret2 = 0;
		hv_ret3 = 0;
		hv_ret4 = 0;
		hv_ret5 = 0;

		SideCHH_PingJie(input_Image, &ho_ImagePJ, 1080, 2, 0, 60, &hv_DownRow);
		DispObj(ho_ImagePJ, hv_hwnd);

		disp_message(hv_hwnd,
			((((((((((((((("argv:" + hv_input_Scale_1Goupian) + " ") + input_Scale_2) + " ") + input_Scale_3) + " ") + input_Scale_4) + " ") + input_Param_CaoWidthUp) + " ") + input_Param_CaoWidthDown) + " ") + input_Param_MaociWidth) + " ") + input_Param_MaociHeight),
			"window", 0, 200, "black", "true");
		disp_message(hv_hwnd, (((((((("槽宽" + Sidefun1NgNum) + "粘") + Sidefun2NgNum) + "碰") + Sidefun3NgNum) + "高") + Sidefun4NgNum) + "中") + Sidefun5NgNum,
			"window", 20, 200, "black", "true");

		//槽宽
		Sidefun1(ho_ImagePJ, &ho_region1, &ho_outRegion1, hv_hwnd, input_Param_CaoWidthDown,
			input_Param_CaoWidthUp, &hv_ret1);
		if (0 != (HTuple(hv_ret1 == 0).TupleOr(hv_ret1 == -10)))
		{
			//毛刺
			Sidefun2(ho_ImagePJ, ho_outRegion1, &ho_region2, hv_hwnd, input_Param_MaociWidth,
				input_Param_MaociHeight, &hv_ret2);

			//片区大划伤、生锈
			Sidefun3(ho_ImagePJ, ho_outRegion1, &ho_region3, hv_hwnd, input_Scale_2, &hv_ret3);
			
			//钩顶到换向器底部距离（钩右极限位置）  + 钩偏（返回值：钩高度不良ret4<0, 钩偏ret5<0）
			Sidefun4(ho_ImagePJ, ho_outRegion1, &ho_region4, hv_input_Scale_1Goupian,hv_hwnd, &hv_ret4, &hv_ret5);

			//对钩居中的检测（肩上方区域黑色宽度）
			//Sidefun5(ho_ImagePJ, ho_outRegion1, &ho_region5, hv_WindowHandle, input_Scale_1, &hv_ret5);
		}


		//**此处第二版粘铜区分下料注意注意：同时有多种不良时最后反馈结果为粘铜
		//**2个工位检测有多种不良结果时，最后反应为粘铜
		(*hv_Result) = 0;
		set_display_font(hv_WindowHandle, 14, "mono", "true", "false");
		if (0 != (hv_ret1<0))
		{
			(*hv_Result) = 3;
			disp_message(hv_hwnd, "NG 槽宽度", "window", 30, 30, "red", "true");
			Sidefun1NgNum = Sidefun1NgNum + 1;
		}
		if (0 != (hv_ret3<0))
		{
			(*hv_Result) = 1;
			disp_message(hv_hwnd, "NG 大碰伤划伤、生锈", "window", 90, 30, "red", "true");
			Sidefun3NgNum = Sidefun3NgNum + 1;
		}
		if (0 != (hv_ret4<0))
		{
			(*hv_Result) = 2;
			disp_message(hv_hwnd, "NG 钩高度", "window", 120, 30, "red", "true");
			Sidefun4NgNum = Sidefun4NgNum + 1;
		}
		if (0 != (hv_ret5<0))
		{
			(*hv_Result) = 2;
			disp_message(hv_hwnd, "NG 钩不居中", "window", 150, 30, "red", "true");
			Sidefun5NgNum = Sidefun5NgNum + 1;
		}
		if (0 != (hv_ret2<0))
		{
			(*hv_Result) = 4;
			disp_message(hv_hwnd, "NG 槽内异物", "window", 60, 30, "red", "true");
			Sidefun2NgNum = Sidefun2NgNum + 1;
		}
		//槽内被粘铜堵住导致槽不连续，不进行粘铜检测情况也判位粘铜
		if (0 != (HTuple(hv_ret1 == -2).TupleOr(hv_ret1 == -3)))
		{
			(*hv_Result) = 4;
		}
		if (0 != (((((hv_ret1 + hv_ret2) + hv_ret3) + hv_ret4) + hv_ret5) == 0))
		{
			set_display_font(hv_hwnd, 20, "mono", "false", "false");
			disp_message(hv_hwnd, "OK-良品", "window", 40, 30, "blue", "true");
		}
		set_display_font(hv_hwnd, 11, "mono", "false", "false");

		*output_ExceptionInformtion = errorInformation;
	}
	catch (const HException& e)
	{
		*output_ExceptionInformtion = e.ErrorMessage();
		HTuple errorMsg;
		e.ToHTuple(&errorMsg);
		disp_message(hv_hwnd, errorMsg, "window", 60, 30, "red", "true");
		//异常情况下返回 bad
		(*hv_Result) = 1;
	}
	//等于0时只出良品
	if (input_Scale_4 == 0)
	{
		(*hv_Result) = 0;
	}
}