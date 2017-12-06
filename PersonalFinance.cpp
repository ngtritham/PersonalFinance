// PersonalFinance.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "PersonalFinance.h"

typedef struct OUTCOME {
	WCHAR item[MAX_LOADSTRING];
	WCHAR content[MAX_LOADSTRING];
	WCHAR cost[MAX_LOADSTRING];
};


bool isAllNum(WCHAR*);
void showRecordsInListview();
bool saveData(LPCWSTR fileName);
bool loadData(LPCWSTR fileName);
void convertBuffToItem(WCHAR *buffer);
WCHAR* convertItemToBuf(int index);

long long WideToLong(wchar_t *a);
long long sumCost();

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name


WCHAR* itemList[6] = { L"Ăn uống" , L"Di chuyển", L"Nhà cửa", L"Xe cộ", L"Nhu yếu phẩm", L"Dịch vụ" };
int itemIdex = -1;

HWND hComboboxItem;
HWND hContent;
HWND hCost;
HWND hButtonAdd;
HWND hListview;
HWND hTextBox;


HWND tempHandle;

vector<OUTCOME> Records;
OUTCOME tempRecord;
int recIndex = -1;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PERSONALFINANCE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PERSONALFINANCE));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PERSONALFINANCE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_BTNFACE+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PERSONALFINANCE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 600, 600, nullptr, nullptr, hInstance, nullptr);


   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_LISTVIEW_CLASSES | ICC_TREEVIEW_CLASSES;
	InitCommonControlsEx(&icex);


    switch (message)
    {
		HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
		HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);
		HANDLE_MSG(hWnd, WM_PAINT, OnPaint);
		HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);
		HANDLE_MSG(hWnd, WM_CLOSE, OnClose);
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}




// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}



//---------------------------------------------------------------------------------------------------------------------------------------------------


BOOL OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct) {

	LOGFONT lf;
	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
	HFONT hFont = CreateFont(lf.lfHeight - 4, lf.lfWidth,
		lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
		lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, lf.lfFaceName);


	INITCOMMONCONTROLSEX icex;

	// Ensure that the common control DLL is loaded. 
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_LISTVIEW_CLASSES | ICC_TREEVIEW_CLASSES;
	InitCommonControlsEx(&icex);


	tempHandle = CreateWindow(L"STATIC", L"Mục chi", WS_CHILD | WS_VISIBLE | SS_CENTER, 20, 20, 115, 35, hWnd, NULL, hInst, NULL);
	SendMessage(tempHandle, WM_SETFONT, WPARAM(hFont), TRUE);
	hComboboxItem = CreateWindow(L"COMBOBOX", L"", WS_CHILD | WS_VISIBLE | CBS_DROPDOWN, 20, 40, 115, 35, hWnd, (HMENU)IDC_COMBOBOX_ITEM, hInst, NULL);
	for (int i = 0; i < 6; i++) {
		ComboBox_AddString(hComboboxItem, itemList[i]);
	}
	SendMessage(hComboboxItem, WM_SETFONT, WPARAM(hFont), TRUE);


	tempHandle = CreateWindow(L"STATIC", L"Nội dung chi", WS_CHILD | WS_VISIBLE | SS_CENTER, 150, 20, 230, 25, hWnd, NULL, hInst, NULL);
	SendMessage(tempHandle, WM_SETFONT, WPARAM(hFont), TRUE);
	hContent = CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 150, 40, 230, 25, hWnd, NULL, hInst, NULL);
	SendMessage(hContent, WM_SETFONT, WPARAM(hFont), TRUE);


	tempHandle = CreateWindow(L"STATIC", L"Số tiền", WS_CHILD | WS_VISIBLE | SS_CENTER, 150, 80, 230, 25, hWnd, NULL, hInst, NULL);
	SendMessage(tempHandle, WM_SETFONT, WPARAM(hFont), TRUE);
	hCost = CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 150, 100, 230, 25, hWnd, NULL, hInst, NULL);
	SendMessage(hCost, WM_SETFONT, WPARAM(hFont), TRUE);



	

	hButtonAdd = CreateWindow(L"BUTTON", L"Thêm", WS_CHILD | WS_VISIBLE | SS_CENTER | BS_PUSHBUTTON, 400, 65, 100, 24, hWnd, (HMENU)IDC_BUTTON_ADD, hInst, NULL);
	SendMessage(hButtonAdd, WM_SETFONT, WPARAM(hFont), TRUE);

	hListview = CreateWindow(WC_LISTVIEWW, L"", WS_CHILD | WS_VISIBLE | WS_VSCROLL | LVS_REPORT | LVS_AUTOARRANGE | WS_BORDER, 20, 160, 360, 360, hWnd, (HMENU)IDC_LISTVIEW_RECORDS, hInst, NULL);
	SendMessage(hListview, WM_SETFONT, WPARAM(hFont), TRUE);
	// Tạo List View
	LVCOLUMN lvCol;
	lvCol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;
	lvCol.fmt = LVCFMT_CENTER;

	lvCol.pszText = L"           Mục chi";
	lvCol.cx = 120;
	ListView_InsertColumn(hListview, 0, &lvCol);


	lvCol.pszText = L"Nội dung chi";
	lvCol.cx = 120;
	ListView_InsertColumn(hListview, 1, &lvCol);


	lvCol.pszText = L"Số tiền";
	lvCol.cx = 120;
	ListView_InsertColumn(hListview, 2, &lvCol);

	loadData(L"data.txt");

	return TRUE;
}

void OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify) {
	unsigned int sizeContent = 0;
	unsigned int sizeCost = 0;

	WCHAR* bufContent = NULL;
	WCHAR* bufCost = NULL;

	OUTCOME tempRecord;

	switch (id)
	{
	case IDC_BUTTON_ADD:
		sizeContent = GetWindowTextLength(hContent);
		sizeCost = GetWindowTextLength(hCost);

		if (itemIdex < 0 || sizeContent == 0 || sizeCost == 0) {
			MessageBox(hWnd, L"Bạn chưa nhập đủ dữ liệu !!!", L"Lỗi !!!", MB_OK);
		}
		else {
			bufContent = new WCHAR[sizeContent + 1];
			bufCost = new WCHAR[sizeCost + 1];
			GetWindowText(hContent, bufContent, sizeContent + 1);
			GetWindowText(hCost, bufCost, sizeCost + 1);

			if (isAllNum(bufCost) == false) {
				MessageBox(hWnd, L"Số tiền không bao gồm chữ cái và các ký tự khác.\nVui lòng nhập lại !", L"Lỗi !!!", MB_OK);
			}
			else {
				SetWindowText(hComboboxItem, L"");
				SetWindowText(hContent, L"");
				SetWindowText(hCost, L"");

				StrCpy(tempRecord.item, itemList[itemIdex]);
				StrCpy(tempRecord.content, bufContent);
				StrCpy(tempRecord.cost, bufCost);

				Records.push_back(tempRecord);
				recIndex++;

				ListView_DeleteAllItems(hListview);
				showRecordsInListview();

				itemIdex = -1;
			}

		}

		break;
	case IDC_COMBOBOX_ITEM:
		if (codeNotify == CBN_SELCHANGE) {
			itemIdex = ComboBox_GetCurSel(hwndCtl);
		}
		break;
	case IDM_ABOUT:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
		break;
	case IDM_EXIT:
		DestroyWindow(hWnd);
		break;
	}
}

void OnPaint(HWND hWnd) {
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	// TODO: Add any drawing code that uses hdc here...
	EndPaint(hWnd, &ps);
}

void OnDestroy(HWND hWnd) {
	PostQuitMessage(0);
}

void OnClose(HWND hWnd) {
	saveData(L"data.txt");
	DestroyWindow(hWnd);
}




// Functions:

void showRecordsInListview() {
	LV_ITEM lvItem;
	int i = 0;

	if (recIndex < 0)
		return;

	while (i <= recIndex)
	{
		lvItem.mask = LVIF_TEXT;
		lvItem.iItem = i;
		lvItem.iSubItem = 0;
		lvItem.pszText = Records[i].item;
		ListView_InsertItem(hListview, &lvItem);

		ListView_SetItemText(hListview, i, 1, Records[i].content);

		ListView_SetItemText(hListview, i, 2, Records[i].cost);

		i++;
	}
}

bool isAllNum(WCHAR* str) {
	int i, j;
	WCHAR pattern[11] = L"0123456789";
	bool flag = false;
	for (i = 0; str[i] != L'\0'; i++)
	{
		flag = false;
		for (j = 0; j < 11; j++) {
			if (str[i] == pattern[j]) {
				flag = true;
				break;
			}
		}
		if (j == 11)
			return false;
	}
	return flag;
}

bool saveData(LPCWSTR fileName) {
	HANDLE hFile;
	
	// Danh sách các Record rỗng
	if (recIndex < 0)
		return false;
	
	
	DWORD dwByteWritten;
	WCHAR* buffer = NULL;

	int bufLen = 0;

	hFile = CreateFile(fileName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		FlushFileBuffers(hFile);
		for (int i = 0; i <= recIndex; i++) {
			buffer = convertItemToBuf(i);
			bufLen = wcslen(buffer);

			WriteFile(hFile, buffer, bufLen * sizeof(WCHAR), &dwByteWritten, NULL);
		}
		delete buffer;
		CloseHandle(hFile);
		return true;
	}
	else {
		return false;
	}
}


bool loadData(LPCWSTR fileName) {
	HANDLE hFile = CreateFile(fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		int bufLen = GetFileSize(hFile, NULL) / sizeof(WCHAR);
		
		if (bufLen == 0)
			return false;

		WCHAR *buffer = new WCHAR[bufLen + 1];

		DWORD dwBytesRead;
		ReadFile(hFile, buffer, bufLen * sizeof(WCHAR), &dwBytesRead, NULL);
		buffer[bufLen] = NULL;
		SetWindowText(hTextBox, buffer);
		convertBuffToItem(buffer);



		LV_ITEM lvItem;
		for (int i = 0; i <= recIndex; i++) {
			lvItem.mask = LVIF_TEXT;
			lvItem.iItem = i;
			lvItem.iSubItem = 0;
			lvItem.pszText = Records[i].item;
			ListView_InsertItem(hListview, &lvItem);

			ListView_SetItemText(hListview, i, 1, Records[i].content);

			ListView_SetItemText(hListview, i, 2, Records[i].cost);
		}

		CloseHandle(hFile);
		return true;
	}
	else {
		return false;
	}
}

void convertBuffToItem(WCHAR *buffer) {
	int j = 0;
	int h = -1;
	WCHAR str[100];
	OUTCOME tmpOUT;
	for (int i = 0; buffer[i] != NULL; i++) {
		//j = 0;
		if (buffer[i] != L'\r' && buffer[i + 1] != L'\n') {
			str[j] = buffer[i];
			j++;
		}
		else {
			h++;
			str[j] = NULL;
			
			switch (h) {
			case 0:
				StrCpy(tmpOUT.item, str);
				break;
			case 1:
				StrCpy(tmpOUT.content, str);
				break;
			case 2:
				StrCpy(tmpOUT.cost, str);
				h = -1;
				Records.push_back(tmpOUT);
				recIndex++;
				break;
			}
			j = 0;
			StrCpy(str, _T(""));
		}
	}

}

WCHAR* convertItemToBuf(int index) {
	WCHAR *buffer = new WCHAR[1000];
	StrCpy(buffer, Records[index].item);
	StrCat(buffer, L"\r\n");
	StrCat(buffer, Records[index].content);
	StrCat(buffer, L"\r\n");
	StrCat(buffer, Records[index].cost);
	StrCat(buffer, L"\r\n");
	int bufLen = wcslen(buffer);
	buffer[bufLen] = NULL;
	return buffer;
}

long long WideToLong(wchar_t *a)
{
	long long i = 0;
	long long len = wcslen(a);
	long long coefficient = 0;
	long long RawNumber = 0;
	long long Number = 0;

	for (int k = 0; k < len; k++)
	{
		coefficient = pow(10, len - 1 - k);
		RawNumber = (int)a[k];
		Number = RawNumber - 48;
		i += coefficient * Number;
	}
	return i;
}

long long sumCost() {
	long long result = 0;
	//long long num;
	for (int i = 0; i <= itemIdex; i++) {
		result += WideToLong(Records[i].cost);
	}
	WCHAR buffer[20];
	swprintf(buffer, sizeof(buffer) / sizeof(*buffer), L"%d", result);
	wprintf(L"[%ls]\n", buffer);

	MessageBox(NULL, buffer, 0, MB_ICONINFORMATION);
	return result;
}