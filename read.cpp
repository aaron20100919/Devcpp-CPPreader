#include <cstdio>
#include <cstring>
#include <windows.h>

char input_content[10240]; // 输入的内容 
int count = 0;				//记录 history 数 
char output_content[20240];//输出内容 
FILE* fp = NULL;			//文操			
HWND hWnd;					//控制住窗口 
char original_content[10240];//原始内容 
char historiy_contents[10][10240];//历史内容 

HWND TMeno_hwnd = NULL; // 用于保存找到的子控件句柄
bool found = false; // 标志变量，记录是否已经找到子控件

void EnumChildControls(HWND hwnd)
{
	EnumChildWindows(hwnd, [](HWND hwndChild, LPARAM lParam) -> BOOL
	{
		if (found) // 如果已经找到子控件，则直接返回FALSE中断循环
			return FALSE;

		char className[256];
		GetClassNameA(hwndChild, className, sizeof(className));
		if (strcmp(className, "TMemo") == 0)
		{
			TMeno_hwnd = hwndChild;
			found = true; // 找到子控件后设置标志变量为true
			return FALSE; // 返回FALSE中断循环
		}
		return TRUE;
	}, 0);
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	char windowTitle[256];
	GetWindowTextA(hwnd, windowTitle, sizeof(windowTitle));
	char targetTitle[] = "Dev-C++"; // 目标窗口标题中的字符串
	if (strstr(windowTitle, targetTitle))
	{
		printf("Dev-C++ Window Found!\n");
		EnumChildControls(hwnd);
		return FALSE;
	}
	return TRUE;
}

HWND FindTMemo()
{
	EnumWindows(EnumWindowsProc, 0);
	return TMeno_hwnd;
}

void wait_off(int key)
{
	while ((GetAsyncKeyState(key) & 0x8000))
	{
		// 等待按键释放
	}
}
void wait_on(int key)
{
	while (!(GetAsyncKeyState(key) & 0x8000))
	{
		// 等待按键按下
	}
}

bool on_press(int key)
{
	// 判断是否按下
	return (GetAsyncKeyState(key) & 0x8000);
}

void out()
{
	// 在文本框内输出
	printf("%s\n", input_content);
	sprintf(output_content, "%s\r\n%s", input_content, original_content);
	SendMessage(hWnd, WM_SETTEXT, 0, (LPARAM)output_content);
}

void handleKeyPress()
{
	// 监听键盘
	if (on_press(VK_F8))
	{
		wait_off(VK_F8);

		if (!feof(fp))
		{
			fscanf(fp, "%s", input_content);
			printf("%s\n", input_content);
			out();

			if (count < 10) // 添加历史记录
			{
				strcpy(historiy_contents[count], input_content);
				count++;
			}
			else
			{
				for (int i = 0; i < 9; i++) // 暴力模拟队列
				{
					strcpy(historiy_contents[i], historiy_contents[i + 1]);
				}
				strcpy(historiy_contents[9], input_content);
			}
		}
		else
		{
			printf("all\n");
		}
	}
	else if (on_press(VK_F7))
	{
		wait_off(VK_F7);

		if (count > 0) // 上一个
		{
			count--;
			strcpy(input_content, historiy_contents[count]);
			out();
		}
		else
		{
			strcpy(input_content, "cant bak, pliz thx.");
			out();
		}
	}
	else if (on_press(VK_F6)) // 隐藏
	{
		wait_off(VK_F6);
		SendMessage(hWnd, WM_SETTEXT, 0, (LPARAM)original_content);
		wait_on(VK_F6);
		wait_off(VK_F6);
		if (count > 0)
		{
			count--;
			strcpy(input_content, historiy_contents[count]);
			out();
		}
		else
		{
			strcpy(input_content, "cant bak, pliz thx.");
			out();
		}
	}
	else if (on_press(VK_F9)) // 退出
	{
		wait_off(VK_F9);

		fclose(fp);
		exit(0);
	}
}

void init()
{
	// 初始化
	hWnd = FindTMemo();

	SendMessage(hWnd, WM_GETTEXT, 10240, (LPARAM)original_content);

	fp = fopen("santi.txt", "r");
	if (fp == NULL)
	{
		perror("WARNING:");
		exit(1);
	}

}
void handle()
{
	// 主程序
	while (1)
	{
		handleKeyPress();
	}
}
int main()
{
	// 主...函数
	init();
	handle();
	fclose(fp);

	return 0;
}
// F8下一个, F7上一个, F6隐藏/显示, F9直接退出 
