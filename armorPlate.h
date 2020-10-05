#pragma once
#ifndef _ARMORPLATE_H_
#define _ARMORPLATE_H_
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include<algorithm>
using namespace cv;
using namespace std;

extern bool capture_IsOpened(VideoCapture cap);//�ж��Ƿ��ȡ��Ƶ�ɹ�
extern bool cmp(RotatedRect a, RotatedRect b);
 class ArmorPlate
{
public:
	ArmorPlate();
	~ArmorPlate();
	RotatedRect result;
	vector<RotatedRect> armor;//�����洢ʶ����������
	vector<RotatedRect> light;//�����洢ʶ����������
	void image_process();//ͼ��Ԥ����
	vector<RotatedRect>& light_choose();//ɸѡ��Ŀ����Ҷ
	vector<RotatedRect>& armor_choose(vector<RotatedRect> Rect);//ɸѡ��Ŀ����Ҷ
	void draw(RotatedRect rect);//�궨���
	void show();//������ʾ
	void setframe(VideoCapture cap)//��֡д��
	{
		cap >> frame;
	}
	Mat getframe()
	{
		return frame;
	}

private:
	Mat frame;
	Mat gray;
	Mat hsv_image;
	Mat temp;
	
};

ArmorPlate::ArmorPlate()
{
}

ArmorPlate::~ArmorPlate()
{
}

#endif