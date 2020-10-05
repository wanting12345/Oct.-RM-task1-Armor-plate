#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include "armorPlate.h"
#include<algorithm>
using namespace cv;
using namespace std;
//后面对装甲板进行排序的sort函数的cmp函数
bool cmp(RotatedRect a, RotatedRect b)
{
	return a.size.area() > b.size.area();
}
bool capture_IsOpened(VideoCapture cap)
{
	if (!cap.isOpened())
	{
		cout << "無法播放視頻" << endl;
		return -1;
	}
}
void ArmorPlate::image_process()
{
		//轉化到HSV空間
	cvtColor(frame, gray, COLOR_BGR2HSV);
	inRange(gray, Scalar(80,40,46), Scalar(124,255,255), hsv_image);
	//开操作
	Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
	morphologyEx(hsv_image, temp, MORPH_OPEN, element);
	//闭操作
	morphologyEx(temp, temp, MORPH_CLOSE, element);
}
vector<RotatedRect>& ArmorPlate::light_choose()
{
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	vector<RotatedRect> light_rect;//存储得到的灯条拟合矩形
	//寻找最外层轮廓
	Canny(temp, temp, 20, 80, 3, false);
	findContours(temp, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	//筛选出灯条
	//if (contours.size() < 2)return light_rect;
	for (int i = 0; i < contours.size(); i++)
	{
		//筛去面积较小的轮廓
		if (contourArea(contours[i]) <= 1)continue;
		//寻找最小外接矩形
		RotatedRect rect = minAreaRect(contours[i]);
		//筛掉长宽比不符合的灯条
		float hei_wid = rect.size.height / rect.size.width;
		if (hei_wid > 1 && rect.size.width < 50)
		{
			light_rect.push_back(rect);//存储获得的矩形
		}
	}
	return light_rect;
}
vector<RotatedRect>& ArmorPlate::armor_choose(vector<RotatedRect> Rect)
{
	vector<RotatedRect> Armor;
	//两两匹配得到装甲板
	for (int i = 0; i < Rect.size() - 1; i++)
	{
		RotatedRect armor;
		for (int j = i + 1; j < Rect.size(); j++)
		{
			//角度差
			float angle_diff = abs(Rect[i].angle - Rect[j].angle);
			//高度差
			float height_diff = abs(Rect[i].size.height - Rect[j].size.height);
			//宽度差
			float width_diff = abs(Rect[i].size.width - Rect[j].size.width);
			if (angle_diff < 5.0 && height_diff < 5 && width_diff < 10
				&& abs(Rect[i].center.x - Rect[j].center.x) < 100
				&& abs(Rect[i].center.y - Rect[j].center.y) < 5)
			{
				//设置装甲板参数
				armor.center.x = (Rect[i].center.x + Rect[j].center.x) / 2;
				armor.center.y = (Rect[i].center.y + Rect[j].center.y) / 2;
				armor.size.height = (Rect[i].size.height + Rect[j].size.height) / 2;
				armor.size.width = abs(Rect[i].center.x - Rect[j].center.x);
				armor.angle = (Rect[i].angle + Rect[j].angle) / 2;
				if (armor.size.width / armor.size.height > 5)continue;			
				Armor.push_back(armor);//将识别到的装甲板存储起来
				break;
			}
		}
	}
	return Armor;
}
void ArmorPlate::draw(RotatedRect rect)
{
	//画出目标矩形
	Point2f P[4];
	rect.points(P);
	for (int i = 0; i < 4; i++)
	{
		line(frame, P[i], P[(i + 1) % 4], Scalar(0, 255, 255), 4, LINE_AA);
	}
}
void ArmorPlate::show()
{
	namedWindow("temp");
	imshow("temp", temp);
	namedWindow("source");
	imshow("source", frame);
}