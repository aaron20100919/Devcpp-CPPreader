#include <cstdio>
#include <cstring>
#include <windows.h>

char input_content[10240]; // ��������� 
int count = 0;				//��¼ history �� 
char output_content[20240];//������� 
FILE* fp = NULL;			//�Ĳ�			
HWND hWnd;					//����ס���� 
char original_content[10240];//ԭʼ���� 
char historiy_contents[10][10240];//��ʷ���� 

HWND TMeno_hwnd = NULL; // ���ڱ����ҵ����ӿؼ����
bool found = false; // ��־��������¼�Ƿ��Ѿ��ҵ��ӿؼ�

void EnumChildControls(HWND hwnd)
{
	EnumChildWindows(hwnd, [](HWND hwndChild, LPARAM lParam) -> BOOL
	{
		if (found) // ����Ѿ��ҵ��ӿؼ�����ֱ�ӷ���FALSE�ж�ѭ��
			return FALSE;

		char className[256];
		GetClassNameA(hwndChild, className, sizeof(className));
		if (strcmp(className, "TMemo") == 0)
		{
			TMeno_hwnd = hwndChild;
			found = true; // �ҵ��ӿؼ������ñ�־����Ϊtrue
			return FALSE; // ����FALSE�ж�ѭ��
		}
		return TRUE;
	}, 0);
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	char windowTitle[256];
	GetWindowTextA(hwnd, windowTitle, sizeof(windowTitle));
	char targetTitle[] = "Dev-C++"; // Ŀ�괰�ڱ����е��ַ���
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
		// �ȴ������ͷ�
	}
}
void wait_on(int key)
{
	while (!(GetAsyncKeyState(key) & 0x8000))
	{
		// �ȴ���������
	}
}

bool on_press(int key)
{
	// �ж��Ƿ���
	return (GetAsyncKeyState(key) & 0x8000);
}

void out()
{
	// ���ı��������
	printf("%s\n", input_content);
	sprintf(output_content, "%s\r\n%s", input_content, original_content);
	SendMessage(hWnd, WM_SETTEXT, 0, (LPARAM)output_content);
}

void handleKeyPress()
{
	// ��������
	if (on_press(VK_F8))
	{
		wait_off(VK_F8);

		if (!feof(fp))
		{
			fscanf(fp, "%s", input_content);
			printf("%s\n", input_content);
			out();

			if (count < 10) // �����ʷ��¼
			{
				strcpy(historiy_contents[count], input_content);
				count++;
			}
			else
			{
				for (int i = 0; i < 9; i++) // ����ģ�����
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

		if (count > 0) // ��һ��
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
	else if (on_press(VK_F6)) // ����
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
	else if (on_press(VK_F9)) // �˳�
	{
		wait_off(VK_F9);

		fclose(fp);
		exit(0);
	}
}

void init()
{
	// ��ʼ��
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
	// ������
	while (1)
	{
		handleKeyPress();
	}
}
int main()
{
	// ��...����
	init();
	handle();
	fclose(fp);

	return 0;
}
// F8��һ��, F7��һ��, F6����/��ʾ, F9ֱ���˳� 
