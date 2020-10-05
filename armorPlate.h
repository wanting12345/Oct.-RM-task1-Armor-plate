#pragma once
#ifndef _ARMORPLATE_H_
#define _ARMORPLATE_H_
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include<algorithm>
using namespace cv;
using namespace std;

extern bool capture_IsOpened(VideoCapture cap);//判断是否读取视频成功
extern bool cmp(RotatedRect a, RotatedRect b);
 class ArmorPlate
{
public:
	ArmorPlate();
	~ArmorPlate();
	RotatedRect result;
	vector<RotatedRect> armor;//用来存储识别结果的容器
	vector<RotatedRect> light;//用来存储识别结果的容器
	void image_process();//图像预处理
	vector<RotatedRect>& light_choose();//筛选出目标扇叶
	vector<RotatedRect>& armor_choose(vector<RotatedRect> Rect);//筛选出目标扇叶
	void draw(RotatedRect rect);//标定结果
	void show();//窗口显示
	void setframe(VideoCapture cap)//逐帧写入
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