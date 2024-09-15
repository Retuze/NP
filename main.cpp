#include <Windows.h>
#include <cstdio>
#include <filesystem>
namespace fs = std::filesystem;

HINSTANCE hAppInstance;

LRESULT CALLBACK WinSunProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HWND hwndEdit;
    HWND hwndCheckbox;
    HDC hdcStatic;
    UINT checkState;
    TCHAR buffer[256];
    switch (uMsg) // 通过判断消息进行消息响应
    {
    case WM_CREATE: {
        printf("hello\n");
        hwndCheckbox = CreateWindow(TEXT("BUTTON"), TEXT("在已经存在工程目录时是否强制生成？"),
                                    WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 40, 220, 300, 20, // 位置和大小
                                    hwnd,                                                      // 父窗口句柄
                                    (HMENU)10003,                                              // 控件ID
                                    hAppInstance,                                              // 当前实例句柄
                                    NULL);                                                     // 附加数据
        CreateWindow(TEXT("edit"), TEXT("输入项目编号，然后按下生成项目，即可自动生成工程目录，拉取git代码，启动工程"),
                     ES_MULTILINE | ES_AUTOVSCROLL | WS_VISIBLE | WS_CHILD | ES_LEFT | ES_READONLY, 40, 20, 320, 100,
                     hwnd, (HMENU)10002, hAppInstance, 0);
        // 创建一段说明文本，输入项目名称，然后按下确认，即可自动生成工程目录，拉取git代码，
        CreateWindow(TEXT("button"), TEXT("生成项目"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_DEFPUSHBUTTON, 350,
                     170, 80, 40, hwnd,
                     (HMENU)1001,  // 子窗口ID
                     hAppInstance, // 应用程序的句柄，也就是winmian中的参数hinstance
                     0);
        hwndEdit = CreateWindow(TEXT("edit"), TEXT("请输入项目编号"),
                                ES_MULTILINE | WS_BORDER | ES_AUTOVSCROLL | WS_VISIBLE | WS_CHILD | ES_LEFT, 40, 170,
                                280, 40, hwnd, (HMENU)10001, hAppInstance, 0);
        HFONT hFont = (HFONT)SendMessage(hwndEdit, WM_GETFONT, 0, 0);
        int fontHeight = 20;
        if (hFont != NULL)
        {
            TEXTMETRIC tm;
            HDC hdc = GetDC(hwndEdit);
            if (hdc != NULL)
            {
                if (GetTextMetrics(hdc, &tm))
                {
                    // tm.tmHeight 是字体的高度（以逻辑单位表示）
                    // 注意：tmHeight可能包含外部前导（external leading），你可能需要调整它
                    fontHeight = tm.tmHeight;

                    ReleaseDC(hwndEdit, hdc);
                    // 使用 fontHeight 进行布局调整
                }
                ReleaseDC(hwndEdit, hdc);
            }
        }
        RECT rect;

        GetClientRect(hwndEdit, &rect);
        OffsetRect(&rect, 0, (40 - fontHeight) / 2);
        SendMessage(hwndEdit, EM_SETRECT, 0, (LPARAM)&rect);
    }

    // 这里创建一个输入框
    // 这里创建一个按钮
    break;
        // 按钮消息
    case WM_COMMAND: {
        switch (LOWORD(wParam))
        {
        case 1001:
            checkState = (UINT)SendMessage(hwndCheckbox, BM_GETCHECK, 0, 0);
            GetDlgItemText(hwnd, 10001, buffer, 256);
            if (CreateDirectory(buffer, NULL))
            {
                MessageBox(hwnd, TEXT("生成成功"), TEXT("Info"), MB_OK);
            }
            else
            {
                printf("hello world\n");
                int result = MessageBox(hwnd, TEXT("项目已存在，是否覆盖？"), TEXT("Info"), MB_OKCANCEL);
jkbhb



                // 根据用户的选择执行不同的操作
                if (result == IDOK)
                {
                    // 用户点击了“确认”
                    // 在这里编写覆盖项目的代码
                    fs::path dir_path = buffer;
                    if (fs::exists(dir_path) && fs::is_directory(dir_path))
                    {
                        fs::remove_all(dir_path);
                        fs::create_directory(dir_path);
                    }
                    MessageBox(hwnd, TEXT("项目将被覆盖。"), TEXT("操作结果"), MB_OK);
                }
                else if (result == IDCANCEL)
                {
                    // 用户点击了“取消”
                    // 在这里编写取消操作的代码
                    MessageBox(hwnd, TEXT("项目覆盖已取消。"), TEXT("操作结果"), MB_OK);
                }
            }
            return 0;
        case 10001:
            TCHAR buffer[256];
            GetDlgItemText(hwnd, 10001, buffer, 256);
            if (wcscmp(buffer, TEXT("请输入项目编号")) == 0)
            {
                SetDlgItemText(hwnd, 10001, TEXT(""));
            }
            return 0;
        }
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    break;
    case WM_CLOSE:
        DestroyWindow(hwnd); // 销毁窗口并发送WM_DESTROY消息，但是程序没有退出
        break;
        PostQuitMessage(0); // 发出WM_QUIT消息，结束消息循环
    case WM_DESTROY:
        break;
    case WM_CTLCOLORSTATIC:
        hdcStatic = (HDC)wParam;
        SetTextColor(hdcStatic, RGB(0x41, 0x96, 0x4F)); // 翠绿色
        SetBkMode(hdcStatic, TRANSPARENT);              // 透明背景
        return (INT_PTR)GetStockObject(NULL_BRUSH);     // 无颜色画刷
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam); // 对不感兴趣的消息进行缺省处理，必须有该代码，否则程序有问题
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    // 假设的窗口宽度和高度
    int width = 500;
    int height = 300;

    // 获取屏幕的宽度和高度
    int cxScreen = GetSystemMetrics(SM_CXSCREEN);
    int cyScreen = GetSystemMetrics(SM_CYSCREEN);

    // 计算窗口的初始位置，使其居中
    int x = (cxScreen - width) / 2;
    int y = (cyScreen - height) / 2;

    hAppInstance = hInstance;                                   // 给全局变量赋值
    WNDCLASS wndcls;                                            // 创建一个窗体类
    wndcls.cbClsExtra = 0;                                      // 类的额外内存，默认为0即可
    wndcls.cbWndExtra = 0;                                      // 窗口的额外内存，默认为0即可
    wndcls.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // 获取画刷句柄（将返回的HGDIOBJ进行强制类型转换）
    wndcls.hCursor = LoadCursor(NULL, IDC_ARROW);               // 设置光标
    wndcls.hIcon = LoadIcon(NULL, IDI_APPLICATION); // 设置窗体左上角的图标
    wndcls.hInstance = hInstance;                   // 设置窗体所属的应用程序实例
    wndcls.lpfnWndProc = WinSunProc; // 设置窗体的回调函数，暂时没写，先设置为NULL，后面补上
    wndcls.lpszClassName = L"NP";    // 设置窗体的类名
    wndcls.lpszMenuName = NULL;      // 设置窗体的菜单,没有，填NULL
    wndcls.style = CS_HREDRAW | CS_VREDRAW; // 设置窗体风格为水平重画和垂直重画
    RegisterClass(&wndcls);                 // 向操作系统注册窗体

    // 产生一个窗体，并返回该窗体的句柄，第一个参数必须为要创建的窗体的类名，第二个参数为窗体标题名
    HWND hwnd = CreateWindow(L"NP", L"新建项目", WS_OVERLAPPEDWINDOW, x, y, width, height, NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, SW_SHOWNORMAL); // 把窗体显示出来
    UpdateWindow(hwnd);              // 更新窗体

    // 消息循环
    MSG 
    while (GetMessage(&msg, NULL, 0, 0)) // 如果消息不是WM_QUIT,返回非零值；如果消息是WM_QUIT，返回零
    {
        DispatchMessage(&msg);  // 派发消息
        TranslateMessage(&msg); // 翻译消息，如把WM_KEYDOWN和WM_KEYUP翻译成一个WM_CHAR消息
    }
}
