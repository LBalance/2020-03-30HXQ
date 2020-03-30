#include "Detect.h"
#include "outfun.h"


////**************************************************************************************************
//
//top
//
////**************************************************************************************************
//���ڼ�¼������⺯�����ز������ۻ�����
HTuple fun1NgNum=0, fun2NgNum = 0, fun3NgNum = 0, fun4NgNum = 0, fun5NgNum = 0;

void coordinateFun(HTuple hv_hwnd, HTuple hv_showColor, HTuple hv_RowCoordinate,
	HTuple hv_ColumnCoordinate, HTuple hv_theTupleTobeShow)
{

	// Local iconic variables
	HObject  ho_HistoRegion1, ho_RegionMoved;

	// Local control variables
	HTuple  hv_Row1, hv_Column1, hv_Row2, hv_Column2;
	HTuple  hv_Row, hv_Column, hv_Width, hv_Height, hv_Diff;
	HTuple  hv_Value;


	GetPart(hv_hwnd, &hv_Row1, &hv_Column1, &hv_Row2, &hv_Column2);
	//�Ŵ���ʾ
	 
		SetPart(hv_hwnd, hv_Row1, hv_Column1, hv_Row2 / 5, hv_Column2 / 10);




	GetWindowExtents(hv_hwnd, &hv_Row, &hv_Column, &hv_Width, &hv_Height);

	 
		SetColor(hv_hwnd, "cyan");
	DispArrow(hv_hwnd, hv_RowCoordinate, hv_ColumnCoordinate, hv_RowCoordinate - 300,
		hv_ColumnCoordinate, 1);
	DispArrow(hv_hwnd, hv_RowCoordinate, hv_ColumnCoordinate, hv_RowCoordinate, hv_ColumnCoordinate + 300,
		2);


	disp_message(hv_hwnd, "��", "image", hv_RowCoordinate - 260, hv_ColumnCoordinate - 8,
		"cyan", "false");
	disp_message(hv_hwnd, "��", "image", hv_RowCoordinate - 240, hv_ColumnCoordinate - 8,
		"cyan", "false");
	disp_message(hv_hwnd, "��", "image", hv_RowCoordinate - 220, hv_ColumnCoordinate - 8,
		"cyan", "false");
	disp_message(hv_hwnd, "3", "image", hv_RowCoordinate - 200, hv_ColumnCoordinate - 8,
		"cyan", "false");
	disp_message(hv_hwnd, "��", "image", hv_RowCoordinate - 180, hv_ColumnCoordinate - 8,
		"cyan", "false");

	 
		SetDraw(hv_hwnd, "margin");
	 
		SetColor(hv_hwnd, hv_showColor);


	TupleSub(hv_theTupleTobeShow, 1100, &hv_Diff);

	GenRegionHisto(&ho_HistoRegion1, hv_Diff, hv_Height / 2, hv_Width / 2, 0.6);
	RegionFeatures(ho_HistoRegion1, (HTuple("row2").Append("column1")), &hv_Value);
	MoveRegion(ho_HistoRegion1, &ho_RegionMoved, hv_RowCoordinate - HTuple(hv_Value[0]),
		(hv_ColumnCoordinate - HTuple(hv_Value[1])) + 2);

	 
		DispObj(ho_RegionMoved, hv_hwnd);

	//��λ
	 
		SetPart(hv_hwnd, hv_Row1, hv_Column1, hv_Row2, hv_Column2);
	//hv_hwnd
	return;
}

void funCorrectImage(HObject ho_image, HObject *ho_imageCorrected0, HObject *ho_imageCorrected1,
	HObject *ho_imageCorrected2)
{

	// Local iconic variables
	HObject  ho_Image1, ho_Image2, ho_Image3, ho_Regions;
	HObject  ho_RegionOpening, ho_RegionClosing, ho_ConnectedRegions;
	HObject  ho_RegionFillUp, ho_SelectedRegions1, ho_SelectedRegions;
	HObject  ho_RegionUnion, ho_RegionDilation;

	// Local control variables
	HTuple  hv_Channels, hv_Number;


	//copy_image (image, imageCorrected0)
	//copy_image (image, imageCorrected1)

	//return ()
	//����ͼƬʱ���ж�������
	CountObj(ho_image, &hv_Channels);
	if (0 != (hv_Channels == 1))
	{
		SelectObj(ho_image, &ho_Image1, 1);
	}
	else if (0 != (hv_Channels == 2))
	{
		SelectObj(ho_image, &ho_Image1, 1);
		SelectObj(ho_image, &ho_Image2, 2);
	}
	else if (0 != (hv_Channels == 3))
	{
		SelectObj(ho_image, &ho_Image1, 1);
		SelectObj(ho_image, &ho_Image2, 2);
		SelectObj(ho_image, &ho_Image3, 3);
	}

	Threshold(ho_Image1, &ho_Regions, 0, 150);
	OpeningCircle(ho_Regions, &ho_RegionOpening, 3.5);

	ClosingCircle(ho_RegionOpening, &ho_RegionClosing, 3.5);
	Connection(ho_RegionClosing, &ho_ConnectedRegions);
	FillUp(ho_ConnectedRegions, &ho_RegionFillUp);
	SelectShapeStd(ho_RegionFillUp, &ho_SelectedRegions1, "max_area", 70);
	Difference(ho_RegionFillUp, ho_SelectedRegions1, &ho_SelectedRegions);
	Union1(ho_SelectedRegions, &ho_RegionUnion);
	DilationCircle(ho_RegionUnion, &ho_RegionDilation, 35);

	CountObj(ho_RegionDilation, &hv_Number);
	if (0 != (hv_Number == 0))
	{
		GenRegionPoints(&ho_RegionDilation, 100, 100);
	}

	if (0 != (hv_Channels == 1))
	{
		PaintRegion(ho_RegionDilation, ho_Image1, &(*ho_imageCorrected0), 245, "fill");
		CopyImage((*ho_imageCorrected0), &(*ho_imageCorrected1));
	}
	else if (0 != (hv_Channels == 2))
	{
		PaintRegion(ho_RegionDilation, ho_Image1, &(*ho_imageCorrected0), 245, "fill");
		PaintRegion(ho_RegionDilation, ho_Image2, &(*ho_imageCorrected1), 245, "fill");
	}
	else if (0 != (hv_Channels == 3))
	{
		PaintRegion(ho_RegionDilation, ho_Image1, &(*ho_imageCorrected0), 245, "fill");
		PaintRegion(ho_RegionDilation, ho_Image2, &(*ho_imageCorrected1), 245, "fill");
		PaintRegion(ho_RegionDilation, ho_Image3, &(*ho_imageCorrected2), 245, "fill");
	}

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
		ReadShapeModel("mode.shm", &hv_ModelID);//E:/������/halcon/
		ReadRegion(&ho_RegionSTD, "stdreg.reg");//E:/������/halcon/
	}
	AreaCenter(ho_RegionSTD, &hv_Area1, &hv_Row0, &hv_Column0);


	Threshold(ho_Image, &ho_Regions, 0, 200);
	Connection(ho_Regions, &ho_ConnectedRegions);
	SelectShapeStd(ho_ConnectedRegions, &ho_SelectedRegions, "max_area", 70);
	RegionFeatures(ho_SelectedRegions, "area", &hv_areaValue);
	if (0 != (HTuple(hv_areaValue<(3950000 - 500000)).TupleOr(hv_areaValue>(3950000 + 500000))))
	{
		disp_message(hv_hwnd, "matchDifference ѡ�������������eorror", "window", 1500,
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
		disp_message(hv_hwnd, "matchDifference ѡ������Բeorror", "window", 1500, 20,
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
		disp_message(hv_hwnd, "matchDifference ƥ����eorror", "window", 1500, 20, "red",
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


	return;
}

void fun4(HObject ho_Image, HObject *ho_region4, HTuple hv_hwnd, HTuple hv_input_Scale_1,
	HTuple hv_input_Scale_4, HTuple hv_gouchangMin, HTuple hv_gouchangMax, HTuple *hv_ret4)
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
	HTuple  hv_a205, hv_RadiusDifL, hv_RadiusDifN;


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
	//����ж�
	RegionFeatures(ho_SelectedRegions, "area", &hv_areaValue);
	hv_calculatedArea = (3.14*hv_tongPianRadRadius)*hv_tongPianRadRadius;
	if (0 != (HTuple(hv_areaValue<(hv_calculatedArea - hv_areaoffset)).TupleOr(hv_areaValue>(hv_calculatedArea + hv_areaoffset))))
	{
		disp_message(hv_hwnd, "topfun4 ѡ�������������eorror", "window", ((hv_Height1 - 400) + 300) / hv_h,
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
	//2019-07-2�� �й����淴����ɰ�ɫ ���Ϊ��ɫ
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
		disp_message(hv_hwnd, "topfun4 ��ȡ������������ ��=" + hv_gouCount, "window", ((hv_Height1 - 400) + 300) / hv_h,
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
				disp_message(hv_hwnd, "topfun4 δ�ҵ��Գ�����", "window", ((hv_Height1 - 400) + 300) / hv_h,
					20, "red", "true");
				(*hv_ret4) = -3;
			}
			ShapeTrans(ho_TwoSymmetryGous, &ho_TwoSymmetryGousConvex, "convex");
			OpeningCircle(ho_TwoSymmetryGousConvex, &ho_TwoSymmetryGousOpened, 30);
			RegionFeatures(ho_TwoSymmetryGousOpened, "max_diameter", &hv_twoOutGouDiameter);

			//����
			//intersection (TwoSymmetryGousConvex, RegionOpening1, RegionIntersection1)
			//region_features (RegionIntersection1, 'max_diameter', Value)
			//JiaoZhengValue := 994*2-Value
			hv_JiaoZhengValue = 0;

			//�뾶�������䳤��
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

			//ֱ�������ж�
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


			//�⾶��ֵ��ʾ
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

	//��ֱ�������ҿ�����С��׼ֵʱ�������������뾶�����Դ�����ʱҲ��Ϊ���ǣ����⾶С��
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
					hv_a205 = hv_input_Scale_4;
					hv_RadiusDifL = HTuple(hv_GouRadiusAll[hv_Index1]) - HTuple(hv_GouRadiusAll[hv_Index1Last]);
					hv_RadiusDifN = HTuple(hv_GouRadiusAll[hv_Index1]) - HTuple(hv_GouRadiusAll[hv_Index1Next]);
					if (0 != (HTuple(HTuple(HTuple(hv_RadiusDifL>hv_a205).TupleOr(hv_RadiusDifL<(-hv_a205))).TupleOr(hv_RadiusDifN>hv_a205)).TupleOr(hv_RadiusDifN<(-hv_a205))))
					{
						//���
						if (0 != (HTuple(HTuple(hv_twoOutGouDiameterChangedAll[hv_Index1 % 11])>(hv_gouchangMin + 0.08)).TupleAnd(HTuple(hv_twoOutGouDiameterChangedAll[hv_Index1 % 11])<(hv_gouchangMax - 0.08))))
						{
							continue;
						}
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

	//��ȡԲ�ģ��˴�����������������ͬ��ͨ����Բȷ��Բ��
	Threshold(ho_Image, &ho_Regions, 0, hv_graythreshold);
	Connection(ho_Regions, &ho_ConnectedRegions);
	SelectShapeStd(ho_ConnectedRegions, &ho_SelectedRegions, "max_area", 70);
	FillUp(ho_SelectedRegions, &ho_RegionFillUp);
	OpeningCircle(ho_RegionFillUp, &ho_RegionOpening, 800);
	AreaCenter(ho_RegionOpening, &hv_Area, &hv_Row, &hv_Column);

	//ִ�е��˴�ʱ������һ���ϸ�
	//�����ж����ŷ�ֵ
	hv_LengthJudgeNum = hv_GouTaoWide;
	//��ƫ�������棬ͨ��������루��������Ŀ��ȣ����жϻ��ǲ��У������׹����ߵ�ԭ�����
	GenCircle(&ho_CircleCheckBase, hv_Row, hv_Column, hv_tongPianRadRadius + 102);
	DilationCircle(ho_CircleCheckBase, &ho_CircleCheckBaseDilation, 110);
	Difference(ho_CircleCheckBaseDilation, ho_CircleCheckBase, &ho_CircleCheckBaseCircle
	);
	Connection(ho_gouTopRegions, &ho_gouTopConnectedRegions);

	//�������
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
	//�����ڿ׿���
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
		//�ж�
		if (0 != (HTuple(HTuple(HTuple(hv_Value[0])<200).TupleAnd(HTuple(hv_Value[1])<hv_GouGouTaoIntersectionInner_radius)).TupleAnd(HTuple(hv_Value[2])<70)))
		{
			hv_isOkMark = 1;
			if (0 != hv_isDebugMark)
			{
				disp_message(hv_hwnd, HTuple("������,�˳�ѭ��------>>>>>"), "window", ((hv_Height1 - 400) + 100) / hv_h,
					20, "blue", "true");
				// stop(...); only in hdevelop
			}

			disp_message(hv_hwnd, (((("topfun3 ���׽�Msg area/inR/D=" + HTuple(hv_Value[0])) + "/") + HTuple(hv_Value[1])) + "/") + HTuple(hv_Value[2]),
				"window", ((hv_Height1 - 400) + 100) / hv_h, 20, "blue", "true");
			break;
		}
	}

	if (0 != hv_isDebugMark)
	{
		disp_message(hv_hwnd, "�˳�ѭ��------>>>>>", "window", ((hv_Height1 - 400) + 100) / hv_h,
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
		//disp_message (hwnd, 'topfun3 ��������', 'window', (Height1-400+100)/h, 20, 'red', 'true')
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
	//hv_hwnd
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
	//����ж�
	RegionFeatures(ho_SelectedRegions, "area", &hv_areaValue);
	hv_calculatedArea = (3.14*hv_tongPianRadRadius)*hv_tongPianRadRadius;
	if (0 != (HTuple(hv_areaValue<(hv_calculatedArea - hv_areaoffset)).TupleOr(hv_areaValue>(hv_calculatedArea + hv_areaoffset))))
	{
		disp_message(hv_hwnd, "topfun2 ѡ�������������eorror", "window", ((hv_Height1 - 400) + 100) / hv_h,
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
		disp_message(hv_hwnd, "topfun2 ��ȡ������������ ��=" + hv_gouCount, "window", ((hv_Height1 - 400) + 100) / hv_h,
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
		disp_message(hv_hwnd, "topfun2 ��ȡ������������ ��=" + hv_gouCount, "window", ((hv_Height1 - 400) + 100) / hv_h,
			20, "red", "true");
		(*hv_ret2) = -3;
		return;
	}

	//������Χ�ж�
	hv_tuple0 = HTuple();
	hv_tuple1 = HTuple();

	{
		HTuple end_val70 = hv_gouCount;
		HTuple step_val70 = 1;
		for (hv_Index = 1; hv_Index.Continue(end_val70, step_val70); hv_Index += step_val70)
		{
			//������
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
			//���׿�
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
	//hv_hwnd
	return;
}

void fun5(HObject ho_imageCorrected0, HObject ho_imageCorrected1, HObject ho_imageCorrected2,
	HObject *ho_imageCorrectedSTD, HTuple hv_hwnd, HTuple hv_input_Scale_1, HTuple hv_gouchangMin,
	HTuple hv_gouchangMax, HTuple *hv_ret5)
{

	// Local iconic variables
	HObject  ho_EmptyRegion, ho_Regions, ho_ConnectedRegions;
	HObject  ho_SelectedRegions, ho_RegionFillUp, ho_ImageReduced;
	HObject  ho_Regions1, ho_ConnectedRegions1, ho_SelectedRegions2;
	HObject  ho_Circle, ho_RegionDifference, ho_Region0, ho_RegionOpening;
	HObject  ho_ConnectedRegions2, ho_CircleSTD0, ho_CircleSTD1;
	HObject  ho_gouRadiusLineRight, ho_gouRadiusLineLeft, ho_ObjectSelected1;
	HObject  ho_RegionTrans, ho_circleMudle, ho_GouOuterCircle0;
	HObject  ho_GouOuterCircle1, ho_GouOuterCircle, ho_TwoSymmetryGous;
	HObject  ho_TwoSymmetryGousConvex, ho_TwoSymmetryGousOpened;
	HObject  ho_RegionIntersection, ho_RegionLines, ho_RegionIntersectionRight;
	HObject  ho_RegionIntersectionLeft, ho_RegionErosion;

	// Local control variables
	HTuple  hv_graythreshold, hv_areathreshold, hv_diameterMax;
	HTuple  hv_diameterMin, hv_areaoffset, hv_tongPianRadRadius;
	HTuple  hv_gouCount, hv_Row, hv_Column, hv_Width, hv_Height;
	HTuple  hv_Width1, hv_Height1, hv_w, hv_h, hv_areaValue;
	HTuple  hv_calculatedArea, hv_Area, hv_Number, hv_circleTimes;
	HTuple  hv_GouRadiusRight, hv_GouRadiusLeft, hv_Index, hv_HomMat2DIdentity;
	HTuple  hv_HomMat2DRotate, hv_TwoSymmetryGousArea, hv_twoOutGouDiameter;
	HTuple  hv_Area1, hv_Row1, hv_Column1, hv_aaa, hv_bbb, hv_cccc;
	HTuple  hv_sina, hv_cosa, hv_rowOffset, hv_columnOffset;
	HTuple  hv_rowDispPoint, hv_columnDispPoint, hv_Value1;
	HTuple  hv_GouRadiusAll0, hv_GouRadiusAll1, hv_GouRadiusAll2;
	HTuple  hv_subAbsMaxAll, hv_sub0__1, hv_subAbs0__1, hv_subAbsMax0__1;
	HTuple  hv_sub0__2, hv_subAbs0__2, hv_subAbsMax0__2, hv_sub1__2;
	HTuple  hv_subAbs1__2, hv_subAbsMax1__2, hv_Max395, hv_Min396;
	HTuple  hv_a400, hv_coordinateRow, hv_coordinateColumn;
	HTuple  hv_GouRadiusAll0Show, hv_GouRadiusAll1Show, hv_GouRadiusAll2Show;
	HTuple  hv_msgColor0, hv_msgColor01, hv_msgColor02, hv_msgColor12;
	HTuple  hv_Median;




	(*hv_ret5) = 0;
	hv_graythreshold = 160;
	hv_areathreshold = 1500;
	hv_diameterMax = 2800;
	hv_diameterMin = 1365 * 2;
	hv_areaoffset = 500000;
	hv_tongPianRadRadius = 998;
	hv_gouCount = 22;



	 
		SetColor(hv_hwnd, "blue");
	GetWindowExtents(hv_hwnd, &hv_Row, &hv_Column, &hv_Width, &hv_Height);
	GetImageSize(ho_imageCorrected0, &hv_Width1, &hv_Height1);
	TupleReal(hv_Width, &hv_Width);
	TupleReal(hv_Height, &hv_Height);
	hv_w = hv_Width1 / hv_Width;
	hv_h = hv_Height1 / hv_Height;

	GenEmptyRegion(&ho_EmptyRegion);


	//��һ��ͼƬ
	Threshold(ho_imageCorrected0, &ho_Regions, 0, hv_graythreshold);
	Connection(ho_Regions, &ho_ConnectedRegions);
	SelectShapeStd(ho_ConnectedRegions, &ho_SelectedRegions, "max_area", 70);
	//����ж�
	RegionFeatures(ho_SelectedRegions, "area", &hv_areaValue);
	hv_calculatedArea = (3.14*hv_tongPianRadRadius)*hv_tongPianRadRadius;
	if (0 != (HTuple(hv_areaValue<(hv_calculatedArea - hv_areaoffset)).TupleOr(hv_areaValue>(hv_calculatedArea + hv_areaoffset))))
	{
		disp_message(hv_hwnd, "topfun5 ѡ�������������eorror", "window", ((hv_Height1 - 400) + 300) / hv_h,
			20, "red", "true");
		(*hv_ret5) = -1;
		return;
	}
	FillUp(ho_SelectedRegions, &ho_RegionFillUp);
	ReduceDomain(ho_imageCorrected0, ho_RegionFillUp, &ho_ImageReduced);
	Threshold(ho_ImageReduced, &ho_Regions1, 100, 255);
	Connection(ho_Regions1, &ho_ConnectedRegions1);
	SelectShapeStd(ho_ConnectedRegions1, &ho_SelectedRegions2, "max_area", 70);
	AreaCenter(ho_SelectedRegions2, &hv_Area, &hv_Row, &hv_Column);

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
		disp_message(hv_hwnd, "topfun5 ��ȡ������������ ��=" + hv_gouCount, "window", ((hv_Height1 - 400) + 300) / hv_h,
			20, "red", "true");
		(*hv_ret5) = -2;
		if (0 != ((hv_Number % 2) == 1))
		{
			hv_circleTimes = (hv_Number + 1) / 2;
		}
		return;
	}

	GenCircle(&ho_CircleSTD0, hv_Row, hv_Column, hv_tongPianRadRadius - 2);
	GenCircle(&ho_CircleSTD1, hv_Row, hv_Column, hv_tongPianRadRadius + 202);

	//dev_display (CircleSTD1)


	hv_GouRadiusRight = HTuple();
	hv_GouRadiusLeft = HTuple();
	GenEmptyRegion(&ho_gouRadiusLineRight);
	GenEmptyRegion(&ho_gouRadiusLineLeft);



	SelectObj(ho_ConnectedRegions2, &ho_ObjectSelected1, 1);
	ShapeTrans(ho_ObjectSelected1, &ho_RegionTrans, "outer_circle");
	DilationCircle(ho_RegionTrans, &ho_circleMudle, 50);

	{
		HTuple end_val79 = hv_circleTimes;
		HTuple step_val79 = 1;
		for (hv_Index = 1; hv_Index.Continue(end_val79, step_val79); hv_Index += step_val79)
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
				disp_message(hv_hwnd, "topfun5 δ�ҵ��Գ�����", "window", ((hv_Height1 - 400) + 300) / hv_h,
					20, "red", "true");
				(*hv_ret5) = -3;
				return;
			}
			ShapeTrans(ho_TwoSymmetryGous, &ho_TwoSymmetryGousConvex, "convex");
			OpeningCircle(ho_TwoSymmetryGousConvex, &ho_TwoSymmetryGousOpened, 30);
			RegionFeatures(ho_TwoSymmetryGousOpened, "max_diameter", &hv_twoOutGouDiameter);

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
		}
	}

	TupleConcat(hv_GouRadiusRight, hv_GouRadiusLeft, &hv_GouRadiusAll0);






	//��2��ͼƬ
	Threshold(ho_imageCorrected1, &ho_Regions, 0, hv_graythreshold);
	Connection(ho_Regions, &ho_ConnectedRegions);
	SelectShapeStd(ho_ConnectedRegions, &ho_SelectedRegions, "max_area", 70);
	FillUp(ho_SelectedRegions, &ho_RegionFillUp);
	ErosionCircle(ho_RegionFillUp, &ho_RegionErosion, 30);
	GenCircle(&ho_Circle, hv_Row, hv_Column, hv_tongPianRadRadius + 27);
	Difference(ho_SelectedRegions, ho_Circle, &ho_RegionDifference);
	OpeningCircle(ho_RegionDifference, &ho_RegionOpening, 8);
	Connection(ho_RegionOpening, &ho_ConnectedRegions2);
	SelectShape(ho_ConnectedRegions2, &ho_ConnectedRegions2, "area", "and", 9000, 28000);
	CountObj(ho_ConnectedRegions2, &hv_Number);
	hv_circleTimes = hv_gouCount / 2;
	if (0 != (hv_Number != hv_gouCount))
	{
		disp_message(hv_hwnd, "topfun5 ��ȡ������������ ��=" + hv_gouCount, "window", ((hv_Height1 - 400) + 300) / hv_h,
			20, "red", "true");
		(*hv_ret5) = -2;
		if (0 != ((hv_Number % 2) == 1))
		{
			hv_circleTimes = (hv_Number + 1) / 2;
		}
		return;
	}

	hv_GouRadiusRight = HTuple();
	hv_GouRadiusLeft = HTuple();
	GenEmptyRegion(&ho_gouRadiusLineRight);
	GenEmptyRegion(&ho_gouRadiusLineLeft);

	SelectObj(ho_ConnectedRegions2, &ho_ObjectSelected1, 1);
	ShapeTrans(ho_ObjectSelected1, &ho_RegionTrans, "outer_circle");
	DilationCircle(ho_RegionTrans, &ho_circleMudle, 50);

	{
		HTuple end_val183 = hv_circleTimes;
		HTuple step_val183 = 1;
		for (hv_Index = 1; hv_Index.Continue(end_val183, step_val183); hv_Index += step_val183)
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
				disp_message(hv_hwnd, "topfun5 δ�ҵ��Գ�����", "window", ((hv_Height1 - 400) + 300) / hv_h,
					20, "red", "true");
				(*hv_ret5) = -3;
				return;
			}
			ShapeTrans(ho_TwoSymmetryGous, &ho_TwoSymmetryGousConvex, "convex");
			OpeningCircle(ho_TwoSymmetryGousConvex, &ho_TwoSymmetryGousOpened, 30);
			RegionFeatures(ho_TwoSymmetryGousOpened, "max_diameter", &hv_twoOutGouDiameter);

			//�뾶�������䳤��
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
		}
	}

	TupleConcat(hv_GouRadiusRight, hv_GouRadiusLeft, &hv_GouRadiusAll1);







	//��3��ͼƬ
	Threshold(ho_imageCorrected2, &ho_Regions, 0, hv_graythreshold);
	Connection(ho_Regions, &ho_ConnectedRegions);
	SelectShapeStd(ho_ConnectedRegions, &ho_SelectedRegions, "max_area", 70);
	FillUp(ho_SelectedRegions, &ho_RegionFillUp);
	ErosionCircle(ho_RegionFillUp, &ho_RegionErosion, 30);
	GenCircle(&ho_Circle, hv_Row, hv_Column, hv_tongPianRadRadius + 27);
	Difference(ho_SelectedRegions, ho_Circle, &ho_RegionDifference);
	OpeningCircle(ho_RegionDifference, &ho_RegionOpening, 8);
	Connection(ho_RegionOpening, &ho_ConnectedRegions2);
	SelectShape(ho_ConnectedRegions2, &ho_ConnectedRegions2, "area", "and", 9000, 28000);
	CountObj(ho_ConnectedRegions2, &hv_Number);
	hv_circleTimes = hv_gouCount / 2;
	if (0 != (hv_Number != hv_gouCount))
	{
		disp_message(hv_hwnd, "topfun5 ��ȡ������������ ��=" + hv_gouCount, "window", ((hv_Height1 - 400) + 300) / hv_h,
			20, "red", "true");
		(*hv_ret5) = -2;
		if (0 != ((hv_Number % 2) == 1))
		{
			hv_circleTimes = (hv_Number + 1) / 2;
		}
		return;
	}

	hv_GouRadiusRight = HTuple();
	hv_GouRadiusLeft = HTuple();
	GenEmptyRegion(&ho_gouRadiusLineRight);
	GenEmptyRegion(&ho_gouRadiusLineLeft);

	SelectObj(ho_ConnectedRegions2, &ho_ObjectSelected1, 1);
	ShapeTrans(ho_ObjectSelected1, &ho_RegionTrans, "outer_circle");
	DilationCircle(ho_RegionTrans, &ho_circleMudle, 50);

	{
		HTuple end_val290 = hv_circleTimes;
		HTuple step_val290 = 1;
		for (hv_Index = 1; hv_Index.Continue(end_val290, step_val290); hv_Index += step_val290)
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
				disp_message(hv_hwnd, "topfun5 δ�ҵ��Գ�����", "window", ((hv_Height1 - 400) + 300) / hv_h,
					20, "red", "true");
				(*hv_ret5) = -3;
				return;
			}
			ShapeTrans(ho_TwoSymmetryGous, &ho_TwoSymmetryGousConvex, "convex");
			OpeningCircle(ho_TwoSymmetryGousConvex, &ho_TwoSymmetryGousOpened, 30);
			RegionFeatures(ho_TwoSymmetryGousOpened, "max_diameter", &hv_twoOutGouDiameter);


			//�뾶�������䳤��
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
		}
	}

	TupleConcat(hv_GouRadiusRight, hv_GouRadiusLeft, &hv_GouRadiusAll2);



	//��������֮�������ֵ
	hv_subAbsMaxAll = HTuple();
	//0-1
	TupleSub(hv_GouRadiusAll0, hv_GouRadiusAll1, &hv_sub0__1);
	TupleAbs(hv_sub0__1, &hv_subAbs0__1);
	TupleMax(hv_subAbs0__1, &hv_subAbsMax0__1);
	hv_subAbsMaxAll = hv_subAbsMaxAll.TupleConcat(hv_subAbsMax0__1);
	//0-2
	TupleSub(hv_GouRadiusAll0, hv_GouRadiusAll2, &hv_sub0__2);
	TupleAbs(hv_sub0__2, &hv_subAbs0__2);
	TupleMax(hv_subAbs0__2, &hv_subAbsMax0__2);
	hv_subAbsMaxAll = hv_subAbsMaxAll.TupleConcat(hv_subAbsMax0__2);
	//1-2
	TupleSub(hv_GouRadiusAll1, hv_GouRadiusAll2, &hv_sub1__2);
	TupleAbs(hv_sub1__2, &hv_subAbs1__2);
	TupleMax(hv_subAbs1__2, &hv_subAbsMax1__2);
	hv_subAbsMaxAll = hv_subAbsMaxAll.TupleConcat(hv_subAbsMax1__2);

	TupleMax(hv_subAbsMaxAll, &hv_Max395);
	TupleMin(hv_subAbsMaxAll, &hv_Min396);

	hv_a400 = 3;
	//�Ƿ���ʾ�𶯼�����ص�����Ϣ����
	if (0 != 1)
	{
		//��ʾÿ��ͼ�ĳ�����״ͼ
		//5  10 ����ʾ����ϵcoordinateFun�����е�dev_set_part �����й�
		hv_coordinateRow = 2670 / 5;
		hv_coordinateColumn = 600 / 10;

		hv_GouRadiusAll0Show.Clear();
		hv_GouRadiusAll0Show.Append(hv_GouRadiusAll0);
		hv_GouRadiusAll0Show.Append(1210);
		coordinateFun(hv_hwnd, "magenta", hv_coordinateRow, hv_coordinateColumn, hv_GouRadiusAll0Show);

		hv_GouRadiusAll1Show.Clear();
		hv_GouRadiusAll1Show.Append(hv_GouRadiusAll1);
		hv_GouRadiusAll1Show.Append(1210);
		coordinateFun(hv_hwnd, "orange red", hv_coordinateRow, hv_coordinateColumn, hv_GouRadiusAll1Show);

		hv_GouRadiusAll2Show.Clear();
		hv_GouRadiusAll2Show.Append(hv_GouRadiusAll2);
		hv_GouRadiusAll2Show.Append(1210);
		coordinateFun(hv_hwnd, "cyan", hv_coordinateRow, hv_coordinateColumn, hv_GouRadiusAll2Show);

		hv_msgColor0 = "blue";
		hv_msgColor01 = "blue";
		hv_msgColor02 = "blue";
		hv_msgColor12 = "blue";
		//max value's color
		if (0 != (hv_Max395 == hv_subAbsMax0__1))
		{
			hv_msgColor01 = "magenta";
		}
		else if (0 != (hv_Max395 == hv_subAbsMax0__2))
		{
			hv_msgColor02 = "magenta";
		}
		else if (0 != (hv_Max395 == hv_subAbsMax1__2))
		{
			hv_msgColor12 = "magenta";
		}
		//the color who will be used
		if (0 != (hv_subAbsMax0__1<hv_a400))
		{
			TupleMedian(hv_GouRadiusAll1, &hv_Median);
			disp_message(hv_hwnd, ("01-1RM:" + hv_Median) + "pix", "window", (((hv_Height1 - 400) + 330) - 1400) / hv_h,
				105, "cyan", "false");
			hv_msgColor01 = "green";
		}
		else if (0 != (hv_subAbsMax0__2<hv_a400))
		{
			TupleMedian(hv_GouRadiusAll2, &hv_Median);
			disp_message(hv_hwnd, ("02-2RM:" + hv_Median) + "pix", "window", (((hv_Height1 - 400) + 330) - 1400) / hv_h,
				105, "cyan", "false");
			hv_msgColor02 = "green";
		}
		else if (0 != (hv_subAbsMax1__2<hv_a400))
		{
			TupleMedian(hv_GouRadiusAll2, &hv_Median);
			disp_message(hv_hwnd, ("12-2RM:" + hv_Median) + "pix", "window", (((hv_Height1 - 400) + 330) - 1400) / hv_h,
				105, "cyan", "false");
			hv_msgColor12 = "green";
		}
		else
		{
			disp_message(hv_hwnd, "NOSTD", "window", (((hv_Height1 - 400) + 330) - 1400) / hv_h,
				110, "cyan", "false");
			hv_msgColor0 = "red";
			hv_msgColor01 = "red";
			hv_msgColor02 = "red";
			hv_msgColor12 = "red";
		}
		disp_message(hv_hwnd, "topfun5 �������:" + hv_Max395, "window", ((hv_Height1 - 400) + 330) / hv_h,
			100, hv_msgColor0, "false");
		disp_message(hv_hwnd, "| 0-1:" + hv_subAbsMax0__1, "window", ((hv_Height1 - 400) + 330) / hv_h,
			260, hv_msgColor01, "false");
		disp_message(hv_hwnd, "| 0-2:" + hv_subAbsMax0__2, "window", ((hv_Height1 - 400) + 330) / hv_h,
			350, hv_msgColor02, "false");
		disp_message(hv_hwnd, "| 1-2:" + hv_subAbsMax1__2, "window", ((hv_Height1 - 400) + 330) / hv_h,
			440, hv_msgColor12, "false");

	}

	//ȷ������ͼƬ
	if (0 != (hv_subAbsMax0__1<hv_a400))
	{
		(*ho_imageCorrectedSTD) = ho_imageCorrected1;
	}
	else if (0 != (hv_subAbsMax0__2<hv_a400))
	{
		(*ho_imageCorrectedSTD) = ho_imageCorrected2;
	}
	else if (0 != (hv_subAbsMax1__2<hv_a400))
	{
		(*ho_imageCorrectedSTD) = ho_imageCorrected2;
	}
	else
	{
		(*hv_ret5) = -10;
		//disp_message (hwnd, 'topfun5 ��С����>'+a400, 'window', (Height1-400+300)/h, 20, 'red', 'false')
	}

	//hv_hwnd
	return;



}

/*
input_Scale_1  ���⾶���ر������ڣ�1Ϊ���䣩
input_Scale_2  �����ڿ׿���
input_Scale_3  ���빳�׽�����������Բ�뾶
input_Scale_4  �����뾶��ֵ��������0ֻʱ����Ʒ
*/
void DetectModule::detectTop(HalconCpp::HObject& input_Image, const HTuple& hv_WindowHandle,
	const HTuple& input_Param_BaoluoDiameterUp, const HTuple& input_Param_BaoluoDiameterDown,
	const HTuple& input_Param_GouWidthUp, const HTuple& input_Param_GouWidthDown,
	const HTuple& input_Scale_1, const HTuple& input_Scale_2,
	const HTuple& input_Scale_3, const HTuple& input_Scale_4,
	HTuple* hv_Result, HTuple* output_ExceptionInformtion)
{
	// ����ͼƬ���
	HString errorInformation("");
	HTuple hv_hwnd = hv_WindowHandle;
	HObject ho_imageCorrected0, ho_imageCorrected1, ho_imageCorrected2, ho_imageCorrectedSTD;

	try
	{
		SetColor(hv_hwnd, "green");
		SetDraw(hv_hwnd, "margin");
		SetLineWidth(hv_hwnd, 1);
		set_display_font(hv_hwnd, 11, "mono", "false", "false");

		funCorrectImage(input_Image, &ho_imageCorrected0, &ho_imageCorrected1, &ho_imageCorrected2);

		DispObj(ho_imageCorrected0, hv_hwnd);
		
		HObject  ho_region1, ho_region2, ho_region3, ho_region4, ho_gouTopRegions;
		HTuple  hv_ret1=0, hv_ret2 = 0, hv_ret3 = 0, hv_ret4 = 0, hv_ret5 = 0;


		//����������²۲���
		//fun1(ho_imageCorrected1, &ho_region1, hv_hwnd, &hv_ret1);

		//�⾶�������,�ҵ��ǻ���ͼƬ
		fun5(ho_imageCorrected0, ho_imageCorrected1, ho_imageCorrected2, &ho_imageCorrectedSTD,
			hv_hwnd, input_Scale_1, input_Param_BaoluoDiameterDown, input_Param_BaoluoDiameterUp, &hv_ret5);
		if (0 != (hv_ret5 == 0))
		{
				//������ֱ��
				fun4(ho_imageCorrectedSTD, &ho_region4, hv_hwnd, input_Scale_1, input_Scale_4, input_Param_BaoluoDiameterDown,
					input_Param_BaoluoDiameterUp, &hv_ret4);

				////������������
				fun2(ho_imageCorrectedSTD, &ho_gouTopRegions, &ho_region2, hv_hwnd, input_Param_GouWidthDown, input_Param_GouWidthUp, &hv_ret2);

				//�������ϸ񣬹�������������壬ֱ�����������⾶���ϸ�ʱҲ������
				if (0 != (HTuple(hv_ret2 == 0).TupleAnd(hv_ret4 == 0)))
				{
					//�������С���ƫ���ڶ²�����£�ģ���׻������
					fun3(ho_imageCorrectedSTD, ho_gouTopRegions, &ho_region3, hv_hwnd, input_Scale_2, input_Scale_3, &hv_ret3);
				}
		}
		
		(*hv_Result) = 0;
		set_display_font(hv_WindowHandle, 14, "mono", "true", "false");

		if (0 != (hv_ret5<0))
		{
			(*hv_Result) = 2;
			if (0 != (hv_ret5 == -10))
			{
				disp_message(hv_hwnd, "NG ��ʧ��", "window", 150, 10, "red", "true");
				fun5NgNum = fun5NgNum + 1;
			}
			else
			{
				disp_message(hv_hwnd, "NG ������", "window", 150, 10, "red", "true");
				hv_ret4 = -10;
			}
		}
		if (0 != (hv_ret2<0))
		{
			if (0 != (hv_ret2>-10))
			{
				//funMatchDifference (Image, WindowHandle)
			}
			(*hv_Result) = 2;
			disp_message(hv_hwnd, "NG ������", "window", 60, 10, "red", "true");
			fun2NgNum= fun2NgNum+1;
		}
		if (0 != (hv_ret3<0))
		{
			if (0 != (HTuple(hv_ret3>-10).TupleAnd(hv_ret3 != -3)))
			{
				//funMatchDifference (Image, WindowHandle)
			}
			(*hv_Result) = 2;
			disp_message(hv_hwnd, "NG ��ƫ��Ť", "window", 90, 10, "red", "true");
			fun3NgNum = fun3NgNum + 1;
		}
		if (0 != (hv_ret4<0))
		{
			if (0 != (HTuple(hv_ret4>-10).TupleAnd(hv_ret4 != -4)))
			{
				//funMatchDifference (Image, WindowHandle)
			}
			(*hv_Result) = 2;
			disp_message(hv_hwnd, "NG ������", "window", 120, 10, "red", "true");
			fun4NgNum = fun4NgNum + 1;
		}
		if (0 != (hv_ret1<0))
		{
			if (0 != (hv_ret1>-10))
			{
				//funMatchDifference (Image, WindowHandle)
			}
			(*hv_Result) = 4;
			disp_message(hv_hwnd, "NG ��������", "window", 30, 10, "red", "true");
			fun1NgNum = fun1NgNum + 1;
		}
		if (0 != ((((hv_ret1 + hv_ret2) + hv_ret3) + hv_ret4) == 0))
		{
			set_display_font(hv_hwnd, 20, "mono", "false", "false");
			disp_message(hv_hwnd, "OK-��Ʒ", "window", 40, 30, "blue", "true");
		}
		set_display_font(hv_hwnd, 11, "mono", "false", "false");

		disp_message(hv_hwnd,
			((((((((((((((("argv:" + input_Scale_1) + " ") + input_Scale_2) + " ") + input_Scale_3) + " ") + input_Scale_4) + " ") + input_Param_BaoluoDiameterUp) + " ") + input_Param_BaoluoDiameterDown) + " ") + input_Param_GouWidthUp) + " ") + input_Param_GouWidthDown),
			"window", 0, 200, "black", "true");
		disp_message(hv_hwnd, (((((((("ճ" + fun1NgNum) + "����") + fun2NgNum) + "��") + fun3NgNum) + "��") + fun4NgNum) + "��") + fun5NgNum,
			"window", 20, 0, "black", "true");

		*output_ExceptionInformtion = errorInformation;
	}
	catch (const HException& e)
	{
		*output_ExceptionInformtion = e.ErrorMessage();
		HTuple errorMsg;
		e.ToHTuple(&errorMsg);
		disp_message(hv_hwnd, errorMsg, "window", 60, 30, "red","true");
		//�쳣����·��� bad
		(*hv_Result) = 1;
	}
	//����0ʱֻ����Ʒ
	if (input_Scale_4 == 0)
	{
		(*hv_Result) = 0;
	}
}





////**************************************************************************************************
//
// ��һ��λ���ӿڣ���4��ͼƬ��������Դ��������µļ�⣩
//
////**************************************************************************************************
//���ڼ�¼������⺯�����ز������ۻ�����
HTuple top2_fun1NgNum = 0, top2_fun2NgNum = 0, top2_fun3NgNum = 0, top2_fun4NgNum = 0;

void top2fun1(HObject ho_Image, HObject *ho_region1, HTuple hv_prav2_1, HTuple hv_hwnd,
	HTuple *hv_ret1)
{

	// Local iconic variables
	HObject  ho_EmptyRegion, ho_ROI_0, ho_ImageReduced;
	HObject  ho_Regions, ho_RegionFillUp, ho_RegionOpening, ho_ImageReduced1;
	HObject  ho_circle1, ho_circle2, ho_circle3, ho_circle4;
	HObject  ho_RegionDilation1;

	// Local control variables
	HTuple  hv_graythreshold, hv_areaoffset, hv_tongPianRadRadius;
	HTuple  hv_caoCount, hv_Row, hv_Column, hv_Width, hv_Height;
	HTuple  hv_Width1, hv_Height1, hv_w, hv_h, hv_errorRgnArea;



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

	if (0 != 0)
	{
		disp_message(hv_hwnd, "topfun1 ѡ�������������eorror", "window", (hv_Height1 - 400) / hv_h,
			20, "red", "true");
		(*hv_ret1) = -1;
		return;
	}

	GenEllipse(&ho_ROI_0, 1253.5, 1813.5, HTuple(-0.838415).TupleRad(), 1640.18, 1524.72);
	ReduceDomain(ho_Image, ho_ROI_0, &ho_ImageReduced);
	Threshold(ho_ImageReduced, &ho_Regions, 0, 86);
	FillUp(ho_Regions, &ho_RegionFillUp);
	OpeningCircle(ho_RegionFillUp, &ho_RegionOpening, 800);
	ReduceDomain(ho_ImageReduced, ho_RegionOpening, &ho_ImageReduced1);

	ErosionCircle(ho_RegionOpening, &ho_circle1, 270);
	ErosionCircle(ho_RegionOpening, &ho_circle2, 370);
	ErosionCircle(ho_RegionOpening, &ho_circle3, 465);
	ErosionCircle(ho_RegionOpening, &ho_circle4, 560);



	//���׼��



	//ȱ�ϼ��
	Union1(ho_EmptyRegion, &(*ho_region1));
	RegionFeatures((*ho_region1), "area", &hv_errorRgnArea);
	if (0 != (HTuple(hv_errorRgnArea == 0).TupleAnd((*hv_ret1) == 0)))
	{










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
	//hv_hwnd


	return;
}

/*
hv_prav2_5 �����쳣��������ж���ֵ
*/
void DetectModule::detectTop2(HalconCpp::HObject& ho_Image, const HTuple& hv_WindowHandle,
	const HTuple& hv_prav2_1, const HTuple& hv_prav2_2,
	const HTuple& hv_prav2_3, const HTuple& hv_prav2_4,
	const HTuple& hv_prav2_5, const HTuple& hv_prav2_6,
	const HTuple& hv_prav2_7, const HTuple& hv_prav2_8,
	HTuple* hv_Result, HTuple* output_ExceptionInformtion)
{
	// ����ͼƬ���
	HString errorInformation("");
	HTuple hv_hwnd = hv_WindowHandle;
	HObject ho_imageCorrected, ho_imageCorrected1;

	try
	{
		SetColor(hv_hwnd, "green");
		SetDraw(hv_hwnd, "margin");
		SetLineWidth(hv_hwnd, 1);
		set_display_font(hv_hwnd, 11, "mono", "false", "false");

		DispObj(ho_imageCorrected, hv_hwnd);

		disp_message(hv_hwnd,
			(((((((((((((((/*argv*/":" + hv_prav2_1) + " ") + hv_prav2_2) + " ") + hv_prav2_3) + " ") + hv_prav2_4) + " ") + hv_prav2_5) + " ") + hv_prav2_6) + " ") + hv_prav2_7) + " ") + hv_prav2_8),
			"window", 0, 80, "black", "false");
		disp_message(hv_hwnd, (((((("-" + top2_fun3NgNum) + "-") + top2_fun2NgNum) + "����") + top2_fun1NgNum) + "-") + top2_fun4NgNum,
			"window", 20, 0, "black", "true");

		HObject  ho_region1, ho_region2, ho_region3, ho_region4;
		HTuple  hv_ret1 = 0, hv_ret2 = 0, hv_ret3 = 0, hv_ret4 = 0;

		//��ⶥ�����ף����棩��ȱ��
		top2fun1(ho_imageCorrected1, &ho_region1, hv_prav2_5, hv_hwnd, &hv_ret1);
		

		(*hv_Result) = 0;
		set_display_font(hv_WindowHandle, 14, "mono", "true", "false");


		if (0 != (hv_ret1<0))
		{
			if (0 != (hv_ret1 == -1))
			{
				disp_message(hv_hwnd, "NG ��������", "window", 50, 10, "red", "true");
				top2_fun1NgNum = top2_fun1NgNum + 1;
				(*hv_Result) = 5;
			}
		}

		if (0 != ((((hv_ret1 + hv_ret2) + hv_ret3) + hv_ret4) == 0))
		{
			set_display_font(hv_hwnd, 20, "mono", "false", "false");
			disp_message(hv_hwnd, "OK-��Ʒ", "window", 40, 30, "blue", "true");
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
		//�쳣����·��� bad
		(*hv_Result) = 1;
	}
	//����0ʱֻ����Ʒ
	if (hv_prav2_8 == 0)
	{
		(*hv_Result) = 0;
	}
}





////**************************************************************************************************
//
//side
//
////**************************************************************************************************
//���ڼ�¼������⺯�����ز������ۻ�����
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
	GenRectangle1(&ho_Rectangle, 40, 793.5, 1645, 1420);
	GrayProjections(ho_Rectangle, ho_Image, "simple", &hv_HorProjection, &hv_VertProjection);

	//***************
	//HorProjectionGrayVal ����Ĳ���
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
	HObject  ho_Contours, ho_Rectangle1, ho_RegionDilation1;
	HObject  ho_ImageReduced4, ho_Regions4, ho_RegionClosing3;
	HObject  ho_RegionFillUp2, ho_RegionOpening4, ho_ConnectedRegions1;
	HObject  ho_SelectedRegions1, ho_RegionFillUp1, ho_RegionOpening2;
	HObject  ho_RegionErosion, ho_Partitioned;

	// Local control variables
	HTuple  hv_chipHeight, hv_chipWidth0, hv_Row;
	HTuple  hv_Column, hv_Width, hv_Height, hv_Width1, hv_Height1;
	HTuple  hv_w, hv_h, hv_PianColumn2, hv_Number, hv_caoHeight;
	HTuple  hv_caoHeightsTuple, hv_Index, hv_Area, hv_Row1;
	HTuple  hv_Column1, hv_Phi, hv_Length1, hv_Length2, hv_PointOrder;
	HTuple  hv_caoHeightValues, hv_caoHeightValuesMax, hv_caoHeightValuesMin;
	HTuple  hv_caoHeightValuesMean, hv_addoffset, hv_caoHeightValuesMax_mm;
	HTuple  hv_caoHeightValuesMin_mm, hv_caoHeightValuesMean_mm;
	HTuple  hv_len1Value, hv_len1Valuesz, hv_errorRgnArea;



	(*hv_ret1) = 0;
	//��Ƭ�߶�,�����жϲ۵�������
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
	Threshold(ho_ImageReduced1, &ho_Regions1, 180, 255);
	ClosingRectangle1(ho_Regions1, &ho_RegionClosing2, 20, 150);
	OpeningRectangle1(ho_RegionClosing2, &ho_RegionOpening3, 500, 500);
	RegionFeatures(ho_RegionOpening3, "column2", &hv_PianColumn2);
	if (0 != (HTuple(0 == hv_PianColumn2).TupleOr(hv_PianColumn2>1900)))
	{
		disp_message(hv_hwnd, "sidefun1 ������ȡƬ��column1 error", "window", 900 / hv_h,
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
		disp_message(hv_hwnd, "sidefun1 ��ȡ���������� <23", "window", 900 / hv_h, 30,
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
				disp_message(hv_hwnd, "sidefun1 �۲�����", "window", 900 / hv_h, 30, "red",
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
		//������ϵĲ�������һ����ʹ��
		ConcatObj(ho_outEmptyRegion, ho_Rectangle1, &ho_outEmptyRegion);
		DilationRectangle1(ho_ObjectSelected, &ho_RegionDilation1, 0.5, 60);
		ReduceDomain(ho_Image, ho_RegionDilation1, &ho_ImageReduced4);
		Threshold(ho_ImageReduced4, &ho_Regions4, 0, 120);
		ClosingRectangle1(ho_Regions4, &ho_RegionClosing3, 20, 1);
		FillUp(ho_RegionClosing3, &ho_RegionFillUp2);
		OpeningRectangle1(ho_RegionFillUp2, &ho_RegionOpening4, 20, 3);
		Connection(ho_RegionOpening4, &ho_ConnectedRegions1);
		SelectShapeStd(ho_ConnectedRegions1, &ho_SelectedRegions1, "max_area", 70);
		FillUp(ho_SelectedRegions1, &ho_RegionFillUp1);
		OpeningRectangle1(ho_RegionFillUp1, &ho_RegionOpening2, 100, 1);
		ErosionRectangle1(ho_RegionOpening2, &ho_RegionErosion, 1, 25);
		PartitionRectangle(ho_RegionErosion, &ho_Partitioned, 20, 200);
		RegionFeatures(ho_Partitioned, "height", &hv_caoHeightValues);
		TupleMax(hv_caoHeightValues, &hv_caoHeightValuesMax);
		TupleMin(hv_caoHeightValues, &hv_caoHeightValuesMin);
		TupleMean(hv_caoHeightValues, &hv_caoHeightValuesMean);
		//�Կ�������,��Ե������
		hv_addoffset = 0;
		hv_caoHeightValuesMax_mm = (hv_caoHeightValuesMax - hv_addoffset)*0.00982;
		hv_caoHeightValuesMin_mm = (hv_caoHeightValuesMin - hv_addoffset)*0.00982;
		hv_caoHeightValuesMean_mm = (hv_caoHeightValuesMean - hv_addoffset)*0.00982;


		hv_len1Value = hv_caoHeightValuesMean_mm;

		//�жϿ���
		hv_len1Valuesz = hv_len1Value.TupleString(".2");
		if (0 != (HTuple(hv_len1Value<hv_CaoWidthMin).TupleOr(hv_len1Value>hv_CaoWidthMax)))
		{
			//�ڲۿ���ʱ�������±�Ե�а�ɫ����ʱ����������
			disp_message(hv_hwnd, (((hv_Index - 1) + " �ۿ���") + hv_len1Valuesz) + "mm", "window",
				(hv_Row + 60) / hv_h, hv_Column / hv_w, "red", "true");
			ConcatObj(ho_EmptyRegion, ho_Rectangle1, &ho_EmptyRegion);
		}
		else if (0 != 1)
		{
			disp_message(hv_hwnd, (((hv_Index - 1) + " �ۿ���") + hv_len1Valuesz) + "mm", "window",
				(hv_Row + 60) / hv_h, hv_Column / hv_w, "blue", "true");
			 
				DispObj(ho_Rectangle1, hv_hwnd);
		}
	}

	Union1(ho_outEmptyRegion, &(*ho_outRegion1));



	Union1(ho_EmptyRegion, &(*ho_region1));
	RegionFeatures((*ho_region1), "area", &hv_errorRgnArea);
	if (0 != (HTuple(hv_errorRgnArea>0).TupleAnd((*hv_ret1) == 0)))
	{
		(*hv_ret1) = -10;
	}

	 
		SetColor(hv_hwnd, "red");
	 
		DispObj((*ho_region1), hv_hwnd);
	//hv_hwnd
	return;
}

void Sidefun2(HObject ho_Image, HObject ho_inRegion2, HObject *ho_region2, HTuple hv_hwnd,
	HTuple hv_MaociWidth, HTuple hv_MaociHeight, HTuple *hv_ret2)
{

	// Local iconic variables
	HObject  ho_EmptyRegion, ho_RectangleLeft, ho_ConnectedRegions;
	HObject  ho_ObjectSelected, ho_ObjectSelected_, ho_RegionMiddle;
	HObject  ho_ImageReduced, ho_Regions, ho_ConnectedRegions1;
	HObject  ho_SelectedRegions, ho_ObjectSelected1;

	// Local control variables
	HTuple  hv_heightThreshold, hv_Row, hv_Column;
	HTuple  hv_Width, hv_Height, hv_Width1, hv_Height1, hv_w;
	HTuple  hv_h, hv_Value5, hv_inRegion2Column1, hv_debugMark;
	HTuple  hv_Index, hv_ObjectSelectedcolumn1, hv_Value2, hv_xxxxx;
	HTuple  hv_Number, hv_Index1, hv_errorRgnArea;


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
		disp_message(hv_hwnd, "sidefun2 ��ȡ���������� <11", "window", 1000 / hv_h, 30,
			"red", "true");
		(*hv_ret2) = -1;
		return;
	}

	RegionFeatures(ho_inRegion2, "column2", &hv_Value5);
	RegionFeatures(ho_inRegion2, "column1", &hv_inRegion2Column1);
	GenRectangle1(&ho_RectangleLeft, 0, hv_inRegion2Column1, hv_Height1, hv_inRegion2Column1 + 110);


	Connection(ho_inRegion2, &ho_ConnectedRegions);


	hv_debugMark = 1;

	for (hv_Index = 1; hv_Index <= 22; hv_Index += 1)
	{
		SelectObj(ho_ConnectedRegions, &ho_ObjectSelected, hv_Index);
		RegionFeatures(ho_ObjectSelected, "column1", &hv_ObjectSelectedcolumn1);
		RegionFeatures(ho_ObjectSelected, "rect2_len2", &hv_Value2);
		hv_xxxxx = 25;
		ErosionRectangle1(ho_ObjectSelected, &ho_ObjectSelected_, 1, hv_xxxxx);
		Difference(ho_ObjectSelected_, ho_RectangleLeft, &ho_RegionMiddle);

		ReduceDomain(ho_Image, ho_RegionMiddle, &ho_ImageReduced);
		Threshold(ho_ImageReduced, &ho_Regions, 0, 35);
		Connection(ho_Regions, &ho_ConnectedRegions1);
		SelectShape(ho_ConnectedRegions1, &ho_SelectedRegions, "height", "and", 25, 100);
		CountObj(ho_SelectedRegions, &hv_Number);
		{
			HTuple end_val43 = hv_Number;
			HTuple step_val43 = 1;
			for (hv_Index1 = 1; hv_Index1.Continue(end_val43, step_val43); hv_Index1 += step_val43)
			{
				SelectObj(ho_SelectedRegions, &ho_ObjectSelected1, hv_Index1);

				if (0 != 1)
				{




					if (0 != (hv_debugMark == 1))
					{
						// stop(...); only in hdevelop
					}
					ConcatObj(ho_EmptyRegion, ho_ObjectSelected1, &ho_EmptyRegion);
				}
			}
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
	//hv_hwnd
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
	HObject  ho_ConnectedRegions2, ho_SelectedRegions1, ho_RegionClosing;
	HObject  ho_RegionOpening2, ho_ObjectSelectedCaoUp, ho_ObjectSelectedCaoMid;
	HObject  ho_ObjectSelectedCaoDown, ho_RegionUnionCaoThree;
	HObject  ho_PianErrDouble;

	// Local control variables
	HTuple  hv_Row, hv_Column, hv_Width, hv_Height;
	HTuple  hv_Width1, hv_Height1, hv_w, hv_h, hv_pianWidths;
	HTuple  hv_Index, hv_pianWidth, hv_Value, hv_Maxarea, hv_Maxwidth;
	HTuple  hv_Value1, hv_Value2, hv_Value3, hv_Value4, hv_errorRgnArea;
	HTuple  hv_pianwidthDifs, hv_Index1, hv_pianwidthDif, hv_Max;
	HTuple  hv_Indices;


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
		disp_message(hv_hwnd, "sidefun3 ��ȡ���������� <10", "window", 1100 / hv_h, 30,
			"red", "true");
		(*hv_ret3) = -1;
		return;
	}

	hv_pianWidths = HTuple();

	Connection(ho_inRegion3, &ho_ConnectedRegions);
	for (hv_Index = 1; hv_Index <= 22; hv_Index += 1)
	{
		SelectObj(ho_ConnectedRegions, &ho_ObjectSelected, hv_Index);
		SelectObj(ho_ConnectedRegions, &ho_ObjectSelected1, hv_Index + 1);
		Union2(ho_ObjectSelected, ho_ObjectSelected1, &ho_RegionUnion);
		ShapeTrans(ho_RegionUnion, &ho_RegionTrans, "convex");
		Difference(ho_RegionTrans, ho_RegionUnion, &ho_RegionDifference);
		OpeningCircle(ho_RegionDifference, &ho_RegionOpening, 40);
		//��ۼ���룬���ڲ۷ֶȼ��
		RegionFeatures(ho_RegionOpening, "rect2_len2", &hv_pianWidth);
		hv_pianWidths = hv_pianWidths.TupleConcat(hv_pianWidth * 2);

		ReduceDomain(ho_Image, ho_RegionOpening, &ho_ImageReduced2);
		Threshold(ho_ImageReduced2, &ho_Regions3, 110, 255);
		OpeningCircle(ho_Regions3, &ho_RegionOpening1, 3.5);
		ShapeTrans(ho_RegionOpening1, &ho_RegionTrans1, "rectangle2");

		ErosionCircle(ho_RegionTrans1, &ho_RegionErosion, 6);
		MoveRegion(ho_RegionErosion, &ho_RegionMoved, 3, 0);
		ReduceDomain(ho_Image, ho_RegionMoved, &ho_ImageReduced);
		//�߷�ֵ����ȡ����
		Threshold(ho_ImageReduced, &ho_Regions, 0, 161);
		RegionFeatures(ho_Regions, "area", &hv_Value);

		//�ͷ�ֵ����ȡ��ѹ������
		Threshold(ho_ImageReduced, &ho_Regions2, 0, 85);
		Connection(ho_Regions2, &ho_ConnectedRegions1);
		SelectShapeStd(ho_ConnectedRegions1, &ho_SelectedRegions, "max_area", 70);
		RegionFeatures(ho_SelectedRegions, "area", &hv_Maxarea);
		RegionFeatures(ho_SelectedRegions, "width", &hv_Maxwidth);
		RegionFeatures(ho_SelectedRegions, (HTuple("row").Append("column")), &hv_Value1);
		RegionFeatures(ho_RegionErosion, ((HTuple("row1").Append("row2")).Append("column2")),
			&hv_Value2);

		//
		if (0 != (HTuple(hv_Value>hv_blackArea).TupleOr(hv_Maxarea>(hv_blackArea / 10))))
		{
			//�������±�Ե������ɵ�����
			if (0 != (HTuple(hv_Maxarea<700).TupleAnd(hv_Maxwidth>80)))
			{
				if (0 != (HTuple(HTuple(hv_Value1[0])<(HTuple(hv_Value2[0]) + 10)).TupleOr(HTuple(hv_Value1[0])>(HTuple(hv_Value2[1]) - 10))))
				{
					continue;
				}
			}
			//�����ײ�С������ɵ�����
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
			//������ת��������ɵ�����
			//��ת���������������ƣ����нϸߵĻҶ�100+-������ת�����и������ڲ�������Ƭ�����ȴ�������
			ClosingCircle(ho_Regions, &ho_RegionClosing, 8);
			OpeningCircle(ho_RegionClosing, &ho_RegionOpening2, 10);
			RegionFeatures(ho_RegionOpening2, "width", &hv_Value4);
			if (0 != (HTuple(HTuple(hv_Value4>880).TupleAnd(hv_Maxarea<(hv_blackArea / 10))).TupleAnd(hv_Value>7000)))
			{
				continue;
			}

			if (0 != (HTuple(hv_Maxarea<500).TupleAnd(hv_Value<(hv_blackArea + 3000))))
			{
				continue;
			}

			ConcatObj(ho_EmptyRegion, ho_Regions, &ho_EmptyRegion);
		}
	}

	//�ڲۿ��ϸ�������£��۷ֶȼ��
	Union1(ho_EmptyRegion, &(*ho_region3));
	RegionFeatures((*ho_region3), "area", &hv_errorRgnArea);
	hv_pianwidthDifs = HTuple();
	if (0 != (HTuple(hv_errorRgnArea == 0).TupleAnd((*hv_ret3) == 0)))
	{
		for (hv_Index1 = 0; hv_Index1 <= 21; hv_Index1 += 1)
		{
			hv_pianwidthDif = HTuple(hv_pianWidths[hv_Index1]) - HTuple(hv_pianWidths[(hv_Index1 + 1) % 22]);
			hv_pianwidthDifs = hv_pianwidthDifs.TupleConcat(hv_pianwidthDif);
		}
		TupleAbs(hv_pianwidthDifs, &hv_pianwidthDifs);
		TupleMax(hv_pianwidthDifs, &hv_Max);
		TupleFind(hv_pianwidthDifs, hv_Max, &hv_Indices);
		if (0 != (hv_Max>50))
		{
			disp_message(hv_hwnd, "sidefun3 �۷ֶ��쳣", "window", 1100 / hv_h, 30, "red",
				"true");
			(*hv_ret3) = -5;
			SelectObj(ho_ConnectedRegions, &ho_ObjectSelectedCaoUp, hv_Indices);
			SelectObj(ho_ConnectedRegions, &ho_ObjectSelectedCaoMid, hv_Indices + 1);
			SelectObj(ho_ConnectedRegions, &ho_ObjectSelectedCaoDown, hv_Indices + 2);
			Union2(ho_ObjectSelectedCaoUp, ho_ObjectSelectedCaoDown, &ho_RegionUnionCaoThree
			);
			ShapeTrans(ho_RegionUnionCaoThree, &ho_PianErrDouble, "convex");
			 
				SetColor(hv_hwnd, "red");
			 
				DispObj(ho_PianErrDouble, hv_hwnd);
			return;
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
	//hv_hwnd
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

	//2019-09-18�� ����Ƭ���ϣ��󣩱�Ե��ȡ���ȶ����ϱ�Ե��״���ȶ����������±�Ե����Ϊ������λ����
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

	//����������ȡ
	for (hv_Index0 = 1; hv_Index0 <= 22; hv_Index0 += 1)
	{
		SelectObj(ho_inRegion4ConnectedRegions, &ho_caoUp, hv_Index0);
		SelectObj(ho_inRegion4ConnectedRegions, &ho_caoDown, hv_Index0 + 1);
		Union2(ho_caoUp, ho_caoDown, &ho_caoUpDownRegionUnion);
		ClosingRectangle1(ho_caoUpDownRegionUnion, &ho_RegionClosing2, 2, 300);
		Difference(ho_RegionClosing2, ho_caoUpDownRegionUnion, &ho_RegionDifference);
		Intersection(ho_RegionDifference, ho_RectangleSTD, &ho_RegionIntersectionSTD);
		RegionFeatures(ho_RegionIntersectionSTD, "row", &hv_chipValueSTD);

		//����������ȡ
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
			disp_message(hv_hwnd, "sidefun4 �����ϲ�����쳣", "window", 1200 / hv_h, 30,
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
			disp_message(hv_hwnd, "sidefun4 ��������δ�ҵ���", "window", 1200 / hv_h, 30,
				"red", "true");
			(*hv_ret4) = -2;
			//concat_obj (RegionErosion, EmptyRegion, EmptyRegion)
			 
				SetColor(hv_hwnd, "red");
			 
				DispObj(ho_RegionErosion, hv_hwnd);
			return;
		}
		else
		{
			//һ���������ȼ��
			RegionFeatures(ho_gouDingSingle, "height", &hv_SingleGouDingHeight);
			if (0 != (HTuple(hv_SingleGouDingHeight>110).TupleOr(hv_SingleGouDingHeight<70)))
			{
				disp_message(hv_hwnd, "sidefun4 ���������쳣", "window", 1200 / hv_h, 30, "red",
					"true");
				(*hv_ret4) = -3;
				//concat_obj (RegionErosion, EmptyRegion, EmptyRegion)
				 
					SetColor(hv_hwnd, "red");
				 
					DispObj(ho_gouDingSingle, hv_hwnd);
				return;
			}
			//��ȡ�������жϹ�ƫ��ͨ���жϵ��������²۵ľ�����жϣ�
			//���������
			RegionFeatures(ho_gouDingSingle, "row", &hv_SingleGouDingRow);
			RegionFeatures(ho_RegionIntersectionSTD, (HTuple("row1").Append("row2")), &hv_chipSTDRows);
			hv_upDistance = hv_SingleGouDingRow - HTuple(hv_chipSTDRows[0]);
			hv_downDitance = HTuple(hv_chipSTDRows[1]) - hv_SingleGouDingRow;
			hv_distanceDif = hv_upDistance - hv_downDitance;
			hv_gouPianDif = hv_gouPianDif.TupleConcat(hv_distanceDif);


			//��Ϊ��ת�Ƿ���жԾ����Ӱ��޴��������У����Բ��ܶ�ÿ�������й�ƫ�ж�
			//if (distanceDif> 30 or distanceDif<-30)
			//disp_message (hwnd, 'sidefun4 ��ƫ', 'window', (1200)/h, 30, 'red', 'true')
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
		disp_message(hv_hwnd, "sidefun4 ��ȡ������������ !=22", "window", 1200 / hv_h,
			30, "red", "true");
		(*hv_ret4) = -4;
		return;
	}

	//��ƫ�ж�
	//��Ϊ�����������������ж�(������ƫ���ų�)��ʣ�����й���ƫ���������������ֻ���ж���һ�����У���ȫ��������
	//���Ͻ����ڲۼ������ȵ������³�����������δȷ��
	TupleMean(hv_gouPianDif, &hv_gouPianDifMean);
	if (0 != (HTuple(hv_gouPianDifMean>hv_input_Scale_1Goupian).TupleOr(hv_gouPianDifMean<(-hv_input_Scale_1Goupian))))
	{
		(*hv_ret5) = -10;
		return;
	}

	//���߶��ж�
	RegionFeatures(ho_ConnectedRegions, "column2", &hv_GouDingColumn2Values);
	TupleSub(hv_ChipColumn2, hv_GouDingColumn2Values, &hv_GouHeightsValues);

	for (hv_Index = 0; hv_Index <= 21; hv_Index += 1)
	{
		TupleSelect(hv_GouHeightsValues, hv_Index, &hv_Selected);
		//�ж�
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
	//hv_hwnd
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
		disp_message(hv_hwnd, "sidefun3 ��ȡ���������� <18", "window", 1300 / hv_h, 30,
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
		//Ƭ���Ȳ��ȣ�ͨ��ʵ�ʿ��ȸ�ʴ
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
			disp_message(hv_hwnd, "sidefun5 ����������=2", "window", 1300 / hv_h, 30, "red",
				"true");
			(*hv_ret5) = -2;
			//stop ()
			return;
		}
		for (hv_Index1 = 1; hv_Index1 <= 2; hv_Index1 += 1)
		{
			SelectObj(ho_ConnectedRegions3, &ho_ObjectSelected2, hv_Index1);
			ReduceDomain(ho_Image, ho_ObjectSelected2, &ho_ImageReduced);
			//��ȡƬ�����������Ҷ�ֵ
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
			//��ֵԽ��Խ��ȷ
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
	//hv_hwnd
	return;


}


/*
hv_input_Scale_1Goupian  �����б������ڣ������ϰ��sidefun5���ã��� �����ĵ����²۾����ֵ�����أ�
input_Scale_2  Ƭ�Ϻ�ɫ��������жϷ�ֵ
*/
void DetectModule::detectSide(HalconCpp::HObject& input_Image, const HTuple& hv_WindowHandle,
	const HTuple& input_Param_CaoWidthUp, const HTuple& input_Param_CaoWidthDown,
	const HTuple& input_Param_MaociWidth, const HTuple& input_Param_MaociHeight,
	const HTuple& hv_input_Scale_1Goupian, const HTuple& input_Scale_2,
	const HTuple& input_Scale_3, const HTuple& input_Scale_4,
	HTuple* hv_Result, HTuple* output_ExceptionInformtion)
{
	// ����ͼƬ���
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
		disp_message(hv_hwnd, (((((((("�ۿ�" + Sidefun1NgNum) + "����") + Sidefun2NgNum) + "��") + Sidefun3NgNum) + "��") + Sidefun4NgNum) + "��") + Sidefun5NgNum,
			"window", 20, 200, "black", "true");

		//�ۿ�
		Sidefun1(ho_ImagePJ, &ho_region1, &ho_outRegion1, hv_hwnd, input_Param_CaoWidthDown,
			input_Param_CaoWidthUp, &hv_ret1);
		if (0 != (HTuple(hv_ret1 == 0).TupleOr(hv_ret1 == -10)))
		{
			//ճͭ����ľ��
			Sidefun2(ho_ImagePJ, ho_outRegion1, &ho_region2, hv_hwnd, input_Param_MaociWidth,
				input_Param_MaociHeight, &hv_ret2);

			//Ƭ�����ˡ�����
			Sidefun3(ho_ImagePJ, ho_outRegion1, &ho_region3, hv_hwnd, input_Scale_2, &hv_ret3);
			
			//�������������ײ����루���Ҽ���λ�ã�  + ��ƫ������ֵ�����߶Ȳ���ret4<0, ��ƫret5<0��
			//Sidefun4(ho_ImagePJ, ho_outRegion1, &ho_region4, hv_input_Scale_1Goupian,hv_hwnd, &hv_ret4, &hv_ret5);

			//�Թ����еļ�⣨���Ϸ������ɫ���ȣ�
			//Sidefun5(ho_ImagePJ, ho_outRegion1, &ho_region5, hv_WindowHandle, input_Scale_1, &hv_ret5);
		}


		//**�˴��ڶ���ճͭ��������ע��ע�⣺ͬʱ�ж��ֲ���ʱ��������Ϊճͭ
		//**2����λ����ж��ֲ������ʱ�����ӦΪճͭ
		(*hv_Result) = 0;
		set_display_font(hv_WindowHandle, 14, "mono", "true", "false");
		if (0 != (hv_ret1<0))
		{
			(*hv_Result) = 3;
			disp_message(hv_hwnd, "NG �ۿ���", "window", 30, 30, "red", "true");
			Sidefun1NgNum = Sidefun1NgNum + 1;
		}
		if (0 != (hv_ret3<0))
		{
			(*hv_Result) = 1;
			disp_message(hv_hwnd, "NG �����˻��ˡ�����", "window", 90, 30, "red", "true");
			Sidefun3NgNum = Sidefun3NgNum + 1;
		}
		if (0 != (hv_ret4<0))
		{
			(*hv_Result) = 2;
			disp_message(hv_hwnd, "NG ���߶�", "window", 120, 30, "red", "true");
			Sidefun4NgNum = Sidefun4NgNum + 1;
		}
		if (0 != (hv_ret5<0))
		{
			(*hv_Result) = 2;
			disp_message(hv_hwnd, "NG ��������", "window", 150, 30, "red", "true");
			Sidefun5NgNum = Sidefun5NgNum + 1;
		}
		if (0 != (hv_ret2<0))
		{
			(*hv_Result) = 4;
			disp_message(hv_hwnd, "NG ��������", "window", 60, 30, "red", "true");
			Sidefun2NgNum = Sidefun2NgNum + 1;
		}
		//���ڱ�ճͭ��ס���²۲�������������ճͭ������Ҳ��λճͭ
		if (0 != (HTuple(hv_ret1 == -2).TupleOr(hv_ret1 == -3)))
		{
			(*hv_Result) = 4;
		}
		if (0 != (((((hv_ret1 + hv_ret2) + hv_ret3) + hv_ret4) + hv_ret5) == 0))
		{
			set_display_font(hv_hwnd, 20, "mono", "false", "false");
			disp_message(hv_hwnd, "OK-��Ʒ", "window", 40, 30, "blue", "true");
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
		//�쳣����·��� bad
		(*hv_Result) = 1;
	}
	//����0ʱֻ����Ʒ
	if (input_Scale_4 == 0)
	{
		(*hv_Result) = 0;
	}
}

