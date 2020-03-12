#include <iostream>
#include <opencv2/core/mat.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

Mat img;

const Vec3b black({ 0, 0, 0 });
const Vec3b red ({0, 0, 255});
const Vec3b blue({255, 0, 0});

const int dx[] = { -1, 0, 1, 0 };
const int dy[] = { 0, -1, 0, 1 };

int m, n;
bool in_bounds(int x, int y)
{
	return x >= 0 && x < n && y >= 0 && y < m;
}

vector<pair<int, int> > cc;
long long tot_x, tot_y;
int tot_n;
void dfs(int x, int y, Vec3b tar_col)
{
	Vec3b col = img.at<Vec3b>(x, y);
	if (!in_bounds(x, y) || col != tar_col)
		return;
	img.at<Vec3b>(x, y) = blue;
	tot_x += x;
	tot_y += y;
	tot_n++;
	cc.push_back({ x, y });
	for (int k = 0; k < 4; k++)
	{
		dfs(x + dx[k], y + dy[k], tar_col);
	}
}

int dist(pair<int, int> a, pair<int, int> b)
{
	return sqrt((a.first - b.first) * (a.first - b.first) + (b.second - a.second) * (b.second - a.second));
}

int main()
{
	img = imread("circles.png", IMREAD_COLOR);
	n = img.rows;
	m = img.cols;
	//imshow("test", img);
	//waitKey(0);
	int RC = 0, BC = 0;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
		{
			Vec3b col = img.at<Vec3b>(i, j);
			if (col == black || col == red)
			{
				dfs(i, j, col);
				tot_x /= tot_n;
				tot_y /= tot_n;
				pair<int, int> C = { tot_x, tot_y };
				int L = 1e6, R = 0;
				for (auto p : cc)
				{
					int d = dist(p, C);
					for (int k = 0; k < 4; k++)
						if (img.at<Vec3b>(p.first + dx[k], p.second + dy[k]) != blue)
						{
							L = min(L, d);
							R = max(R, d);
							break;
						}
				}
				if (R - L < 3)
				{
					if (col == red)
						RC++;
					else
						BC++;
				}
				cc.clear();
				tot_x = 0;
				tot_y = 0;
				tot_n = 0;
			}
		}
	cout << "Red: " << RC << " Black: " << BC << "\n";
	return 0;
}