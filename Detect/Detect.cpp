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
	HObject  ho_Image1, ho_Image2, ho_Image3, ho_RegionDilation;
	HObject  ho_ROI_0;

	// Local control variables
	HTuple  hv_Channels;


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

	//threshold (Image1, Regions, 0, 150)
	//* opening_circle (Regions, RegionOpening, 3.5)

	//* closing_circle (RegionOpening, RegionClosing, 3.5)
	//* connection (RegionClosing, ConnectedRegions)
	//* fill_up (ConnectedRegions, RegionFillUp)
	//* select_shape_std (RegionFillUp, SelectedRegions1, 'max_area', 70)
	//* difference (RegionFillUp, SelectedRegions1, SelectedRegions)
	//* union1 (SelectedRegions, RegionUnion)
	//* dilation_circle (RegionUnion, RegionDilation, 35)

	//count_obj (RegionDilation, Number)
	//* if (Number==0)
	//gen_region_points (RegionDilation, 100, 100)
	//* endif

	GenEllipse(&ho_ROI_0, 1365.5, 1869.5, HTuple(-1.48018).TupleRad(), 1548.52, 1378.95);


	Difference(ho_Image1, ho_ROI_0, &ho_RegionDilation);



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

void fun1(HObject ho_Image, HObject ho_GouDings, HTuple hv_circlrRow, HTuple hv_circlrColumn,
	HTuple *hv_ret1, HTuple *hv_MaxDif)
{

	// Local iconic variables
	HObject  ho_ConnectedRegions, ho_ObjectSelected;
	HObject  ho_Circle;

	// Local control variables
	HTuple  hv_Area, hv_Row, hv_Column, hv_StartPhi;
	HTuple  hv_StartPhi0, hv_EedPhi, hv_AmplitudeThreshold;
	HTuple  hv_RoiWidthLen2, hv_ArcRow_Measure_01_0, hv_ArcColumn_Measure_01_0;
	HTuple  hv_ArcRadius_Measure_01_0, hv_ArcAngleStart_Measure_01_0;
	HTuple  hv_ArcAngleExtent_Measure_01_0, hv_MsrHandle_Measure_01_0;
	HTuple  hv_Row_Measure_01_0, hv_Column_Measure_01_0, hv_Amplitude_Measure_01_0;
	HTuple  hv_Distance_Measure_01_0, hv_distanceGous, hv_Length;
	HTuple  hv_Index, hv_Selected, hv_ditanceDif, hv_Length1;
	HTuple  hv_Index1, hv_Selected1, hv_Abs;



	(*hv_ret1) = 0;

	Connection(ho_GouDings, &ho_ConnectedRegions);
	SelectObj(ho_ConnectedRegions, &ho_ObjectSelected, 1);
	AreaCenter(ho_ObjectSelected, &hv_Area, &hv_Row, &hv_Column);
	LineOrientation(hv_Row, hv_Column, hv_circlrRow, hv_circlrColumn, &hv_StartPhi);

	hv_StartPhi0 = hv_StartPhi.TupleDeg();
	hv_EedPhi = (360 - hv_StartPhi0).TupleRad();


	//Measure 01: Code generated by Measure 01
	//Measure 01: Prepare measurement
	hv_AmplitudeThreshold = 80;
	hv_RoiWidthLen2 = 25;
	SetSystem("int_zooming", "true");
	//Measure 01: Coordinates for circular arc Measure 01 [0]
	hv_ArcRow_Measure_01_0 = hv_circlrRow;
	hv_ArcColumn_Measure_01_0 = hv_circlrColumn;
	hv_ArcRadius_Measure_01_0 = 1145;
	GenCircle(&ho_Circle, hv_ArcRow_Measure_01_0, hv_ArcColumn_Measure_01_0, hv_ArcRadius_Measure_01_0);

	hv_ArcAngleStart_Measure_01_0 = hv_StartPhi;
	hv_ArcAngleExtent_Measure_01_0 = -6.28;
	//Measure 01: Create measure for circular arc Measure 01 [0]
	//Measure 01: Attention: This assumes all images have the same size!
	GenMeasureArc(hv_ArcRow_Measure_01_0, hv_ArcColumn_Measure_01_0, hv_ArcRadius_Measure_01_0,
		hv_ArcAngleStart_Measure_01_0, hv_ArcAngleExtent_Measure_01_0, HTuple(1).TupleMax2(hv_RoiWidthLen2.TupleMin2(hv_ArcRadius_Measure_01_0)),
		3840, 2748, "nearest_neighbor", &hv_MsrHandle_Measure_01_0);
	//Measure 01: ***************************************************************
	//Measure 01: * The code which follows is to be executed once / measurement *
	//Measure 01: ***************************************************************
	//Measure 01: The image is assumed to be made available in the
	//Measure 01: variable last displayed in the graphics window
	CopyObj(ho_Image, &ho_Image, 1, 1);
	//Measure 01: Execute measurements
	MeasurePos(ho_Image, hv_MsrHandle_Measure_01_0, 2, hv_AmplitudeThreshold, "all",
		"all", &hv_Row_Measure_01_0, &hv_Column_Measure_01_0, &hv_Amplitude_Measure_01_0,
		&hv_Distance_Measure_01_0);
	//Measure 01: Do something with the results
	//Measure 01: Clear measure when done
	CloseMeasure(hv_MsrHandle_Measure_01_0);



	hv_distanceGous = HTuple();

	TupleLength(hv_Distance_Measure_01_0, &hv_Length);
	{
		HTuple end_val46 = hv_Length - 1;
		HTuple step_val46 = 1;
		for (hv_Index = 0; hv_Index.Continue(end_val46, step_val46); hv_Index += step_val46)
		{
			TupleSelect(hv_Distance_Measure_01_0, hv_Index, &hv_Selected);
			if (0 != (hv_Selected>200))
			{
				hv_distanceGous = hv_distanceGous.TupleConcat(hv_Selected);
			}
		}
	}


	hv_ditanceDif = HTuple();
	TupleLength(hv_distanceGous, &hv_Length1);
	{
		HTuple end_val56 = hv_Length1 - 1;
		HTuple step_val56 = 1;
		for (hv_Index1 = 0; hv_Index1.Continue(end_val56, step_val56); hv_Index1 += step_val56)
		{
			TupleSelect(hv_distanceGous, hv_Index1, &hv_Selected);
			TupleSelect(hv_distanceGous, (hv_Index1 + 1) % hv_Length1, &hv_Selected1);
			TupleAbs(hv_Selected1 - hv_Selected, &hv_Abs);
			hv_ditanceDif = hv_ditanceDif.TupleConcat(hv_Abs);
			if (0 != (hv_Abs>30))
			{

				(*hv_ret1) = hv_Index1 + 1;
				break;
			}
		}
	}

	TupleMax(hv_ditanceDif, &(*hv_MaxDif));

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
	HTuple  hv_SZJiaoZhengValue, hv_gouRaMinPix, hv_gouRaMaxPix;
	HTuple  hv_errorRgnArea, hv_GouRadiusAll, hv_GouRadiusMedian;
	HTuple  hv_GouRadiusMean, hv_Index1, hv_Index1Last, hv_Index1Next;
	HTuple  hv_a205, hv_RadiusDifL, hv_RadiusDifN, hv_a227;


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
			if (0 != (HTuple(hv_twoOutGouDiameterChanged<(hv_gouchangMin - 0.00)).TupleOr(hv_twoOutGouDiameterChanged>(hv_gouchangMax + 0.00))))
			{
				hv_msgShowColor = "red";
				ConcatObj(ho_EmptyRegion, ho_diameterRegion1, &ho_EmptyRegion);
			}
			else
			{
				if (0 != (HTuple((hv_gouchangMin - hv_twoOutGouDiameterChanged) <= 0.00).TupleAnd((hv_gouchangMin - hv_twoOutGouDiameterChanged)>0)))
				{
					hv_twoOutGouDiameterChanged = hv_gouchangMin;
				}
				else if (0 != (HTuple((hv_twoOutGouDiameterChanged - hv_gouchangMax) <= 0.00).TupleAnd((hv_twoOutGouDiameterChanged - hv_gouchangMax)>0)))
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
			disp_message(hv_hwnd, hv_SZtwoOutGouDiameterChanged + " mm", "window", hv_rowDispPoint / hv_h,
				hv_columnDispPoint / hv_w, hv_msgShowColor, "false");
		}
	}

	//��ֱ�������ҿ�����С��׼ֵʱ�������������뾶�����Դ�����ʱҲ��Ϊ���ǣ����⾶С��
	//����������ֵ
	hv_gouRaMinPix = (hv_gouchangMin / (hv_input_Scale_1*0.011736)) / 2;
	hv_gouRaMaxPix = (hv_gouchangMax / (hv_input_Scale_1*0.011736)) / 2;

	Union1(ho_EmptyRegion, &(*ho_region4));
	RegionFeatures((*ho_region4), "area", &hv_errorRgnArea);
	if (0 != (HTuple(hv_errorRgnArea == 0).TupleAnd((*hv_ret4) == 0)))
	{
		TupleConcat(hv_GouRadiusRight, hv_GouRadiusLeft, &hv_GouRadiusAll);
		TupleMedian(hv_GouRadiusAll, &hv_GouRadiusMedian);
		TupleMean(hv_GouRadiusAll, &hv_GouRadiusMean);
		ConcatObj(ho_gouRadiusLineRight, ho_gouRadiusLineLeft, &ho_gouRadiusLineAll);
		{
			HTuple end_val205 = hv_gouCount - 1;
			HTuple step_val205 = 1;
			for (hv_Index1 = 0; hv_Index1.Continue(end_val205, step_val205); hv_Index1 += step_val205)
			{
				//*         if (GouRadiusAll[Index1]<GouRadiusMedian-5 or GouRadiusAll[Index1]>GouRadiusMedian+5)
				if (0 != (HTuple(HTuple(hv_GouRadiusAll[hv_Index1])<(hv_gouRaMinPix - 3)).TupleOr(HTuple(hv_GouRadiusAll[hv_Index1])>(hv_gouRaMaxPix + 3))))
				{
					hv_Index1Last = ((hv_Index1 - 1) + 22) % 22;
					hv_Index1Next = (hv_Index1 + 1) % 22;
					hv_a205 = hv_input_Scale_4;
					hv_RadiusDifL = HTuple(hv_GouRadiusAll[hv_Index1]) - HTuple(hv_GouRadiusAll[hv_Index1Last]);
					hv_RadiusDifN = HTuple(hv_GouRadiusAll[hv_Index1]) - HTuple(hv_GouRadiusAll[hv_Index1Next]);
					if (0 != (HTuple(HTuple(HTuple(hv_RadiusDifL>hv_a205).TupleOr(hv_RadiusDifL<(-hv_a205))).TupleOr(hv_RadiusDifN>hv_a205)).TupleOr(hv_RadiusDifN<(-hv_a205))))
					{
						//���
						//if (twoOutGouDiameterChangedAll[Index1%11]>(gouchangMin+0.08) and twoOutGouDiameterChangedAll[Index1%11]<gouchangMax-0.08)
						//continue
						//endif
						SelectObj(ho_gouRadiusLineAll, &ho_ObjectSelected, hv_Index1 + 1);
						ConcatObj(ho_EmptyRegion, ho_ObjectSelected, &ho_EmptyRegion);
						(*hv_ret4) = -9;
					}
				}
				if (0 != (HTuple(HTuple(hv_GouRadiusAll[hv_Index1])<(hv_GouRadiusMean - 7)).TupleOr(HTuple(hv_GouRadiusAll[hv_Index1])>(hv_GouRadiusMean + 7))))
				{
					hv_Index1Last = ((hv_Index1 - 1) + 22) % 22;
					hv_Index1Next = (hv_Index1 + 1) % 22;
					hv_a227 = hv_input_Scale_4 + 3;
					hv_RadiusDifL = HTuple(hv_GouRadiusAll[hv_Index1]) - HTuple(hv_GouRadiusAll[hv_Index1Last]);
					hv_RadiusDifN = HTuple(hv_GouRadiusAll[hv_Index1]) - HTuple(hv_GouRadiusAll[hv_Index1Next]);
					if (0 != (HTuple(HTuple(HTuple(hv_RadiusDifL>hv_a227).TupleOr(hv_RadiusDifL<(-hv_a227))).TupleOr(hv_RadiusDifN>hv_a227)).TupleOr(hv_RadiusDifN<(-hv_a227))))
					{
						//���
						//if (twoOutGouDiameterChangedAll[Index1%11]>(gouchangMin+0.08) and twoOutGouDiameterChangedAll[Index1%11]<gouchangMax-0.08)
						//continue
						//endif
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

void fun3(HObject ho_Image, HObject ho_Image1, HObject ho_gouTopRegions, HObject *ho_region3,
	HTuple hv_hwnd, HTuple hv_GouTaoWide, HTuple hv_GouGouTaoIntersectionInner_radius,
	HTuple *hv_ret3)
{

	// Local iconic variables
	HObject  ho_EmptyRegion, ho_ROI_0, ho_ImageReduced1;
	HObject  ho_Regions2, ho_ConnectedRegions2, ho_SelectedRegions2;
	HObject  ho_RegionOpening, ho_CircleCheckBaseIn, ho_CircleCheckBaseOut;
	HObject  ho_CircleCheckBaseCircle, ho_gouTopConnectedRegions;
	HObject  ho_ObjectSelectedBase, ho_RegionLinesBase, ho_gouRegionSTD0;
	HObject  ho_RegionLinesBaseaffined, ho_gouRegionSTD1, ho_RegionDifference;
	HObject  ho_gouTaoBaseIn, ho_gouTaoBaseOut, ho_gouTaoOutCircle;
	HObject  ho_gouTao, ho_minRR, ho_gouTaoNow, ho_ImageReduced;
	HObject  ho_Regions1, ho_ConnectedRegions1, ho_SelectedRegions1;
	HObject  ho_RegionDilation, ho_minRR_D;

	// Local control variables
	HTuple  hv_graythreshold, hv_tongPianRadRadius;
	HTuple  hv_gouCount, hv_Row, hv_Column, hv_Width, hv_Height;
	HTuple  hv_Width1, hv_Height1, hv_w, hv_h, hv_Number, hv_Area2;
	HTuple  hv_ret1, hv_MaxDif1, hv_Area1, hv_Row1, hv_Column1;
	HTuple  hv_cccc, hv_sina, hv_cosa, hv_rowOffset, hv_columnOffset;
	HTuple  hv_Index, hv_ratateNum, hv_HomMat2DIdentity, hv_HomMat2DRotate;
	HTuple  hv_isOkMark, hv_isDebugMark, hv_minRInIteration;
	HTuple  hv_minRRInfo, hv_ValueIR, hv_Max, hv_Indices, hv_Value;
	HTuple  hv_circleStep, hv_errorRgnArea;


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

	GenCircle(&ho_ROI_0, 1377.5, 1873.5, 900);
	ReduceDomain(ho_Image, ho_ROI_0, &ho_ImageReduced1);
	Threshold(ho_ImageReduced1, &ho_Regions2, 54, 254);
	Connection(ho_Regions2, &ho_ConnectedRegions2);
	SelectShape(ho_ConnectedRegions2, &ho_SelectedRegions2, (HTuple("circularity").Append("area")),
		"and", (HTuple(0.9303).Append(268349)), (HTuple(1.0917).Append(500000)));
	CountObj(ho_SelectedRegions2, &hv_Number);
	if (0 != (hv_Number == 0))
	{
		disp_message(hv_hwnd, "topfun3 ��ȡ��Բerror", "window", ((hv_Height1 - 400) + 100) / hv_h,
			20, "red", "true");
		(*hv_ret3) = -1;
		return;
	}
	AreaCenter(ho_SelectedRegions2, &hv_Area2, &hv_Row, &hv_Column);
	GenCircle(&ho_RegionOpening, hv_Row, hv_Column, 1000);




	hv_ret1 = 0;
	fun1(ho_Image, ho_gouTopRegions, hv_Row, hv_Column, &hv_ret1, &hv_MaxDif1);
	if (0 != (hv_ret1>0))
	{
		disp_message(hv_hwnd, (("topfun3->fun1 ����" + (hv_ret1 + 1)) + " maxDifDis=") + hv_MaxDif1,
			"window", ((hv_Height1 - 600) + 100) / hv_h, 20, "red", "false");
		(*hv_ret3) = -5;
		return;
	}
	disp_message(hv_hwnd, (HTuple(HTuple("topfun3->fun1 ����") + "x") + " maxDifDis=") + hv_MaxDif1,
		"window", ((hv_Height1 - 600) + 100) / hv_h, 20, "magenta", "false");
	if (0 != (hv_MaxDif1<13))
	{
		hv_GouTaoWide += 2;
	}

	//read_image (Image2, 'C:/Users/Administrator/Desktop/pic/Image_20200423152620473.bmp')


	//ִ�е��˴�ʱ�����һ���ϸ�
	//�����ж����ŷ�ֵ

	//��ƫ�������棬ͨ��������루��������Ŀ�ȣ����жϻ��ǲ��У������׹����ߵ�ԭ����
	GenCircle(&ho_CircleCheckBaseIn, hv_Row, hv_Column, 1060);
	GenCircle(&ho_CircleCheckBaseOut, hv_Row, hv_Column, 1210);
	Difference(ho_CircleCheckBaseOut, ho_CircleCheckBaseIn, &ho_CircleCheckBaseCircle
	);
	Connection(ho_gouTopRegions, &ho_gouTopConnectedRegions);

	//�������
	SelectObj(ho_gouTopConnectedRegions, &ho_ObjectSelectedBase, 1);
	AreaCenter(ho_ObjectSelectedBase, &hv_Area1, &hv_Row1, &hv_Column1);

	TupleSqrt(((hv_Row - hv_Row1)*(hv_Row - hv_Row1)) + ((hv_Column - hv_Column1)*(hv_Column - hv_Column1)),
		&hv_cccc);

	hv_sina = (hv_Row - hv_Row1) / hv_cccc;
	hv_cosa = ((hv_Column - hv_Column1) - 15) / hv_cccc;
	hv_rowOffset = 1250 * hv_sina;
	hv_columnOffset = 1250 * hv_cosa;

	GenRegionLine(&ho_RegionLinesBase, hv_Row, hv_Column, hv_Row - hv_rowOffset, hv_Column - hv_columnOffset);
	GenEmptyRegion(&ho_gouRegionSTD0);
	ConcatObj(ho_gouRegionSTD0, ho_RegionLinesBase, &ho_gouRegionSTD0);
	{
		HTuple end_val76 = hv_gouCount - 1;
		HTuple step_val76 = 1;
		for (hv_Index = 1; hv_Index.Continue(end_val76, step_val76); hv_Index += step_val76)
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
	Difference(ho_gouRegionSTD1, ho_CircleCheckBaseIn, &ho_RegionDifference);
	//�����ڿ׿��
	DilationCircle(ho_RegionDifference, &ho_gouTaoBaseIn, hv_GouTaoWide);
	DilationCircle(ho_gouTaoBaseIn, &ho_gouTaoBaseOut, 20);
	Difference(ho_gouTaoBaseOut, ho_gouTaoBaseIn, &ho_gouTaoOutCircle);
	Intersection(ho_gouTaoOutCircle, ho_CircleCheckBaseCircle, &ho_gouTao);


	hv_isOkMark = 0;
	hv_isDebugMark = 0;
	hv_minRInIteration = 100;
	GenEmptyRegion(&ho_minRR);
	hv_minRRInfo = HTuple();
	for (hv_Index = 0; hv_Index <= 1.5; hv_Index += 0.05)
	{
		hv_ratateNum = hv_Index.TupleRad();
		HomMat2dIdentity(&hv_HomMat2DIdentity);
		HomMat2dRotate(hv_HomMat2DIdentity, hv_ratateNum, hv_Row, hv_Column, &hv_HomMat2DRotate);
		AffineTransRegion(ho_gouTao, &ho_gouTaoNow, hv_HomMat2DRotate, "nearest_neighbor");
		//��һ��ͼƬ����
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
		if (0 != (HTuple(hv_Value[1])<hv_minRInIteration))
		{
			Union1(ho_SelectedRegions1, &ho_minRR);
			hv_minRInIteration = ((const HTuple&)hv_Value)[1];
			hv_minRRInfo = hv_Value;
		}
		if (0 != hv_isDebugMark)
		{
			 
				ClearWindow(hv_hwnd);
			 
				SetColor(hv_hwnd, "green");
			 
				DispObj(ho_Image, hv_hwnd);
			 
				DispObj(ho_gouTaoNow, hv_hwnd);
			 
				SetColor(hv_hwnd, "red");
			DilationCircle(ho_SelectedRegions1, &ho_RegionDilation, 15);
			 
				DispObj(ho_RegionDilation, hv_hwnd);
			disp_message(hv_hwnd, (((("topfun3 ���׽�Msg area/inR/maxD=" + HTuple(hv_Value[0])) + "/") + HTuple(hv_Value[1])) + "/") + HTuple(hv_Value[2]),
				"window", ((hv_Height1 - 400) + 100) / hv_h, 20, "blue", "true");

			// stop(...); only in hdevelop
		}
		//�ж�
		if (0 != (HTuple(hv_Value[1])<hv_GouGouTaoIntersectionInner_radius))
		{

			if (0 != hv_isDebugMark)
			{
				disp_message(hv_hwnd, HTuple("������,�˳�ѭ��------>>>>>"), "window", ((hv_Height1 - 400) + 100) / hv_h,
					20, "blue", "true");
				// stop(...); only in hdevelop
			}

			hv_isOkMark = 1;
			disp_message(hv_hwnd, (((("topfun3 ���׽�1 Msg area/inR/maxD=" + HTuple(hv_Value[0])) + "/") + HTuple(hv_Value[1])) + "/") + HTuple(hv_Value[2]),
				"window", ((hv_Height1 - 400) + 100) / hv_h, 20, "magenta", "false");
			break;
		}
		if (0 != (HTuple(HTuple(hv_Value[0])<50).TupleAnd(HTuple(hv_Value[1])<(hv_GouGouTaoIntersectionInner_radius + 1))))
		{

			if (0 != hv_isDebugMark)
			{
				disp_message(hv_hwnd, HTuple("������,�˳�ѭ��------>>>>>"), "window", ((hv_Height1 - 400) + 100) / hv_h,
					20, "blue", "true");
				// stop(...); only in hdevelop
			}

			hv_isOkMark = 1;
			disp_message(hv_hwnd, (((("topfun3 ���׽�1 Msg area/inR/maxD=" + HTuple(hv_Value[0])) + "/") + HTuple(hv_Value[1])) + "/") + HTuple(hv_Value[2]),
				"window", ((hv_Height1 - 400) + 100) / hv_h, 20, "magenta", "false");
			break;
		}

		//�ڶ���ͼƬ����
		ReduceDomain(ho_Image1, ho_gouTaoNow, &ho_ImageReduced);
		Threshold(ho_ImageReduced, &ho_Regions1, 0, 100);
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
		if (0 != (HTuple(hv_Value[1])<hv_minRInIteration))
		{
			Union1(ho_SelectedRegions1, &ho_minRR);
			hv_minRInIteration = ((const HTuple&)hv_Value)[1];
			hv_minRRInfo = hv_Value;
		}
		if (0 != hv_isDebugMark)
		{
			 
				ClearWindow(hv_hwnd);
			 
				SetColor(hv_hwnd, "green");
			 
				DispObj(ho_Image, hv_hwnd);
			 
				DispObj(ho_gouTaoNow, hv_hwnd);
			 
				SetColor(hv_hwnd, "red");
			DilationCircle(ho_SelectedRegions1, &ho_RegionDilation, 15);
			 
				DispObj(ho_RegionDilation, hv_hwnd);
			disp_message(hv_hwnd, (((("topfun3 ���׽���Msg area/inR/maxD=" + HTuple(hv_Value[0])) + "/") + HTuple(hv_Value[1])) + "/") + HTuple(hv_Value[2]),
				"window", ((hv_Height1 - 400) + 100) / hv_h, 20, "blue", "true");

			// stop(...); only in hdevelop
		}
		//�ж�
		if (0 != (HTuple(hv_Value[1])<hv_GouGouTaoIntersectionInner_radius))
		{

			if (0 != hv_isDebugMark)
			{
				disp_message(hv_hwnd, HTuple("������,�˳�ѭ����------>>>>>"), "window", ((hv_Height1 - 400) + 100) / hv_h,
					20, "blue", "true");
				// stop(...); only in hdevelop
			}

			hv_isOkMark = 1;
			disp_message(hv_hwnd, (((("topfun3 ���׽�2 Msg area/inR/maxD=" + HTuple(hv_Value[0])) + "/") + HTuple(hv_Value[1])) + "/") + HTuple(hv_Value[2]),
				"window", ((hv_Height1 - 400) + 100) / hv_h, 20, "magenta", "false");
			break;
		}
		if (0 != (HTuple(HTuple(hv_Value[0])<50).TupleAnd(HTuple(hv_Value[1])<(hv_GouGouTaoIntersectionInner_radius + 1))))
		{

			if (0 != hv_isDebugMark)
			{
				disp_message(hv_hwnd, HTuple("������,�˳�ѭ����------>>>>>"), "window", ((hv_Height1 - 400) + 100) / hv_h,
					20, "blue", "true");
				// stop(...); only in hdevelop
			}

			hv_isOkMark = 1;
			disp_message(hv_hwnd, (((("topfun3 ���׽�2 Msg area/inR/maxD=" + HTuple(hv_Value[0])) + "/") + HTuple(hv_Value[1])) + "/") + HTuple(hv_Value[2]),
				"window", ((hv_Height1 - 400) + 100) / hv_h, 20, "magenta", "false");
			break;
		}
		//��ѭ�����Ż�
		hv_circleStep = HTuple(hv_Value[1]) - (hv_GouGouTaoIntersectionInner_radius + 1);
		if (0 != (hv_circleStep>0))
		{
			hv_Index += 0.05*hv_circleStep;
		}

	}

	if (0 != hv_isDebugMark)
	{
		disp_message(hv_hwnd, "�˳�ѭ��---->>>>>", "window", ((hv_Height1 - 400) + 100) / hv_h,
			20, "blue", "true");
		// stop(...); only in hdevelop
	}

	if (0 != (hv_isOkMark == 0))
	{
		ConcatObj(ho_minRR, ho_EmptyRegion, &ho_EmptyRegion);
		//dilation_circle (minRR, minRR_D, 15)
		DilationRectangle1(ho_minRR, &ho_minRR_D, 20, 20);
		 
			SetColor(hv_hwnd, "magenta");
		 
			DispObj(ho_minRR_D, hv_hwnd);
		disp_message(hv_hwnd, (((("topfun3 ���׽�Msg area/inR/maxD=" + HTuple(hv_minRRInfo[0])) + "/") + HTuple(hv_minRRInfo[1])) + "/") + HTuple(hv_minRRInfo[2]),
			"window", ((hv_Height1 - 400) + 100) / hv_h, 20, "red", "false");
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
	hv_graythreshold = 60;
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
	Threshold(ho_ImageReduced1, &ho_Regions2, 0, 50);
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

	//����Χ�ж�
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
			hv_errClear = 0.04;
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
			if (0 != (HTuple(hv_len2Value<((hv_goukuanMin - hv_errClear) + 0.12)).TupleOr(hv_len2Value>((hv_goukuanMax + hv_errClear) + 0.3))))
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
	HTuple hv_hwnd, HTuple hv_input_Scale_1, HTuple hv_gouchangMin, HTuple hv_gouchangMax,
	HTuple *hv_ret5)
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
		//imageCorrectedSTD := imageCorrected1
		(*hv_ret5) = 10;
	}
	else if (0 != (hv_subAbsMax0__2<hv_a400))
	{
		//imageCorrectedSTD := imageCorrected2
		(*hv_ret5) = 20;
	}
	else if (0 != (hv_subAbsMax1__2<hv_a400))
	{
		//imageCorrectedSTD := imageCorrected2
		(*hv_ret5) = 21;
	}
	else
	{
		(*hv_ret5) = -10;
	}

	//hv_hwnd
	return;



}

/*
input_Scale_1  ���⾶���ر������ڣ�1Ϊ���䣩
input_Scale_2  �����ڿ׿��
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
	HObject ho_imageCorrected0, ho_imageCorrected1, ho_imageCorrected2, ho_imageCorrectedSTD, ho_imageCorrectedSTD1;

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
		fun5(ho_imageCorrected0, ho_imageCorrected1, ho_imageCorrected2, 
			hv_hwnd, input_Scale_1, input_Param_BaoluoDiameterDown, input_Param_BaoluoDiameterUp, &hv_ret5);
		if (0 != (hv_ret5>0))
		{
			if (0 != (hv_ret5 == 10))
			{
				ho_imageCorrectedSTD = ho_imageCorrected1;
				ho_imageCorrectedSTD1 = ho_imageCorrected0;
			}
			else if (0 != (hv_ret5 == 20))
			{
				ho_imageCorrectedSTD = ho_imageCorrected2;
				ho_imageCorrectedSTD1 = ho_imageCorrected0;
			}
			else if (0 != (hv_ret5 == 21))
			{
				ho_imageCorrectedSTD = ho_imageCorrected2;
				ho_imageCorrectedSTD1 = ho_imageCorrected1;
			}
			hv_ret5 = 0;
		}

		if (0 != (hv_ret5 == 0))
		{
				//������ֱ��
				fun4(ho_imageCorrectedSTD, &ho_region4, hv_hwnd, input_Scale_1, input_Scale_4, input_Param_BaoluoDiameterDown,
					input_Param_BaoluoDiameterUp, &hv_ret4);

				////����������
				fun2(ho_imageCorrectedSTD, &ho_gouTopRegions, &ho_region2, hv_hwnd, input_Param_GouWidthDown, input_Param_GouWidthUp, &hv_ret2);

				//�����ϸ񣬹�������������壬ֱ�����������⾶���ϸ�ʱҲ������
				if (0 != (HTuple(hv_ret2 == 0).TupleAnd(hv_ret4 == 0)))
				{
					//�������С���ƫ���ڶ²�����£�ģ���׻������
					fun3(ho_imageCorrectedSTD, ho_imageCorrectedSTD1, ho_gouTopRegions, &ho_region3, hv_hwnd, input_Scale_2, input_Scale_3, &hv_ret3);
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
			disp_message(hv_hwnd, "NG �����", "window", 60, 10, "red", "true");
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
		disp_message(hv_hwnd, (((((((("-" + fun1NgNum) + "����") + fun2NgNum) + "����") + fun3NgNum) + "��") + fun4NgNum) + "��") + fun5NgNum,
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

void top2fun1(HObject ho_Image, HObject *ho_region1, HTuple hv_prav2_5, HTuple hv_prav2_6,
	HTuple hv_hwnd, HTuple *hv_ret1)
{

	// Local iconic variables
	HObject  ho_EmptyRegion, ho_ROI_0, ho_ImageReduced;
	HObject  ho_Regions, ho_ConnectedRegions, ho_SelectedRegions;
	HObject  ho_SelectedRegions_, ho_circle1_, ho_circle2_, ho_circle3_;
	HObject  ho_circle4_, ho_circle5_, ho_circle5__, ho_circle6_;
	HObject  ho_circle7_, ho_Contours, ho_circle2_M, ho_circle3_M;
	HObject  ho_circle4_M, ho_circle2_D, ho_circle3_D, ho_circle4_D;
	HObject  ho_circle23_D, ho_circle234_D, ho_SelectedRegions_D;
	HObject  ho_circleSel_5Dif, ho_ImageReduced1_, ho_ImageReduced1_th;
	HObject  ho_RegionWide, ho_RegionDilation, ho_RegionWideIntersection;
	HObject  ho_ImageResult, ho_ImageResult_th, ho_RegionUnion;
	HObject  ho_RegionClosing, ho_ConnectedRegions1, ho_SelectedRegions1;
	HObject  ho_RegionUnion1, ho_circle2__, ho_RegionIntersection;
	HObject  ho_RegionOpening__, ho_RegionDilation2, ho_RegionIntersection1;
	HObject  ho_RegionDifference, ho_RegionUnion2, ho_ConnectedRegions2;
	HObject  ho_SelectedRegions2Max, ho_Difference65, ho_ImageReduced65;
	HObject  ho_Regions65, ho_ConnectedRegions65, ho_RegionFillUp65;
	HObject  ho_SelectedRegions65max, ho_Difference76, ho_ImageReduced76;
	HObject  ho_Regions76, ho_ConnectedRegions76, ho_SelectedRegions76;
	HObject  ho_RegionDifference54, ho_RegionErosion54, ho_ImageReduced54;
	HObject  ho_Regions54, ho_RegionOpening54, ho_RegionClosing54;
	HObject  ho_RegionFillUp54, ho_ConnectedRegions54, ho_SelectedRegions54;
	HObject  ho_RegionDifference43, ho_ImageReduced43, ho_Region43;
	HObject  ho_RegionOpening43, ho_RegionClosing43, ho_RegionFillUp43;
	HObject  ho_ConnectedRegions43, ho_SelectedRegions43, ho_RegionDilation1;

	// Local control variables
	HTuple  hv_graythreshold, hv_areaoffset, hv_tongPianRadRadius;
	HTuple  hv_caoCount, hv_Row, hv_Column, hv_Width, hv_Height;
	HTuple  hv_Width1, hv_Height1, hv_w, hv_h, hv_Number, hv_Area;
	HTuple  hv_Row1, hv_Column1, hv_R53, hv_Mean, hv_Deviation;
	HTuple  hv_Mean234, hv_Deviation234, hv_debugMark, hv_Number1;
	HTuple  hv_MaxInfoValues, hv_errorRgnArea, hv_Value, hv_NumberMax65;
	HTuple  hv_MaxInfo65Values, hv_Number76, hv_Mean54, hv_Deviation54;
	HTuple  hv_Number54, hv_Mean43, hv_Deviation43, hv_Number43;



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

	GenEllipse(&ho_ROI_0, 1400, 2000, HTuple(-0.838415).TupleRad(), 1640.18, 1524.72);
	ReduceDomain(ho_Image, ho_ROI_0, &ho_ImageReduced);
	Threshold(ho_ImageReduced, &ho_Regions, 100, 255);
	Connection(ho_Regions, &ho_ConnectedRegions);
	SelectShape(ho_ConnectedRegions, &ho_SelectedRegions, (HTuple("circularity").Append("area")),
		"and", (HTuple(0.85).Append(276735)), (HTuple(1).Append(492495)));
	CountObj(ho_SelectedRegions, &hv_Number);
	if (0 != (hv_Number == 0))
	{
		disp_message(hv_hwnd, "top2fun1 ѡ������Բeorror", "window", (hv_Height1 - 400) / hv_h,
			20, "red", "true");
		(*hv_ret1) = -1;
		return;
	}

	ClosingCircle(ho_SelectedRegions, &ho_SelectedRegions_, 5);
	OpeningCircle(ho_SelectedRegions_, &ho_SelectedRegions, 10);

	AreaCenter(ho_SelectedRegions, &hv_Area, &hv_Row1, &hv_Column1);
	//J��H��������ͨ���ڿ�����жϣ�J-381K/H-372K
	//Ϊprav2_6==0ʱ����������
	if (0 != (hv_prav2_6 != 0))
	{
		if (0 != (hv_prav2_6 == 1))
		{
			if (0 != (hv_Area<378000))
			{
				disp_message(hv_hwnd, "top2fun1��Ʒ�����쳣H", "window", (hv_Height1 - 400) / hv_h,
					20, "red", "true");
				(*hv_ret1) = -5;
				return;
			}
		}
		else if (0 != (hv_prav2_6 == 2))
		{
			if (0 != (hv_Area>378000))
			{
				disp_message(hv_hwnd, "top2fun1��Ʒ�����쳣J", "window", (hv_Height1 - 400) / hv_h,
					20, "red", "true");
				(*hv_ret1) = -5;
				return;
			}
		}
	}


	GenCircle(&ho_circle1_, hv_Row1, hv_Column1, 350);
	GenCircle(&ho_circle2_, hv_Row1, hv_Column1, 535);
	GenCircle(&ho_circle3_, hv_Row1, hv_Column1, 620);
	GenCircle(&ho_circle4_, hv_Row1, hv_Column1, 720);
	GenCircle(&ho_circle5_, hv_Row1, hv_Column1, 780);
	GenCircle(&ho_circle5__, hv_Row1, hv_Column1, 830);
	GenCircle(&ho_circle6_, hv_Row1, hv_Column1, 890);
	GenCircle(&ho_circle7_, hv_Row1, hv_Column1, 980);

	GenContourRegionXld(ho_circle2_, &ho_Contours, "border");
	GenRegionContourXld(ho_Contours, &ho_circle2_M, "margin");
	GenContourRegionXld(ho_circle3_, &ho_Contours, "border");
	GenRegionContourXld(ho_Contours, &ho_circle3_M, "margin");
	GenContourRegionXld(ho_circle4_, &ho_Contours, "border");
	GenRegionContourXld(ho_Contours, &ho_circle4_M, "margin");

	hv_R53 = 15;
	DilationCircle(ho_circle2_M, &ho_circle2_D, hv_R53);
	DilationCircle(ho_circle3_M, &ho_circle3_D, hv_R53);
	DilationCircle(ho_circle4_M, &ho_circle4_D, hv_R53);

	Union2(ho_circle2_D, ho_circle3_D, &ho_circle23_D);
	Union2(ho_circle23_D, ho_circle4_D, &ho_circle234_D);
	Intensity(ho_circle234_D, ho_ImageReduced, &hv_Mean, &hv_Deviation);
	DilationCircle(ho_SelectedRegions, &ho_SelectedRegions_D, 10);
	Difference(ho_circle5_, ho_SelectedRegions_D, &ho_circleSel_5Dif);
	ReduceDomain(ho_ImageReduced, ho_circleSel_5Dif, &ho_ImageReduced1_);
	Threshold(ho_ImageReduced1_, &ho_ImageReduced1_th, 50, 255);
	OpeningCircle(ho_ImageReduced1_th, &ho_RegionWide, 8);
	DilationCircle(ho_RegionWide, &ho_RegionDilation, 10);
	Intersection(ho_ImageReduced1_th, ho_RegionDilation, &ho_RegionWideIntersection
	);

	PaintRegion(ho_circle234_D, ho_ImageReduced1_, &ho_ImageResult, 6, "fill");
	Intensity(ho_circleSel_5Dif, ho_ImageResult, &hv_Mean234, &hv_Deviation234);

	Threshold(ho_ImageResult, &ho_ImageResult_th, 80, 255);
	Union2(ho_RegionWideIntersection, ho_ImageResult_th, &ho_RegionUnion);

	ClosingCircle(ho_RegionUnion, &ho_RegionClosing, 10);
	Connection(ho_RegionClosing, &ho_ConnectedRegions1);
	SelectShape(ho_ConnectedRegions1, &ho_SelectedRegions1, "area", "and", 10, 500000);
	Union1(ho_SelectedRegions1, &ho_RegionUnion1);

	hv_debugMark = 0;

	//052J��Ʒ��Ȧ2/1�����飬��Ϊ��Ʒ
	GenCircle(&ho_circle2__, hv_Row1, hv_Column1, 430);
	Intersection(ho_circle2__, ho_RegionUnion1, &ho_RegionIntersection);
	OpeningCircle(ho_RegionIntersection, &ho_RegionOpening__, 17);
	DilationCircle(ho_RegionOpening__, &ho_RegionDilation2, 35);
	Intersection(ho_RegionIntersection, ho_RegionDilation2, &ho_RegionIntersection1
	);

	Difference(ho_RegionUnion1, ho_circle2__, &ho_RegionDifference);
	Union2(ho_RegionDifference, ho_RegionIntersection1, &ho_RegionUnion2);

	//�С��ڻ�ȱ�ϼ��
	Connection(ho_RegionUnion2, &ho_ConnectedRegions2);
	SelectShapeStd(ho_ConnectedRegions2, &ho_SelectedRegions2Max, "max_area", 70);
	CountObj(ho_SelectedRegions2Max, &hv_Number1);
	if (0 != (hv_Number1>0))
	{
		RegionFeatures(ho_SelectedRegions2Max, (HTuple("area").Append("inner_radius")),
			&hv_MaxInfoValues);
		if (0 != (HTuple(HTuple(hv_MaxInfoValues[0])>2000).TupleAnd(HTuple(hv_MaxInfoValues[1])>18)))
		{
			if (0 != (hv_debugMark == 1))
			{
				// stop(...); only in hdevelop
			}
			ConcatObj(ho_EmptyRegion, ho_RegionUnion1, &ho_EmptyRegion);
		}
	}

	//���׼��
	Union1(ho_EmptyRegion, &(*ho_region1));
	RegionFeatures((*ho_region1), "area", &hv_errorRgnArea);
	if (0 != (HTuple(hv_errorRgnArea == 0).TupleAnd((*hv_ret1) == 0)))
	{
		RegionFeatures(ho_RegionUnion2, "area", &hv_Value);
		if (0 != (hv_Value>hv_prav2_5))
		{
			if (0 != (hv_debugMark == 1))
			{
				// stop(...); only in hdevelop
			}
			ConcatObj(ho_EmptyRegion, ho_RegionUnion1, &ho_EmptyRegion);
		}
		else
		{
			 
				DispObj(ho_SelectedRegions1, hv_hwnd);
		}
	}

	//���⻷ȱ�ϼ�⣬խ����ȥ��,���¿���������
	Difference(ho_circle6_, ho_circle5_, &ho_Difference65);
	ReduceDomain(ho_Image, ho_Difference65, &ho_ImageReduced65);
	Threshold(ho_ImageReduced65, &ho_Regions65, 70, 255);
	Connection(ho_Regions65, &ho_ConnectedRegions65);
	FillUp(ho_ConnectedRegions65, &ho_RegionFillUp65);
	SelectShapeStd(ho_RegionFillUp65, &ho_SelectedRegions65max, "max_area", 70);
	CountObj(ho_SelectedRegions65max, &hv_NumberMax65);
	if (0 != (hv_NumberMax65>0))
	{
		RegionFeatures(ho_SelectedRegions65max, (HTuple("area").Append("inner_radius")),
			&hv_MaxInfo65Values);
		if (0 != (HTuple(HTuple(hv_MaxInfo65Values[0])>1700).TupleAnd(HTuple(hv_MaxInfo65Values[1])>12)))
		{
			if (0 != (hv_debugMark == 1))
			{
				// stop(...); only in hdevelop
			}
			ConcatObj(ho_EmptyRegion, ho_SelectedRegions65max, &ho_EmptyRegion);
		}
	}

	//������ȱ�ϣ�ͨ��
	Difference(ho_circle7_, ho_circle6_, &ho_Difference76);
	ReduceDomain(ho_Image, ho_Difference76, &ho_ImageReduced76);
	Threshold(ho_ImageReduced76, &ho_Regions76, 240, 255);
	Connection(ho_Regions76, &ho_ConnectedRegions76);
	SelectShape(ho_ConnectedRegions76, &ho_SelectedRegions76, "inner_radius", "and",
		12, 50);
	CountObj(ho_SelectedRegions76, &hv_Number76);
	if (0 != (hv_Number76>0))
	{
		if (0 != (hv_debugMark == 1))
		{
			// stop(...); only in hdevelop
		}
		ConcatObj(ho_EmptyRegion, ho_SelectedRegions76, &ho_EmptyRegion);
	}

	//ȱ��or��Ȳ�������£��Ҷ�ֵ���첻�Ǻ�����������
	//3/4��֮��Ϊ͹��ƽ�棬������Ϊ�Ӵ�������մ��or�ֲڣ�Ҳ������ȻҶȲ��죬��ɴ�������
	//ÿһ��б�ȡ��߶ȡ��ֲڶȶ�����ȫ��ͬ���ڼ����΢����ʱ�������ϼ��
	//difference (circle5_, circle2_, RegionDifference52_)
	//difference (RegionDifference52_, circle234_D, circle234_D_52)
	//reduce_domain (ImageResult, circle234_D_52, ImageReduced234_D_52)
	//intensity (circle234_D_52, ImageReduced234_D_52, Mean234_D_52, Deviation234_D_52)
	//threshold (ImageReduced234_D_52, Regions234_D_52, Mean234_D_52+10, 255)
	//closing_circle (Regions234_D_52, RegionClosing234_D_52, 3.5)
	//fill_up (RegionClosing234_D_52, RegionFillUp234_D_52)
	//opening_circle (RegionFillUp234_D_52, RegionOpening234_D_52, 10)
	//connection (RegionOpening234_D_52, ConnectedRegions234_D_52)
	//select_shape (ConnectedRegions234_D_52, SelectedRegions234_D_52, 'area', 'and', 15000, 500000)
	//count_obj (SelectedRegions234_D_52, Number234_D_52)
	//if (Number234_D_52>0)
	//concat_obj (EmptyRegion, SelectedRegions234_D_52, EmptyRegion)
	//endif
	//������ֻ��5/4��б��������Ӧ���
	Difference(ho_circle5__, ho_circle4_, &ho_RegionDifference54);
	ErosionCircle(ho_RegionDifference54, &ho_RegionErosion54, 20);
	ReduceDomain(ho_Image, ho_RegionErosion54, &ho_ImageReduced54);
	Intensity(ho_RegionDifference54, ho_ImageReduced54, &hv_Mean54, &hv_Deviation54);
	Threshold(ho_ImageReduced54, &ho_Regions54, hv_Mean54 + 20, 255);
	OpeningCircle(ho_Regions54, &ho_RegionOpening54, 2);
	ClosingCircle(ho_RegionOpening54, &ho_RegionClosing54, 10);
	FillUp(ho_RegionClosing54, &ho_RegionFillUp54);
	OpeningCircle(ho_RegionFillUp54, &ho_RegionOpening54, 10);
	Connection(ho_RegionOpening54, &ho_ConnectedRegions54);
	SelectShape(ho_ConnectedRegions54, &ho_SelectedRegions54, "inner_radius", "and",
		18, 500);
	CountObj(ho_SelectedRegions54, &hv_Number54);
	if (0 != (hv_Number54>0))
	{
		if (0 != (hv_debugMark == 1))
		{
			// stop(...); only in hdevelop
		}
		ConcatObj(ho_EmptyRegion, ho_SelectedRegions54, &ho_EmptyRegion);
	}
	//��4/3�����Ӵ�Ħ��������մ����ֲڣ����м��
	Difference(ho_circle4_, ho_circle3_, &ho_RegionDifference43);
	ReduceDomain(ho_Image, ho_RegionDifference43, &ho_ImageReduced43);
	Intensity(ho_RegionDifference43, ho_ImageReduced43, &hv_Mean43, &hv_Deviation43);
	Threshold(ho_ImageReduced43, &ho_Region43, hv_Mean43 + 30, 255);
	OpeningCircle(ho_Region43, &ho_RegionOpening43, 2);
	ClosingCircle(ho_RegionOpening43, &ho_RegionClosing43, 10);
	FillUp(ho_RegionClosing43, &ho_RegionFillUp43);
	OpeningCircle(ho_RegionFillUp43, &ho_RegionOpening43, 10);
	Connection(ho_RegionOpening43, &ho_ConnectedRegions43);
	SelectShape(ho_ConnectedRegions43, &ho_SelectedRegions43, "inner_radius", "and",
		20, 500);
	CountObj(ho_SelectedRegions43, &hv_Number43);
	if (0 != (hv_Number43>0))
	{
		if (0 != (hv_debugMark == 1))
		{
			// stop(...); only in hdevelop
		}
		ConcatObj(ho_EmptyRegion, ho_SelectedRegions43, &ho_EmptyRegion);
	}


	Union1(ho_EmptyRegion, &(*ho_region1));
	RegionFeatures((*ho_region1), "area", &hv_errorRgnArea);
	if (0 != (HTuple(hv_errorRgnArea>0).TupleAnd((*hv_ret1) == 0)))
	{
		(*hv_ret1) = -10;
	}

	 
		SetColor(hv_hwnd, "red");
	//dev_set_line_width (2)
	DilationCircle((*ho_region1), &ho_RegionDilation1, 30);
	 
		DispObj(ho_RegionDilation1, hv_hwnd);
	//dev_set_line_width (1)
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

		CopyImage(ho_Image, &ho_imageCorrected);
		CopyImage(ho_Image, &ho_imageCorrected1);

		DispObj(ho_imageCorrected, hv_hwnd);

		HObject  ho_region1, ho_region2, ho_region3, ho_region4;
		HTuple  hv_ret1 = 0, hv_ret2 = 0, hv_ret3 = 0, hv_ret4 = 0;

		//��ⶥ�����ף����棩��ȱ�ϡ�J/H��������
		top2fun1(ho_imageCorrected1, &ho_region1, hv_prav2_5, hv_prav2_6, hv_hwnd, &hv_ret1);
		

		(*hv_Result) = 0;
		set_display_font(hv_WindowHandle, 14, "mono", "true", "false");


		if (0 != (hv_ret1<0))
		{
			if (hv_ret1 == -5)
			{
				top2_fun2NgNum = top2_fun2NgNum + 1;
			}
			else
			{
				top2_fun1NgNum = top2_fun1NgNum + 1;
			}
			disp_message(hv_hwnd, "NG �����쳣", "window", 50, 10, "red", "true");
			(*hv_Result) = 5;
		}

		if (0 != ((((hv_ret1 + hv_ret2) + hv_ret3) + hv_ret4) == 0))
		{
			set_display_font(hv_hwnd, 20, "mono", "false", "false");
			disp_message(hv_hwnd, "OK-��Ʒ", "window", 40, 30, "blue", "true");
		}

		set_display_font(hv_hwnd, 11, "mono", "false", "false");
		disp_message(hv_hwnd,
			(((((((((((((((/*argv*/":" + hv_prav2_1) + " ") + hv_prav2_2) + " ") + hv_prav2_3) + " ") + hv_prav2_4) + " ") + hv_prav2_5) + " ") + hv_prav2_6) + " ") + hv_prav2_7) + " ") + hv_prav2_8),
			"window", 0, 80, "black", "false");
		disp_message(hv_hwnd, (((((("-" + top2_fun3NgNum) + "����") + top2_fun2NgNum) + "����") + top2_fun1NgNum) + "-") + top2_fun4NgNum,
			"window", 20, 0, "black", "true");

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
	HObject  ho_ImageReduced1, ho_Regions1, ho_ConnectedRegions2;
	HObject  ho_SelectedRegions2, ho_RegionUnion, ho_RegionClosing2;
	HObject  ho_RegionOpening3, ho_RectangleAllPian, ho_ImageReduced;
	HObject  ho_Regions, ho_RegionFillUp3, ho_RegionOpening;
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
	HTuple  hv_w, hv_h, hv_PianColumn2, hv_PianWidth, hv_Number;
	HTuple  hv_caoHeight, hv_caoHeightsTuple, hv_Index, hv_Area;
	HTuple  hv_Row1, hv_Column1, hv_Phi, hv_Length1, hv_Length2;
	HTuple  hv_PointOrder, hv_caoHeightValues, hv_caoHeightValuesMax;
	HTuple  hv_caoHeightValuesMin, hv_caoHeightValuesMean, hv_addoffset;
	HTuple  hv_caoHeightValuesMax_mm, hv_caoHeightValuesMin_mm;
	HTuple  hv_caoHeightValuesMean_mm, hv_len1Value, hv_len1Valuesz;
	HTuple  hv_errorRgnArea;



	(*hv_ret1) = 0;
	//��Ƭ�߶�,�����жϲ۵�������
	hv_chipHeight = 285;
	hv_chipWidth0 = 1050;
	 
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
	Threshold(ho_ImageReduced1, &ho_Regions1, 140, 255);
	Connection(ho_Regions1, &ho_ConnectedRegions2);
	SelectShape(ho_ConnectedRegions2, &ho_SelectedRegions2, "area", "and", 70000, 1000000);
	Union1(ho_SelectedRegions2, &ho_RegionUnion);
	ClosingRectangle1(ho_RegionUnion, &ho_RegionClosing2, 20, 150);
	FillUp(ho_RegionClosing2, &ho_RegionClosing2);
	OpeningRectangle1(ho_RegionClosing2, &ho_RegionOpening3, 500, 1500);
	RegionFeatures(ho_RegionOpening3, "column2", &hv_PianColumn2);
	RegionFeatures(ho_RegionOpening3, "width", &hv_PianWidth);

	if (0 != (HTuple(HTuple(HTuple(0 == hv_PianColumn2).TupleOr(hv_PianColumn2>1900)).TupleOr(hv_PianWidth<(1030 - 60))).TupleOr(hv_PianWidth>(1030 + 40))))
	{
		disp_message(hv_hwnd, "sidefun1 ������ȡƬ��column1 error", "window", 900 / hv_h,
			30, "red", "false");
		(*hv_ret1) = -1;
		return;
	}

	GenRectangle1(&ho_RectangleAllPian, 0, hv_PianColumn2 - 925, hv_Height1 - (hv_chipHeight / 2),
		hv_PianColumn2);
	ReduceDomain(ho_Image, ho_RectangleAllPian, &ho_ImageReduced);
	Threshold(ho_ImageReduced, &ho_Regions, 0, 150);
	FillUp(ho_Regions, &ho_RegionFillUp3);
	OpeningRectangle1(ho_RegionFillUp3, &ho_RegionOpening, 50, 10);
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
		//�Կ������,��Ե������
		hv_addoffset = 0;
		hv_caoHeightValuesMax_mm = (hv_caoHeightValuesMax - hv_addoffset)*0.00982;
		hv_caoHeightValuesMin_mm = (hv_caoHeightValuesMin - hv_addoffset)*0.00982;
		hv_caoHeightValuesMean_mm = (hv_caoHeightValuesMean - hv_addoffset)*0.00982;


		hv_len1Value = hv_caoHeightValuesMean_mm;

		//�жϿ��
		hv_len1Valuesz = hv_len1Value.TupleString(".2");
		if (0 != (HTuple(hv_len1Value<hv_CaoWidthMin).TupleOr(hv_len1Value>hv_CaoWidthMax)))
		{
			//�ڲۿ��ʱ�������±�Ե�а�ɫ����ʱ����������
			disp_message(hv_hwnd, (((hv_Index - 1) + " �ۿ�") + hv_len1Valuesz) + "mm", "window",
				(hv_Row + 60) / hv_h, hv_Column / hv_w, "red", "true");
			ConcatObj(ho_EmptyRegion, ho_Rectangle1, &ho_EmptyRegion);
		}
		else if (0 != 1)
		{
			disp_message(hv_hwnd, (((hv_Index - 1) + " �ۿ�") + hv_len1Valuesz) + "mm", "window",
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
	HTuple hv_YiWuArea, HTuple hv_YiWuHeight, HTuple *hv_ret2)
{

	// Local iconic variables
	HObject  ho_EmptyRegion, ho_RectangleLeft, ho_RectangleMiddle;
	HObject  ho_RectangleRight, ho_ImageReducedLeft, ho_RegionsBlackLeft;
	HObject  ho_ConnectedRegions4, ho_SelectedRegions2, ho_RegionFillUp;
	HObject  ho_RegionOpening1, ho_RectangleLeft_L, ho_RectangleLeft_R;
	HObject  ho_ConnectedRegions, ho_ObjectSelected, ho_RegionDilation;
	HObject  ho_ImageReduced2, ho_ObjectSelected_, ho__RectangleLeft;
	HObject  ho_RegionLeft, ho_RegionErosionL, ho__RegionIntersection_L;
	HObject  ho_ImageReducedL, ho_RegionsLLight, ho__RegionIntersection_R;
	HObject  ho_ImageReducedR, ho_RegionsLBlack, ho_RegionsLBlackConnected;
	HObject  ho_SelectedRegionsLBlack, ho_SelectedRegionsLBlackU1;
	HObject  ho_RegionMoved, ho_RegionDifference, ho_ImageReduced1;
	HObject  ho_Regions1, ho__RegionMiddle, ho_ImageReduced;
	HObject  ho_Regions, ho_ConnectedRegions1, ho_SelectedRegions3;
	HObject  ho_RegionUnion, ho_RegionClosing1, ho_ConnectedRegions5;
	HObject  ho_SelectedRegions, ho_Regions_, ho_ConnectedRegions2;
	HObject  ho_SelectedRegionsLight, ho_RegionDilation2, ho_RegionClosing2;
	HObject  ho_ConnectedRegions5_, ho_SelectedRegions_, ho_ObjectSelected1;
	HObject  ho_ObjectSelected1Opened, ho_ConnectedRegions3;
	HObject  ho_ImageReduced5, ho_Regions4, ho_ConnectedRegions6;
	HObject  ho_RegionRight_, ho_ImageReduced3, ho_Regions2;
	HObject  ho_ConnectedRegions7, ho_SelectedRegions4, ho_RegionUnion1;
	HObject  ho_RegionClosing3, ho_SelectedRegions1, ho_ObjectSelected2;
	HObject  ho_ImageReduced4, ho_Regions3, ho_RegionDilation1;

	// Local control variables
	HTuple  hv_heightThreshold, hv_Row, hv_Column;
	HTuple  hv_Width, hv_Height, hv_Width1, hv_Height1, hv_w;
	HTuple  hv_h, hv_inRegion2Column2, hv_Value, hv_LeftGuoDuColumn;
	HTuple  hv_debugMark, hv_caoMidGrayValues, hv_Index, hv_ObjectSelectedcolumn1;
	HTuple  hv_ObjectSelected_Infos, hv__RectangleLeftMean;
	HTuple  hv_Deviation2, hv_MeanLL, hv_DeviationLL, hv_RegionsLLLightArea;
	HTuple  hv__RegionIntersection_RMean, hv_Deviation3, hv_SelectedRegionsLBlackarea;
	HTuple  hv__RegionIntersection_RC1W, hv_SelectedRegionsLBlackU1C1W;
	HTuple  hv_Value3, hv_Mean, hv_Deviation, hv_LightArea;
	HTuple  hv__RegionMiddleH, hv_Number, hv_Index1, hv_ObjectSelected1W;
	HTuple  hv_ObjectSelected1Height, hv_ObjectSelected1Area;
	HTuple  hv_ObjectSelected1IR, hv_ConnectedRegions3H, hv_Max1;
	HTuple  hv_addthreshold, hv_Value1, hv_Value2, hv_Max, hv_Mean1;
	HTuple  hv_Deviation1, hv_thresholdSUBValues, hv_ObjectSelected2Area;
	HTuple  hv_ObjectSelected2Height, hv_caoMidGrayValuesMean;
	HTuple  hv_errorRgnArea;


	(*hv_ret2) = 0;
	 
		SetColor(hv_hwnd, "green");
	hv_heightThreshold = (hv_YiWuHeight / 0.00833) - 10;

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

	RegionFeatures(ho_inRegion2, "column2", &hv_inRegion2Column2);
	GenRectangle1(&ho_RectangleLeft, 0, hv_inRegion2Column2 - 900, hv_Height1, hv_inRegion2Column2 - 760);

	GenRectangle1(&ho_RectangleMiddle, 0, hv_inRegion2Column2 - 780, hv_Height1, hv_inRegion2Column2 - 220);

	GenRectangle1(&ho_RectangleRight, 0, hv_inRegion2Column2 - 240, hv_Height1, hv_inRegion2Column2 - 10);

	//�ҵ��۶���б�´�������
	ReduceDomain(ho_Image, ho_RectangleLeft, &ho_ImageReducedLeft);
	Threshold(ho_ImageReducedLeft, &ho_RegionsBlackLeft, 0, 25);
	Connection(ho_RegionsBlackLeft, &ho_ConnectedRegions4);
	SelectShape(ho_ConnectedRegions4, &ho_SelectedRegions2, "area", "and", 3125.88,
		50000);
	FillUp(ho_SelectedRegions2, &ho_RegionFillUp);
	OpeningCircle(ho_RegionFillUp, &ho_RegionOpening1, 10);
	RegionFeatures(ho_RegionOpening1, "column2", &hv_Value);
	TupleMean(hv_Value, &hv_LeftGuoDuColumn);
	GenRectangle1(&ho_RectangleLeft_L, 0, hv_inRegion2Column2 - 900, hv_Height1, hv_LeftGuoDuColumn);
	GenRectangle1(&ho_RectangleLeft_R, 0, hv_inRegion2Column2 - 810, hv_Height1, hv_inRegion2Column2 - 760);

	Connection(ho_inRegion2, &ho_ConnectedRegions);

	hv_debugMark = 0;

	hv_caoMidGrayValues = HTuple();

	for (hv_Index = 1; hv_Index <= 22; hv_Index += 1)
	{
		SelectObj(ho_ConnectedRegions, &ho_ObjectSelected, hv_Index);
		RegionFeatures(ho_ObjectSelected, "column1", &hv_ObjectSelectedcolumn1);
		DilationRectangle1(ho_ObjectSelected, &ho_RegionDilation, 0.5, 20);
		ReduceDomain(ho_Image, ho_RegionDilation, &ho_ImageReduced2);
		ErosionRectangle1(ho_ObjectSelected, &ho_ObjectSelected_, 1, 25);
		//�������²۴�ȱ��
		RegionFeatures(ho_ObjectSelected_, ((HTuple("column1").Append("row1")).Append("row2")),
			&hv_ObjectSelected_Infos);
		GenRectangle1(&ho__RectangleLeft, HTuple(hv_ObjectSelected_Infos[1]), HTuple(hv_ObjectSelected_Infos[0]) - 70,
			HTuple(hv_ObjectSelected_Infos[2]), HTuple(hv_ObjectSelected_Infos[0]));
		Intensity(ho__RectangleLeft, ho_Image, &hv__RectangleLeftMean, &hv_Deviation2);
		if (0 != (HTuple(hv__RectangleLeftMean<40).TupleOr(hv__RectangleLeftMean>150)))
		{
			if (0 != (hv_debugMark == 1))
			{
				 
					DispObj(ho_Image, hv_hwnd);
				 
					DispObj(ho__RectangleLeft, hv_hwnd);
				// stop(...); only in hdevelop
			}
			ConcatObj(ho_EmptyRegion, ho__RectangleLeft, &ho_EmptyRegion);
		}

		//���������
		Intersection(ho_ObjectSelected_, ho_RectangleLeft, &ho_RegionLeft);
		ErosionRectangle1(ho_RegionLeft, &ho_RegionErosionL, 1, 5);
		//��������а���
		Intersection(ho_RegionErosionL, ho_RectangleLeft_L, &ho__RegionIntersection_L
		);
		ReduceDomain(ho_Image, ho__RegionIntersection_L, &ho_ImageReducedL);
		Intensity(ho__RegionIntersection_L, ho_ImageReducedL, &hv_MeanLL, &hv_DeviationLL);
		if (0 != (hv_MeanLL>30))
		{
			if (0 != (hv_debugMark == 1))
			{
				 
					DispObj(ho_Image, hv_hwnd);
				 
					DispObj(ho__RegionIntersection_L, hv_hwnd);
				// stop(...); only in hdevelop
			}
			ConcatObj(ho_EmptyRegion, ho__RegionIntersection_L, &ho_EmptyRegion);
		}
		Threshold(ho_ImageReducedL, &ho_RegionsLLight, 60, 255);
		RegionFeatures(ho_RegionsLLight, "area", &hv_RegionsLLLightArea);
		if (0 != (hv_RegionsLLLightArea>500))
		{
			if (0 != (hv_debugMark == 1))
			{
				 
					DispObj(ho_Image, hv_hwnd);
				 
					DispObj(ho_RegionsLLight, hv_hwnd);
				// stop(...); only in hdevelop
			}
			ConcatObj(ho_EmptyRegion, ho_RegionsLLight, &ho_EmptyRegion);
		}
		//�Ҳ������к���
		Intersection(ho_RegionErosionL, ho_RectangleLeft_R, &ho__RegionIntersection_R
		);
		ReduceDomain(ho_Image, ho__RegionIntersection_R, &ho_ImageReducedR);
		Intensity(ho__RegionIntersection_R, ho_ImageReducedR, &hv__RegionIntersection_RMean,
			&hv_Deviation3);

		if (0 != (hv__RegionIntersection_RMean <= 30))
		{
			disp_message(hv_hwnd, "sidefun2 ������Ҳ�Ҷȵ�", "window", 1000 / hv_h, 30,
				"red", "true");
			if (0 != (hv_debugMark == 1))
			{
				// stop(...); only in hdevelop
			}
			ConcatObj(ho_EmptyRegion, ho__RegionIntersection_R, &ho_EmptyRegion);
			hv__RegionIntersection_RMean = 30;
		}

		Threshold(ho_ImageReducedR, &ho_RegionsLBlack, 0, 35);
		Connection(ho_RegionsLBlack, &ho_RegionsLBlackConnected);
		SelectShape(ho_RegionsLBlackConnected, &ho_SelectedRegionsLBlack, (HTuple("height").Append("area")),
			"and", hv_YiWuHeight.TupleConcat(57.8), (HTuple(200).Append(10000)));
		Union1(ho_SelectedRegionsLBlack, &ho_SelectedRegionsLBlackU1);
		RegionFeatures(ho_SelectedRegionsLBlackU1, "area", &hv_SelectedRegionsLBlackarea);
		if (0 != (hv_SelectedRegionsLBlackarea>400))
		{
			//�ڱ���������Ե��������б���������죬������Ϳ�Ƚ�Сʱ�ų�
			RegionFeatures(ho__RegionIntersection_R, (HTuple("column1").Append("width")),
				&hv__RegionIntersection_RC1W);
			RegionFeatures(ho_SelectedRegionsLBlackU1, (HTuple("column1").Append("width")),
				&hv_SelectedRegionsLBlackU1C1W);
			if (0 != (HTuple(HTuple(HTuple(hv__RegionIntersection_RC1W[0]) == HTuple(hv_SelectedRegionsLBlackU1C1W[0])).TupleAnd(HTuple(hv__RegionIntersection_RC1W[1])>(HTuple(hv_SelectedRegionsLBlackU1C1W[1]) + 10))).TupleAnd(hv_SelectedRegionsLBlackarea<600)))
			{
				//ȷ���������ұ������Ƿ�������������Ϊ�������Ϊб����������
				MoveRegion(ho__RegionIntersection_R, &ho_RegionMoved, 0, -20);
				Difference(ho_RegionMoved, ho__RegionIntersection_R, &ho_RegionDifference
				);
				ReduceDomain(ho_Image, ho_RegionDifference, &ho_ImageReduced1);
				Threshold(ho_ImageReduced1, &ho_Regions1, 50, 255);
				RegionFeatures(ho_Regions1, "area", &hv_Value3);
				if (0 != (hv_Value3>500))
				{
					if (0 != (hv_debugMark == 1))
					{
						 
							DispObj(ho_Image, hv_hwnd);
						 
							DispObj(ho_SelectedRegionsLBlackU1, hv_hwnd);
						// stop(...); only in hdevelop
					}
					ConcatObj(ho_EmptyRegion, ho_SelectedRegionsLBlackU1, &ho_EmptyRegion);
				}
			}
			else
			{
				if (0 != (hv_debugMark == 1))
				{
					 
						DispObj(ho_Image, hv_hwnd);
					 
						DispObj(ho_SelectedRegionsLBlackU1, hv_hwnd);
					// stop(...); only in hdevelop
				}
				ConcatObj(ho_EmptyRegion, ho_SelectedRegionsLBlackU1, &ho_EmptyRegion);
			}
		}

		//�м��������
		Intersection(ho_ObjectSelected_, ho_RectangleMiddle, &ho__RegionMiddle);
		ReduceDomain(ho_Image, ho__RegionMiddle, &ho_ImageReduced);
		Intensity(ho__RegionMiddle, ho_ImageReduced, &hv_Mean, &hv_Deviation);
		hv_caoMidGrayValues = hv_caoMidGrayValues.TupleConcat(hv_Mean);
		if (0 != (hv_Mean <= 40))
		{
			disp_message(hv_hwnd, "sidefun2 ���жλҶȵ�", "window", 1000 / hv_h, 30, "red",
				"true");
			if (0 != (hv_debugMark == 1))
			{
				// stop(...); only in hdevelop
			}
			ConcatObj(ho_EmptyRegion, ho__RegionMiddle, &ho_EmptyRegion);
			hv_Mean = 40;
		}
		Threshold(ho_ImageReduced, &ho_Regions, 0, hv_Mean - 40);
		Connection(ho_Regions, &ho_ConnectedRegions1);
		SelectShape(ho_ConnectedRegions1, &ho_SelectedRegions3, "area", "and", 15, 50000);
		Union1(ho_SelectedRegions3, &ho_RegionUnion);
		ClosingCircle(ho_RegionUnion, &ho_RegionClosing1, 3.5);
		Connection(ho_RegionClosing1, &ho_ConnectedRegions5);
		SelectShape(ho_ConnectedRegions5, &ho_SelectedRegions, "height", "and", hv_YiWuHeight,
			200);
		//���ֲ��ڵ�ľ�ۻҶȺ��޵�ľ����ͬ��ֻ������ɫ��Ե������µ���ȡ
		//�ҶȲ����͵��߶Ⱥܸ�
		Threshold(ho_ImageReduced, &ho_Regions, 0, hv_Mean - 30);
		//��ȡ��������
		Threshold(ho_ImageReduced, &ho_Regions_, 200, 255);
		Connection(ho_Regions_, &ho_ConnectedRegions2);
		SelectShapeStd(ho_ConnectedRegions2, &ho_SelectedRegionsLight, "max_area", 70);
		RegionFeatures(ho_SelectedRegionsLight, "area", &hv_LightArea);
		if (0 != (hv_LightArea>80))
		{
			DilationCircle(ho_SelectedRegionsLight, &ho_RegionDilation2, 10);
			Union2(ho_Regions, ho_RegionDilation2, &ho_Regions);
		}

		Connection(ho_Regions, &ho_ConnectedRegions1);
		SelectShape(ho_ConnectedRegions1, &ho_SelectedRegions3, "area", "and", 15, 50000);
		Union1(ho_SelectedRegions3, &ho_RegionUnion);
		ClosingRectangle1(ho_RegionUnion, &ho_RegionClosing2, 2, 5);
		Connection(ho_RegionClosing2, &ho_ConnectedRegions5_);
		RegionFeatures(ho__RegionMiddle, "height", &hv__RegionMiddleH);
		SelectShape(ho_ConnectedRegions5_, &ho_SelectedRegions_, "height", "and", hv__RegionMiddleH - 5,
			200);
		ConcatObj(ho_SelectedRegions, ho_SelectedRegions_, &ho_SelectedRegions);

		CountObj(ho_SelectedRegions, &hv_Number);
		{
			HTuple end_val180 = hv_Number;
			HTuple step_val180 = 1;
			for (hv_Index1 = 1; hv_Index1.Continue(end_val180, step_val180); hv_Index1 += step_val180)
			{
				SelectObj(ho_SelectedRegions, &ho_ObjectSelected1, hv_Index1);
				RegionFeatures(ho_ObjectSelected1, "width", &hv_ObjectSelected1W);
				RegionFeatures(ho_ObjectSelected1, "height", &hv_ObjectSelected1Height);
				RegionFeatures(ho_ObjectSelected1, "area", &hv_ObjectSelected1Area);
				RegionFeatures(ho_ObjectSelected1, "inner_radius", &hv_ObjectSelected1IR);
				//��ȴ�ʱ����ȥ��Ե��Ӱ��
				if (0 != (HTuple(hv_ObjectSelected1W>40).TupleAnd(hv_ObjectSelected1IR>8)))
				{
					OpeningCircle(ho_ObjectSelected1, &ho_ObjectSelected1Opened, 2);
					Connection(ho_ObjectSelected1Opened, &ho_ConnectedRegions3);
					RegionFeatures(ho_ConnectedRegions3, "height", &hv_ConnectedRegions3H);
					TupleMax(hv_ConnectedRegions3H, &hv_Max1);
					SelectShape(ho_ConnectedRegions3, &ho_ObjectSelected1, "height", "and", hv_Max1 - 1,
						150);
				}
				//�������߶�ֵ���ų���ֵ����
				hv_addthreshold = 0;
				if (0 != (hv_ObjectSelected1Height>40))
				{
					hv_addthreshold = 15;
					if (0 != (hv_ObjectSelected1IR>8))
					{
						hv_addthreshold = 8;
					}
				}
				if (0 != (hv_Mean>90))
				{
					hv_addthreshold = (hv_addthreshold + hv_Mean) - 90;
				}
				if (0 != (HTuple(hv_ObjectSelected1Area>hv_YiWuArea).TupleOr(hv_ObjectSelected1Height>hv_YiWuHeight)))
				{
					ReduceDomain(ho_ImageReduced, ho_ObjectSelected1, &ho_ImageReduced5);

					Threshold(ho_ImageReduced5, &ho_Regions4, 0, 20 + hv_addthreshold);
					RegionFeatures(ho_Regions4, "area", &hv_Value1);
					if (0 != (hv_Value1<150))
					{
						continue;
					}
					Connection(ho_Regions4, &ho_ConnectedRegions6);
					RegionFeatures(ho_ConnectedRegions6, "area", &hv_Value2);
					TupleMax(hv_Value2, &hv_Max);
					if (0 != (hv_Max<40))
					{
						continue;
					}
					if (0 != (hv_debugMark == 1))
					{
						 
							DispObj(ho_Image, hv_hwnd);
						 
							DispObj(ho_ObjectSelected1, hv_hwnd);
						// stop(...); only in hdevelop
					}
					ConcatObj(ho_EmptyRegion, ho_ObjectSelected1, &ho_EmptyRegion);
				}
			}
		}

		//�Ҷ�������
		Intersection(ho_ObjectSelected_, ho_RectangleRight, &ho_RegionRight_);
		ReduceDomain(ho_Image, ho_RegionRight_, &ho_ImageReduced3);
		Intensity(ho_RegionRight_, ho_ImageReduced3, &hv_Mean1, &hv_Deviation1);
		hv_thresholdSUBValues = 40;
		if (0 != (hv_Mean1 <= 40))
		{
			if (0 != (HTuple(hv_Mean1>30).TupleAnd(hv_Deviation1<18)))
			{
				 
					DispObj(ho_RegionRight_, hv_hwnd);
				hv_thresholdSUBValues = 25;
			}
			else
			{
				disp_message(hv_hwnd, "sidefun2 ���ҶλҶȵ�", "window", 1000 / hv_h, 30, "red",
					"true");
				if (0 != (hv_debugMark == 1))
				{
					// stop(...); only in hdevelop
				}
				ConcatObj(ho_EmptyRegion, ho_RegionRight_, &ho_EmptyRegion);
				hv_Mean1 = 40;
			}
		}
		else if (0 != (hv_Mean1>(hv_Mean + 3)))
		{
			if (0 != (hv_Deviation1>20))
			{
				//���ҶλҶȴ����ж���ƫ���ʱ�������е�ľ�ۣ������ȡ��ֵ������ȫ��ȡ��Ե����
				hv_thresholdSUBValues = 30;
			}
		}
		if (0 != (hv_Deviation1>20))
		{
			 
				DispObj(ho_RegionRight_, hv_hwnd);
			hv_thresholdSUBValues = hv_thresholdSUBValues - 5;
			if (0 != (hv_Mean1<55))
			{
				hv_thresholdSUBValues = hv_thresholdSUBValues - 5;
			}
		}
		Threshold(ho_ImageReduced3, &ho_Regions2, 0, hv_Mean1 - hv_thresholdSUBValues);
		//��ȡ��������
		Threshold(ho_ImageReduced3, &ho_Regions_, 200, 255);
		Connection(ho_Regions_, &ho_ConnectedRegions2);
		SelectShapeStd(ho_ConnectedRegions2, &ho_SelectedRegionsLight, "max_area", 70);
		RegionFeatures(ho_SelectedRegionsLight, "area", &hv_LightArea);
		if (0 != (hv_LightArea>80))
		{
			DilationCircle(ho_SelectedRegionsLight, &ho_RegionDilation2, 10);
			Union2(ho_Regions2, ho_RegionDilation2, &ho_Regions2);
		}

		Connection(ho_Regions2, &ho_ConnectedRegions7);
		SelectShape(ho_ConnectedRegions7, &ho_SelectedRegions4, "area", "and", 15, 20000);
		Union1(ho_SelectedRegions4, &ho_RegionUnion1);
		ClosingCircle(ho_RegionUnion1, &ho_RegionClosing3, 3.5);
		Connection(ho_RegionClosing3, &ho_ConnectedRegions2);
		SelectShape(ho_ConnectedRegions2, &ho_SelectedRegions1, "height", "and", hv_YiWuHeight,
			200);
		CountObj(ho_SelectedRegions1, &hv_Number);
		{
			HTuple end_val276 = hv_Number;
			HTuple step_val276 = 1;
			for (hv_Index1 = 1; hv_Index1.Continue(end_val276, step_val276); hv_Index1 += step_val276)
			{
				SelectObj(ho_SelectedRegions1, &ho_ObjectSelected2, hv_Index1);
				RegionFeatures(ho_ObjectSelected2, "area", &hv_ObjectSelected2Area);
				RegionFeatures(ho_ObjectSelected2, "height", &hv_ObjectSelected2Height);
				hv_addthreshold = 0;
				if (0 != (hv_ObjectSelected2Height>40))
				{
					hv_addthreshold = 10;
				}
				if (0 != (HTuple(hv_ObjectSelected2Area>hv_YiWuArea).TupleOr(hv_ObjectSelected2Height>hv_YiWuHeight)))
				{
					ReduceDomain(ho_ImageReduced3, ho_ObjectSelected2, &ho_ImageReduced4);
					Threshold(ho_ImageReduced4, &ho_Regions3, 0, 20 + hv_addthreshold);
					RegionFeatures(ho_Regions3, "area", &hv_Value1);
					if (0 != (hv_Value1<100))
					{
						continue;
					}
					Connection(ho_Regions3, &ho_ConnectedRegions6);
					RegionFeatures(ho_ConnectedRegions6, "area", &hv_Value2);
					TupleMax(hv_Value2, &hv_Max);
					if (0 != (hv_Max<35))
					{
						continue;
					}
					if (0 != (hv_debugMark == 1))
					{
						 
							DispObj(ho_Image, hv_hwnd);
						 
							DispObj(ho_ObjectSelected2, hv_hwnd);
						// stop(...); only in hdevelop
					}
					ConcatObj(ho_EmptyRegion, ho_ObjectSelected2, &ho_EmptyRegion);
				}
			}
		}
	}

	TupleMean(hv_caoMidGrayValues, &hv_caoMidGrayValuesMean);
	disp_message(hv_hwnd, "�ۻҶȾ�ֵ:" + hv_caoMidGrayValuesMean, "window", 1600 / hv_h,
		0, "magenta", "false");
	disp_message(hv_hwnd, "����ֵ��Χ:65~90", "window", 1680 / hv_h, 0, "magenta", "false");
	Union1(ho_EmptyRegion, &(*ho_region2));
	RegionFeatures((*ho_region2), "area", &hv_errorRgnArea);
	if (0 != (HTuple(hv_errorRgnArea>0).TupleAnd((*hv_ret2) == 0)))
	{
		(*hv_ret2) = -10;
		 
			SetColor(hv_hwnd, "red");
		DilationRectangle1((*ho_region2), &ho_RegionDilation1, 30, 30);
		 
			DispObj(ho_RegionDilation1, hv_hwnd);
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
	HObject  ho_ConnectedRegions2, ho_SelectedRegions1, ho_ObjectSelectedCaoUp;
	HObject  ho_ObjectSelectedCaoMid, ho_ObjectSelectedCaoDown;
	HObject  ho_RegionUnionCaoThree, ho_PianErrDouble;

	// Local control variables
	HTuple  hv_Row, hv_Column, hv_Width, hv_Height;
	HTuple  hv_Width1, hv_Height1, hv_w, hv_h, hv_pianWidths;
	HTuple  hv_Index, hv_pianWidth, hv_Value, hv_Maxarea, hv_Maxwidth;
	HTuple  hv_Value1, hv_Value2, hv_Value3, hv_errorRgnArea;
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
		Threshold(ho_ImageReduced, &ho_Regions, 0, 120);
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


			if (0 != (HTuple(hv_Maxarea<500).TupleAnd(hv_Value<(hv_blackArea + 10000))))
			{
				continue;
			}

			ConcatObj(ho_EmptyRegion, ho_Regions, &ho_EmptyRegion);
		}
	}

	//�ڲۿ�ϸ�������£��۷ֶȼ��
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
	HObject  ho_gouDingSingle, ho_gouDingSingleFirst, ho_RegionErosion;
	HObject  ho_ImageReduced6, ho_RegionIntersection1, ho_Regions;
	HObject  ho_RegionOpening, ho_ConnectedRegions4, ho_SelectedMax;
	HObject  ho_Rectangle, ho_Regions_, ho_RegionOpening2, ho_ConnectedRegions2;
	HObject  ho_SelectedRegions, ho_RegionOpening1, ho_RegionMoved0;
	HObject  ho_RegionIntersection, ho_ConnectedRegions1, ho_RegionUnion;
	HObject  ho_ConnectedRegions, ho_ObjectSelected, ho_ObjectSelectedNext;
	HObject  ho_RegionMoved, ho_RegionDilation, ho_ImageReduced;
	HObject  ho_Regions1, ho_RegionClosing, ho_RegionOpening3;

	// Local control variables
	HTuple  hv_Row, hv_Column, hv_Width, hv_Height;
	HTuple  hv_Width1, hv_Height1, hv_w, hv_h, hv_ChipColumn2;
	HTuple  hv_gouMin, hv_gouMax, hv_gouMiddle, hv_gouPianDif;
	HTuple  hv_Index0, hv_chipValueSTD, hv_MaxHlast, hv_heightUplimit;
	HTuple  hv_thresholdNum46, hv_Value1, hv_MaxH, hv_Indices;
	HTuple  hv_SelectedMaxInfos, hv_Value, hv_Number2, hv_SelectedRegionsC2;
	HTuple  hv_MaxC2, hv_GouMaxInfos, hv_openRectW, hv_RegionOpening1Height;
	HTuple  hv_RegionOpening1Area, hv_columnMovedDistance, hv_Number1;
	HTuple  hv_SingleGouDingArea, hv_Number, hv_GouDingColumn2Values;
	HTuple  hv_GouHeightsValues, hv_Mean, hv_isdebug, hv_Index;
	HTuple  hv_Selected, hv_Selected1, hv_difNum, hv_CorrectGouInfos;
	HTuple  hv_nextGouHeight, hv_Abs, hv_errorRgnArea;


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
	hv_gouMin = hv_ChipColumn2 - 1050;
	hv_gouMax = (hv_ChipColumn2 - 1050) + 90;
	GenRectangle1(&ho_RectangleSTD, 0, hv_gouMin, hv_Height1, hv_gouMax);
	hv_gouMiddle = hv_ChipColumn2 - 970;
	GenRegionLine(&ho_RegionLinesSTD, 0, hv_gouMiddle, hv_Height1, hv_gouMiddle);
	 
		DispObj(ho_RegionLinesSTD, hv_hwnd);

	MoveRegion(ho_inRegion4, &ho_inRegion4Moved, 0, -150);
	Connection(ho_inRegion4Moved, &ho_inRegion4ConnectedRegions);

	GenEmptyRegion(&ho_GouDing);
	hv_gouPianDif = HTuple();


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
		GenEmptyRegion(&ho_gouDingSingle);
		GenEmptyRegion(&ho_gouDingSingleFirst);
		ErosionRectangle1(ho_RegionIntersectionSTD, &ho_RegionErosion, 0.5, 140);
		ReduceDomain(ho_Image, ho_RegionErosion, &ho_ImageReduced6);

		GenEmptyRegion(&ho_RegionIntersection1);
		hv_MaxHlast = 0;
		hv_heightUplimit = 100;
		for (hv_thresholdNum46 = 60; hv_thresholdNum46 <= 140; hv_thresholdNum46 += 10)
		{
			Threshold(ho_ImageReduced6, &ho_Regions, 0, hv_thresholdNum46);
			OpeningCircle(ho_Regions, &ho_RegionOpening, 3);
			Connection(ho_RegionOpening, &ho_ConnectedRegions4);
			RegionFeatures(ho_ConnectedRegions4, "height", &hv_Value1);
			TupleMax(hv_Value1, &hv_MaxH);

			if (0 != (HTuple(hv_MaxH >= hv_heightUplimit).TupleAnd(hv_MaxHlast <= 75)))
			{
				Threshold(ho_ImageReduced6, &ho_Regions, 0, hv_thresholdNum46 - 5);
				OpeningCircle(ho_Regions, &ho_RegionOpening, 5);
				Connection(ho_RegionOpening, &ho_ConnectedRegions4);
				RegionFeatures(ho_ConnectedRegions4, "height", &hv_Value1);
				TupleMax(hv_Value1, &hv_MaxH);
				if (0 != (HTuple(hv_MaxH >= hv_heightUplimit).TupleAnd(hv_MaxH <= 108)))
				{
					hv_heightUplimit = 108;
				}
			}
			hv_MaxHlast = hv_MaxH;
			if (0 != (HTuple(hv_MaxH<hv_heightUplimit).TupleAnd(hv_MaxH>75)))
			{
				TupleFind(hv_Value1, hv_MaxH, &hv_Indices);
				SelectObj(ho_ConnectedRegions4, &ho_SelectedMax, hv_Indices + 1);
				RegionFeatures(ho_SelectedMax, (HTuple("row1").Append("row2")), &hv_SelectedMaxInfos);
				GenRectangle1(&ho_Rectangle, HTuple(hv_SelectedMaxInfos[0]), hv_gouMin, HTuple(hv_SelectedMaxInfos[1]),
					hv_gouMax);
				Intersection(ho_Rectangle, ho_Regions, &ho_RegionIntersection1);
				break;
			}
		}

		ClosingRectangle1(ho_RegionIntersection1, &ho_Regions_, 2, 30);
		OpeningRectangle1(ho_Regions_, &ho_RegionOpening2, 5, 20);
		RegionFeatures(ho_RegionOpening2, "height", &hv_Value);

		Connection(ho_RegionOpening2, &ho_ConnectedRegions2);
		SelectShape(ho_ConnectedRegions2, &ho_SelectedRegions, "height", "and", 60.459,
			200);
		CountObj(ho_SelectedRegions, &hv_Number2);
		if (0 != (hv_Number2 == 0))
		{
			disp_message(hv_hwnd, "sidefun4 �������޹�", "window", 1200 / hv_h, 30, "red",
				"true");
			(*hv_ret4) = -1;
			//concat_obj (RegionErosion, EmptyRegion, EmptyRegion)
			 
				SetColor(hv_hwnd, "red");
			 
				DispObj(ho_RegionErosion, hv_hwnd);
			return;
		}
		//ѡ���ұߵ�һ������
		if (0 != (hv_Number2>1))
		{
			RegionFeatures(ho_SelectedRegions, "column2", &hv_SelectedRegionsC2);
			TupleMax(hv_SelectedRegionsC2, &hv_MaxC2);
			SelectShape(ho_SelectedRegions, &ho_SelectedRegions, "column2", "and", hv_MaxC2 - 1,
				hv_MaxC2 + 1);
		}

		RegionFeatures(ho_SelectedRegions, (((HTuple("area").Append("height")).Append("width")).Append("inner_radius")),
			&hv_GouMaxInfos);
		//��խ�������
		hv_openRectW = 7;
		OpeningRectangle1(ho_SelectedRegions, &ho_RegionOpening1, hv_openRectW, 50);
		RegionFeatures(ho_RegionOpening1, "height", &hv_RegionOpening1Height);
		RegionFeatures(ho_RegionOpening1, "area", &hv_RegionOpening1Area);
		if (0 != (HTuple((HTuple(hv_GouMaxInfos[1]) - hv_RegionOpening1Height)>20).TupleOr(hv_RegionOpening1Area<650)))
		{
			if (0 != (hv_RegionOpening1Height == 0))
			{
				hv_openRectW = 5;
			}
			ShapeTrans(ho_SelectedRegions, &ho_SelectedRegions, "rectangle1");
		}

		for (hv_columnMovedDistance = 70; hv_columnMovedDistance >= -1; hv_columnMovedDistance += -10)
		{
			MoveRegion(ho_RegionErosion, &ho_RegionMoved0, 0, hv_columnMovedDistance);
			Intersection(ho_SelectedRegions, ho_RegionMoved0, &ho_RegionIntersection);
			OpeningRectangle1(ho_RegionIntersection, &ho_RegionOpening1, hv_openRectW,
				50);
			RegionFeatures(ho_RegionOpening1, "height", &hv_RegionOpening1Height);
			if (0 != (HTuple(hv_RegionOpening1Height>60).TupleAnd(hv_RegionOpening1Height<105)))
			{
				//��������ʱ�ϲ�
				Connection(ho_RegionOpening1, &ho_ConnectedRegions1);
				CountObj(ho_ConnectedRegions1, &hv_Number1);
				if (0 != (hv_Number1>1))
				{
					ClosingRectangle1(ho_RegionOpening1, &ho_RegionOpening1, 60, 1);
				}
				Union1(ho_RegionOpening1, &ho_gouDingSingle);
				break;
			}
		}


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
			 
				DispObj(ho_gouDingSingle, hv_hwnd);
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

	//���߶��ж�
	RegionFeatures(ho_ConnectedRegions, "column2", &hv_GouDingColumn2Values);
	TupleSub(hv_ChipColumn2, hv_GouDingColumn2Values, &hv_GouHeightsValues);
	TupleMean(hv_GouHeightsValues, &hv_Mean);
	if (0 != (HTuple(hv_Mean>1020).TupleOr(hv_Mean<970)))
	{
		disp_message(hv_hwnd, "sidefun4 ���߶Ⱦ�ֵ�쳣", "window", 1200 / hv_h, 30, "red",
			"true");
		(*hv_ret4) = -3;
		//concat_obj (RegionErosion, EmptyRegion, EmptyRegion)
		 
			SetColor(hv_hwnd, "red");
		 
			DispObj(ho_GouDing, hv_hwnd);
		return;
	}

	hv_isdebug = 0;

	for (hv_Index = 0; hv_Index <= 21; hv_Index += 1)
	{
		TupleSelect(hv_GouHeightsValues, hv_Index, &hv_Selected);
		//�ж�
		if (0 != (hv_Selected>((hv_Mean + hv_input_Scale_1Goupian) + 5)))
		{
			if (0 != (hv_isdebug == 1))
			{
				// stop(...); only in hdevelop
			}
			SelectObj(ho_ConnectedRegions, &ho_ObjectSelected, hv_Index + 1);
			ConcatObj(ho_ObjectSelected, ho_EmptyRegion, &ho_EmptyRegion);
		}
		else if (0 != (hv_Selected<((hv_Mean - hv_input_Scale_1Goupian) - 5)))
		{
			if (0 != (hv_isdebug == 1))
			{
				// stop(...); only in hdevelop
			}
			SelectObj(ho_ConnectedRegions, &ho_ObjectSelected, hv_Index + 1);
			ConcatObj(ho_ObjectSelected, ho_EmptyRegion, &ho_EmptyRegion);
		}

		TupleSelect(hv_GouHeightsValues, (hv_Index + 1) % 22, &hv_Selected1);
		hv_difNum = hv_Selected1 - hv_Selected;
		//��ǰ������һ����35+����������һ���������и߻Ҷȣ�������ȡ���󣬽�����
		if (0 != (hv_difNum>35))
		{
			SelectObj(ho_ConnectedRegions, &ho_ObjectSelectedNext, ((hv_Index + 1) % 22) + 1);
			MoveRegion(ho_ObjectSelectedNext, &ho_RegionMoved, 0, hv_difNum);
			DilationCircle(ho_RegionMoved, &ho_RegionDilation, 7);
			ReduceDomain(ho_Image, ho_RegionDilation, &ho_ImageReduced);
			Threshold(ho_ImageReduced, &ho_Regions1, 0, hv_thresholdNum46 + 50);
			ClosingRectangle1(ho_Regions1, &ho_RegionClosing, 1, 10);
			OpeningRectangle1(ho_RegionClosing, &ho_RegionOpening3, 7, 50);
			RegionFeatures(ho_RegionOpening3, ((HTuple("area").Append("height")).Append("column2")),
				&hv_CorrectGouInfos);
			if (0 != (HTuple(HTuple(hv_CorrectGouInfos[0])>800).TupleAnd(HTuple(hv_CorrectGouInfos[1])>70)))
			{
				 
					SetColor(hv_hwnd, "magenta");
				 
					DispObj(ho_RegionOpening3, hv_hwnd);
				ConcatObj(ho_GouDing, ho_RegionOpening3, &ho_GouDing);

				hv_nextGouHeight = hv_ChipColumn2 - HTuple(hv_CorrectGouInfos[2]);
				hv_GouHeightsValues[(hv_Index + 1) % 22] = hv_nextGouHeight;
				hv_difNum = hv_nextGouHeight - hv_Selected;
			}
		}


		TupleAbs(hv_difNum, &hv_Abs);
		if (0 != (hv_Abs>hv_input_Scale_1Goupian))
		{
			if (0 != (hv_isdebug == 1))
			{
				// stop(...); only in hdevelop
			}
			SelectObj(ho_ConnectedRegions, &ho_ObjectSelected, hv_Index + 1);
			ConcatObj(ho_ObjectSelected, ho_EmptyRegion, &ho_EmptyRegion);
			SelectObj(ho_ConnectedRegions, &ho_ObjectSelected, ((hv_Index + 1) % 22) + 1);
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
		//Ƭ��Ȳ��ȣ�ͨ��ʵ�ʿ�ȸ�ʴ
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

		SideCHH_PingJie(input_Image, &ho_ImagePJ, 1080, 2, 0, 180, &hv_DownRow);
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
			
			//�������������ײ����루���Ҽ���λ�ã�  
			Sidefun4(ho_ImagePJ, ho_outRegion1, &ho_region4, hv_input_Scale_1Goupian,hv_hwnd, &hv_ret4, &hv_ret5);

			//�Թ����еļ�⣨���Ϸ������ɫ��ȣ�
			//Sidefun5(ho_ImagePJ, ho_outRegion1, &ho_region5, hv_WindowHandle, input_Scale_1, &hv_ret5);
		}


		//**�˴��ڶ���ճͭ��������ע��ע�⣺ͬʱ�ж��ֲ���ʱ��������Ϊճͭ
		//**2����λ����ж��ֲ������ʱ�����ӦΪճͭ
		(*hv_Result) = 0;
		set_display_font(hv_WindowHandle, 14, "mono", "true", "false");
		if (0 != (hv_ret1<0))
		{
			(*hv_Result) = 3;
			disp_message(hv_hwnd, "NG �ۿ��", "window", 30, 30, "red", "true");
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


