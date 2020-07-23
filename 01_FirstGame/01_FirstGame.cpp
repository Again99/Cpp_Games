#include <iostream>

using namespace std; // 使用 std 名命空间

// 全局变量声明
const char gStageData[] = "\
########\n\
# .. p #\n\
# oo   #\n\
#      #\n\
########";

const int gStageWidth = 8;
const int gStageHeight = 5;

enum Object {
	OBJ_SPACE,
	OBJ_WALL,
	OBJ_GOAL,
	OBJ_BLOCK,
	OBJ_BLOCK_ON_GOAL,
	OBJ_MAN,
	OBJ_MAN_ON_GOAL,

	OBJ_UNKNOWN,
};

void initialize(Object* state, int w, int h, const char* stageData);
void draw(const Object* state, int w, int h);
void update(Object* state, char input, int w, int h);
bool checkClear(const Object* state, int w, int h);

int main()
{
	// 初始化状态数组
	Object* state = new Object[gStageWidth * gStageHeight];

	initialize(state, gStageWidth, gStageHeight, gStageData);

	// 主循环
	while (true) {
		// 绘制场景
		draw(state, gStageWidth, gStageHeight);

		// 通关检查
		if (checkClear(state, gStageWidth, gStageHeight)) {
			break;
		}

		// 输入
		cout << "a: left, d: right, w: up, s: down" << endl;
		char input;
		cin >> input;

		// 更新
		update(state, input, gStageWidth, gStageHeight);

	}

	// 游戏胜利处理
	cout << "Congratulation's: you win." << endl;

	delete[] state;
	state = 0;

	return 0;
}

void initialize(Object* state, int w, int h, const char* stageData)
{
	// 读取指针
	const char* d = stageData;
	int x = 0;
	int y = 0;

	while (*d != '\0')
	{
		Object t;

		// 转换数据
		switch (*d)
		{
		case '#': t = OBJ_WALL; break;
		case ' ': t = OBJ_SPACE; break;
		case 'o': t = OBJ_BLOCK; break;
		case 'O': t = OBJ_BLOCK_ON_GOAL; break;
		case '.': t = OBJ_GOAL; break;
		case 'p': t = OBJ_MAN; break;
		case 'P': t = OBJ_MAN_ON_GOAL; break;
		case '\n':
			x = 0;
			++y;
			t = OBJ_UNKNOWN;
			break;
		default: t = OBJ_UNKNOWN; break;
		}

		++d;

		// 排除未知字符数据
		if (t != OBJ_UNKNOWN)
		{
			state[y * w + x] = t;
			++x;
		}
	}
}

void draw(const Object* state, int w, int h)
{
	const char font[] = { ' ', '#', '.', 'o', 'O', 'p', 'P' };

	for (int y = 0; y < h; ++y)
	{
		for (int x = 0; x < w; ++x)
		{
			Object o = state[y * w + x];

			cout << font[o];
		}

		cout << endl;
	}
}

void update(Object* s, char input, int w, int h)
{
	// 移动量
	int dx = 0;
	int dy = 0;
	switch (input)
	{
		case 'a': dx = -1; break;
		case 'd': dx = 1; break;
		case 'w': dy = -1; break;
		case 's': dy = 1; break;
	}

	// 查找玩家位置
	int i = -1;

	for (i = 0; i < w * h; ++i)
	{
		if (s[i] == OBJ_MAN || s[i] == OBJ_MAN_ON_GOAL)
		{
			break;
		}
	}

	int x = i % w;
	int y = i / w;

	// 移动后的坐标
	int tx = x + dx;
	int ty = y + dy;

	if (tx < 0 || ty < 0 || tx >= w || ty >= h)
	{
		return;
	}

	// 移动
	int p = y * w + x;
	int tp = ty * w + tx;

	if (s[tp] == OBJ_SPACE || s[tp] == OBJ_GOAL)
	{
		s[tp] = (s[tp] == OBJ_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN;

		s[p] = (s[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE;
	}
	else if (s[tp] == OBJ_BLOCK || s[tp] == OBJ_BLOCK_ON_GOAL)
	{
		int tx2 = tx + dx;
		int ty2 = ty + dy;

		if (tx2 < 0 || ty2 < 0 || tx2 >= w || ty2 >= h)
		{
			return;
		}

		int tp2 = ty2 * w + tx2;

		if (s[tp2] == OBJ_SPACE || s[tp2] == OBJ_GOAL)
		{
			s[tp2] = (s[tp2] == OBJ_GOAL) ? OBJ_BLOCK_ON_GOAL : OBJ_BLOCK;

			s[tp] = (s[tp] == OBJ_BLOCK_ON_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN;

			s[p] = (s[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE;
		}
	}
	else
	{
		// 因为是墙壁，所以不做任何操作
	}
}

bool checkClear(const Object* state, int w, int h)
{
	for (int i = 0; i < w * h; ++i)
	{
		if (state[i] == OBJ_BLOCK)
		{
			return false;
		}
	}

	return true;
}
