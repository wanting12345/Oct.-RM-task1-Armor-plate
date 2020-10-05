#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include "armorPlate.h"
#include<algorithm>
using namespace cv;
using namespace std;
//�����װ�װ���������sort������cmp����
bool cmp(RotatedRect a, RotatedRect b)
{
	return a.size.area() > b.size.area();
}
bool capture_IsOpened(VideoCapture cap)
{
	if (!cap.isOpened())
	{
		cout << "�o������ҕ�l" << endl;
		return -1;
	}
}
void ArmorPlate::image_process()
{
		//�D����HSV���g
	cvtColor(frame, gray, COLOR_BGR2HSV);
	inRange(gray, Scalar(80,40,46), Scalar(124,255,255), hsv_image);
	//������
	Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
	morphologyEx(hsv_image, temp, MORPH_OPEN, element);
	//�ղ���
	morphologyEx(temp, temp, MORPH_CLOSE, element);
}
vector<RotatedRect>& ArmorPlate::light_choose()
{
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	vector<RotatedRect> light_rect;//�洢�õ��ĵ�����Ͼ���
	//Ѱ�����������
	Canny(temp, temp, 20, 80, 3, false);
	findContours(temp, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	//ɸѡ������
	//if (contours.size() < 2)return light_rect;
	for (int i = 0; i < contours.size(); i++)
	{
		//ɸȥ�����С������
		if (contourArea(contours[i]) <= 1)continue;
		//Ѱ����С��Ӿ���
		RotatedRect rect = minAreaRect(contours[i]);
		//ɸ������Ȳ����ϵĵ���
		float hei_wid = rect.size.height / rect.size.width;
		if (hei_wid > 1 && rect.size.width < 50)
		{
			light_rect.push_back(rect);//�洢��õľ���
		}
	}
	return light_rect;
}
vector<RotatedRect>& ArmorPlate::armor_choose(vector<RotatedRect> Rect)
{
	vector<RotatedRect> Armor;
	//����ƥ��õ�װ�װ�
	for (int i = 0; i < Rect.size() - 1; i++)
	{
		RotatedRect armor;
		for (int j = i + 1; j < Rect.size(); j++)
		{
			//�ǶȲ�
			float angle_diff = abs(Rect[i].angle - Rect[j].angle);
			//�߶Ȳ�
			float height_diff = abs(Rect[i].size.height - Rect[j].size.height);
			//��Ȳ�
			float width_diff = abs(Rect[i].size.width - Rect[j].size.width);
			if (angle_diff < 5.0 && height_diff < 5 && width_diff < 10
				&& abs(Rect[i].center.x - Rect[j].center.x) < 100
				&& abs(Rect[i].center.y - Rect[j].center.y) < 5)
			{
				//����װ�װ����
				armor.center.x = (Rect[i].center.x + Rect[j].center.x) / 2;
				armor.center.y = (Rect[i].center.y + Rect[j].center.y) / 2;
				armor.size.height = (Rect[i].size.height + Rect[j].size.height) / 2;
				armor.size.width = abs(Rect[i].center.x - Rect[j].center.x);
				armor.angle = (Rect[i].angle + Rect[j].angle) / 2;
				if (armor.size.width / armor.size.height > 5)continue;			
				Armor.push_back(armor);//��ʶ�𵽵�װ�װ�洢����
				break;
			}
		}
	}
	return Armor;
}
void ArmorPlate::draw(RotatedRect rect)
{
	//����Ŀ�����
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