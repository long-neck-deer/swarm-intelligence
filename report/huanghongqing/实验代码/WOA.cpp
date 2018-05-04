#include "stdafx.h"
#include "WOA.h"

// @author 黄红清
// @email huanghqdx@163.com
WOA::WOA(int x_min, int x_max, int y_min, int y_max, int whales_num, int max_it, double (*loss_func)(Point2d))
{
	this->x_min = x_min;
	this->x_max = x_max;
	this->y_min = y_min;
	this->y_max = y_max;
	this->max_it = max_it;
	this->cur_it = 0;
	this->loss_func = loss_func;
	this->best_loss = DBL_MAX;

	this->whales_num = whales_num;
	whales = new Point2d[whales_num];
	srand((unsigned)time(NULL));
	for (int i = 0; i < whales_num; i++) {
		whales[i].x = rand() % (x_max - x_min) + x_min + rand() / (double)RAND_MAX;
		whales[i].y = rand() % (y_max - y_min) + y_min + rand() / (double)RAND_MAX;
	}
	update_best_whale();

	//固定最优解
	/*this->best_whale = { 0.0,0.0 };
	this->best_loss = 0;*/
}

WOA::~WOA()
{
	delete whales;
}

double WOA::run()
{
	while (cur_it < max_it) {
		next_iteration();

	}
	return loss_func(best_whale);
}

int WOA::next_iteration()
{
	if (cur_it >= max_it) return cur_it;
	srand((unsigned)time(NULL));
	for (int i = 0; i < whales_num; i++) {
		double a, p, b=1.0;
		Point2d r1, r2, A, C, l;
		a = 2.0 - 2.0 / max_it * cur_it;
		p = rand() / (double)RAND_MAX;
		r1.x = rand() / (double)RAND_MAX;
		r1.y = rand() / (double)RAND_MAX;
		r2.x = rand() / (double)RAND_MAX;
		r2.y = rand() / (double)RAND_MAX;
		A.x = 2 * a*r1.x - a;
		A.y = 2 * a*r1.y - a;
		C.x = 2 * r2.x;
		C.y = 2 * r2.y;
		//论文中[-1,1]的l
		l.x = rand() % 2 - 1 + rand() / (double)RAND_MAX;
		l.y = rand() % 2 - 1 + rand() / (double)RAND_MAX;

		//论文实现中出现的神奇参数修改了l
		/*double a2 = -1.0 - 1.0 / max_it * cur_it;
		l.x = (a2 - 1)*(rand() / (double)RAND_MAX) + 1;
		l.y = (a2 - 1)*(rand() / (double)RAND_MAX) + 1;*/

		if (p < 0.5) {
			Point2d D;
			D.x = fabs(C.x*best_whale.x - whales[i].x);
			D.y = fabs(C.y*best_whale.y - whales[i].y);
			if (pow(A.ddot(A), 0.5) < 1) {// |A|<1
				whales[i].x = best_whale.x - A.x*D.x;
				whales[i].y = best_whale.y - A.y*D.y;
			}
			else {// |A|>=1
				Point2d rand_whale;
				rand_whale.x = rand() % (x_max - x_min) + x_min + rand() / (double)RAND_MAX;
				rand_whale.y = rand() % (y_max - y_min) + y_min + rand() / (double)RAND_MAX;
				whales[i].x = rand_whale.x - A.x*D.x;
				whales[i].y = rand_whale.y - A.y*D.y;
			}
		}
		else {// p>=0.5
			Point2d D2;
			D2.x = fabs(best_whale.x - whales[i].x);
			D2.y = fabs(best_whale.y - whales[i].y);
			whales[i].x = D2.x*exp(b*l.x)*cos(2 * PI*l.x) + best_whale.x;
			whales[i].y = D2.y*exp(b*l.y)*cos(2 * PI*l.y) + best_whale.y;
		}

	}

	//amend out of bounds
	for (int i = 0; i < whales_num; i++) {
		if (whales[i].x < x_min) whales[i].x = x_min;
		if (whales[i].y < y_min) whales[i].y = y_min;
		if (whales[i].x > x_max) whales[i].x = x_max;
		if (whales[i].y > y_max) whales[i].y = y_max;
	}

	update_best_whale();

	return ++cur_it;
}

int WOA::next_iteration_modify()
{
	if (cur_it >= max_it) return cur_it;
	srand((unsigned)time(NULL));
	for (int i = 0; i < whales_num; i++) {
		double a, p, b = 1.0;
		Point2d r1, r2, A, C, l;
		a = 1.0 - 1.0 / max_it * cur_it;
		p = rand() / (double)RAND_MAX;
		r1.x = rand() / (double)RAND_MAX;
		r1.y = rand() / (double)RAND_MAX;
		r2.x = rand() / (double)RAND_MAX;
		r2.y = rand() / (double)RAND_MAX;
		A.x = 2 * a*r1.x - a;
		A.y = 2 * a*r1.y - a;
		C.x = 2 * r2.x;
		C.y = 2 * r2.y;
		//论文中[-1,1]的l
		l.x = rand() % 2 - 1 + rand() / (double)RAND_MAX;
		l.y = rand() % 2 - 1 + rand() / (double)RAND_MAX;

		//论文实现中出现的神奇参数修改了l
		/*double a2 = -1.0 - 1.0 / max_it * cur_it;
		l.x = (a2 - 1)*(rand() / (double)RAND_MAX) + 1;
		l.y = (a2 - 1)*(rand() / (double)RAND_MAX) + 1;*/

		if (false) {
			Point2d D;
			D.x = fabs(C.x*best_whale.x - whales[i].x);
			D.y = fabs(C.y*best_whale.y - whales[i].y);
			if (/*pow(A.ddot(A), 0.5) < 1*/true) {// |A|<1
				whales[i].x = best_whale.x - A.x*D.x;
				whales[i].y = best_whale.y - A.y*D.y;
			}
			else {// |A|>=1
				Point2d rand_whale;
				rand_whale.x = rand() % (x_max - x_min) + x_min + rand() / (double)RAND_MAX;
				rand_whale.y = rand() % (y_max - y_min) + y_min + rand() / (double)RAND_MAX;
				whales[i].x = rand_whale.x - A.x*D.x;
				whales[i].y = rand_whale.y - A.y*D.y;
			}
		}
		else {// p>=0.5
			Point2d D2;
			D2.x = fabs(best_whale.x - whales[i].x);
			D2.y = fabs(best_whale.y - whales[i].y);
			whales[i].x = D2.x*exp(b*l.x)*cos(2 * PI*l.x) + best_whale.x;
			whales[i].y = D2.y*exp(b*l.y)*cos(2 * PI*l.y) + best_whale.y;
		}

	}

	//amend out of bounds
	for (int i = 0; i < whales_num; i++) {
		if (whales[i].x < x_min) whales[i].x = x_min;
		if (whales[i].y < y_min) whales[i].y = y_min;
		if (whales[i].x > x_max) whales[i].x = x_max;
		if (whales[i].y > y_max) whales[i].y = y_max;
	}

	update_best_whale();

	return ++cur_it;
}

void WOA::update_best_whale()
{
	int best_i = -1;;
	double loss;
	for (int i = 0; i < whales_num; i++) {
		loss = loss_func(whales[i]);
		if (loss < best_loss) {
			best_loss = loss;
			best_i = i;
		}
	}
	if (best_i != -1)
		best_whale = whales[best_i];
}
