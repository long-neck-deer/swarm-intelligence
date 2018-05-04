#pragma once
#include "stdafx.h"

// @author 黄红清
// @email huanghqdx@163.com
class WOA
{
public:
	int x_min, x_max, y_min, y_max;
	int max_it;
	int whales_num;

	double(*loss_func)(Point2d);//目标函数，最优=最小
	
	Point2d *whales;
	Point2d best_whale;
	double best_loss;
	int cur_it;


	WOA(int x_min,int x_max,int y_min,int y_max, int whales_num, int max_it, double(*loss_func)(Point2d));
	~WOA();
	double run();

private:
	void update_best_whale();

protected:
	int next_iteration();
	int next_iteration_modify();
};

