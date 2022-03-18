#include "class.h"
BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
void On_Command(HWND hWnd, int id, HWND hChild, UINT code);
LRESULT On_Notify(HWND hWnd, int idFrom, NMHDR* pnmhdr);
///////////////////////////////////////////////////////////////////////////////////////
HWND hEd,hEd1;
wchar_t byf [MAX_PATH]={0};
HINSTANCE hInstance;
Dir  obj;
///////////////////////////////////////////////////////////////////////////////////////
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpszCmdLine, int nCmdShow)
{
	hInstance=hInst;
	MSG msg;
	INITCOMMONCONTROLSEX icex; 
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC  = ICC_LISTVIEW_CLASSES;
	InitCommonControlsEx(&icex);
	HWND hDialog = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
	HMENU hMenu = LoadMenu(GetModuleHandle(0), MAKEINTRESOURCE(IDR_MENU1));
	HACCEL hAccel = LoadAccelerators(hInst, MAKEINTRESOURCE(IDR_ACCELERATOR1));

	obj.Asses(hDialog,hInst);
	SetMenu(hDialog, hMenu);
	ShowWindow(hDialog, nCmdShow); 

	while(GetMessage(&msg, 0, 0, 0))
	{
		if(!TranslateAccelerator(hDialog,hAccel, &msg)) 
		{

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;
}


///////////////////////////////////////////////////////////////////////////////////////
BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wParam, LPARAM lParam){
	obj.Static_l = GetDlgItem(hWnd, IDC_STATIC2);
	obj.Static_r = GetDlgItem(hWnd, IDC_STATIC1);
	switch(mes){
		HANDLE_MSG(hWnd, WM_INITDIALOG, obj.On_InitDialog);
		HANDLE_MSG(hWnd, WM_COMMAND, On_Command);
		HANDLE_MSG(hWnd, WM_CLOSE,obj.Cls_OnClose);
		HANDLE_MSG(hWnd, WM_NOTIFY,On_Notify);
	}
	return FALSE;
}
///////////////////////////////////////////////////////////////////////////////////////
BOOL CALLBACK DlgProc2(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp)
{
	switch(mes)
	{
	case WM_INITDIALOG:{
		obj.hL2=GetDlgItem(hWnd, IDC_LIST2);
		ShowWindow(obj.hL2, SW_HIDE);
					   }
	case WM_COMMAND:{
		short id = LOWORD(wp);
		UINT msg = HIWORD(wp);
		switch(msg){
	case BN_CLICKED:{
		if(id==IDC_RADIO1){
			obj.FindNameOrText = 1;
			break;
		}
		else if(id==IDC_RADIO2){
			obj.FindNameOrText = 2;
			break;
		}
		else if(id==IDC_BUTTON1){
			if(obj.FindNameOrText!=0){
				HWND hEdit = GetDlgItem(hWnd, IDC_EDIT1);
				HWND hList = GetDlgItem(hWnd, IDC_LIST1);
				//buf_Find = L"D:";
				for(int i=0; i<obj.DisksForFind.size(); i++){
					wstring buf_tmp;
					buf_tmp += obj.DisksForFind[i];
					obj.buf_Find.clear();
					obj.buf_Find += buf_tmp[0];
					obj.buf_Find += buf_tmp[1];
					if(obj.FindNameOrText==1)
						obj.Find_File(hEdit, hList);
					else if(obj.FindNameOrText==2)
						obj.Find_TextInFile(hEdit, hList);
					for(;obj.DerictoryFind.empty()!=true;){
						obj.buf_Find.clear();
						obj.buf_Find += obj.DerictoryFind[0];
						vector<wstring>::iterator tmp_vector = obj.DerictoryFind.begin();
						obj.DerictoryFind.erase(tmp_vector);
						if(obj.FindNameOrText==1)
							obj.Find_File(hEdit, hList);
						else if(obj.FindNameOrText==2)
							obj.Find_TextInFile(hEdit, hList);
					}
				}
				MessageBox(NULL, L"Поиск закончен", L"", MB_OK|MB_ICONINFORMATION);
			}
			else
				MessageBox(NULL, L"Не выбран тип поиска, пожалуйсто выберите тип и повторите", L"Сообщение Менеджера", MB_OK|MB_ICONINFORMATION);
		}
					}
		}
					}
					break;
	case WM_CLOSE:
		EndDialog(hWnd, 0); // закрываем модальный диалог
		return TRUE;
	}
	return FALSE;
}
///////////////////////////////////////////////////////////////////////////////////////
BOOL CALLBACK DlgProc3(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp)
{
	//int FileOrDirectory;
	wstring text_l, text_r;
	switch(mes)
	{
	case WM_COMMAND:{
		short id = LOWORD(wp);
		UINT msg = HIWORD(wp);
		switch(msg){
	case BN_CLICKED:{
		if(id==IDC_BUTTON2)
			EndDialog(hWnd, 0);
		if(id==IDC_RADIO1)
			obj.FileOrDirectory=1;
		if(id==IDC_RADIO2)
			obj.FileOrDirectory=2;
		if(id==IDC_CHECK1){
			
			obj.AddNewFileLefOrRight = 1;
			wchar_t bufer[MAX_PATH];
			GetWindowText(obj.Static_r, bufer, 260);
			text_l += bufer;
		}
		if(id==IDC_CHECK2){
			
			obj.AddNewFileLefOrRight = 2;
			wchar_t bufer[MAX_PATH];
			GetWindowText(obj.Static_l, bufer, 260);
			text_l += bufer;
		}
		if(id==IDC_OK){
			HWND hBL = GetDlgItem(hWnd, IDC_CHECK1);
			HWND hBR = GetDlgItem(hWnd, IDC_CHECK2);
			HWND hE = GetDlgItem(hWnd, IDC_EDIT1);
			wchar_t buf[260];
			wstring text;
			BOOL bFile=false;

			if(SendMessage(hBR,BM_GETCHECK,0,0)==BST_CHECKED || SendMessage(hBL,BM_GETCHECK,0,0)==BST_CHECKED){
				if(obj.AddNewFileLefOrRight==1)
					GetWindowText(obj.Static_l, buf, 260);
				else if(obj.AddNewFileLefOrRight==2)
					GetWindowText(obj.Static_r, buf, 260);
				text_l += buf;

				GetWindowText(hE, buf, 260);
				text_l += buf;
				WIN32_FIND_DATA FindFileData;
				HANDLE hFind;
				hFind = FindFirstFile(text_l.data(), &FindFileData); 
        if (hFind == INVALID_HANDLE_VALUE){
			//MessageBox(0,L"Ошибка", L"Ненаиден",  MB_OK |MB_ICONWARNING);
		} 
		else 
		{
				do {
					bFile=true;
				} while (FindNextFile(hFind, &FindFileData) != 0||FindFileData.cFileName==buf);

				FindClose(hFind);
                }
				if(bFile==false){
					if(obj.FileOrDirectory==2)
						CreateDirectory(text_l.data(), NULL);
					else if(obj.FileOrDirectory==1){
						HANDLE FileHandle;    
						FileHandle=CreateFile(text_l.data(),GENERIC_READ |GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 0);    
						CloseHandle(FileHandle);

					}
				}
				else if(bFile==true){
					MessageBox(0, L"Фаил существует!",L"FaR",  MB_OK |MB_ICONWARNING);
				}
			}
			obj.FindFile(obj.hListBox,obj.hStatic);
			obj.FindFile(obj.hListBox1,obj.hStatic1);
		}
					}
		}
					}
					break;
	case WM_CLOSE:
		EndDialog(hWnd, 0); // закрываем модальный диалог
		return TRUE;
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////////////
LRESULT On_Notify(HWND hWnd, int id, NMHDR* pnmhdr){

	switch(pnmhdr->code){
		 case LVN_BEGINLABELEDIT:
			 {
				 hEd= ListView_GetEditControl(obj.hListBox);
				 hEd1= ListView_GetEditControl(obj.hListBox1);
			 }break;
		 case LVN_ENDLABELEDIT:
			 { 
				 if(obj.bStatikL==true)
				 {
					 int  index = ListView_GetNextItem(obj.hListBox,-1, LVNI_SELECTED);
					 wchar_t baf [260]={0};
					 GetWindowText(obj.hCombo,baf,10);
					 GetWindowText(hEd,byf,260);
					 wchar_t*tmp=wcschr(obj.full_Filename,'.');
					 wchar_t*tp=wcschr(byf,'.');
					 wcscat(baf,byf);
					 if(tmp!=0 && tp==0)wcscat(baf,tmp);
					 wcscat(baf,L"\0");
					 obj.Pereimenovat(obj.full_Filename,baf);
					 ListView_SetItemText(obj.hListBox,index,0,baf);

				 }
				 else if(obj.bStatikR==true)
				 {
					 int  index1 = ListView_GetNextItem(obj.hListBox1,-1, LVNI_SELECTED);
					 wchar_t baf [260]={0};
					 GetWindowText(obj.hCombo1,baf,10);
					 GetWindowText(hEd1,byf,260);
					 wchar_t*tmp=wcschr(obj.full_Filename,'.');
					 wchar_t*tp=wcschr(byf,'.');
					 wcscat(baf,byf);
					 if(tmp!=0 && tp==0) wcscat(baf,tmp);
					 wcscat(baf,L"\0");
					 obj.Pereimenovat(obj.full_Filename,baf);
					 ListView_SetItemText(obj.hListBox1,index1,0,baf);

				 }
				 obj.FindFile(obj.hListBox,obj.hStatic);
				 obj.FindFile(obj.hListBox1,obj.hStatic1);
			 }break;
		 case NM_DBLCLK:{
			 switch(id){
		 case IDC_LIST1:
			 {
				 obj.OpenFileL(obj);
				 break;
			 }
		 case IDC_LIST2:
			 {
				 obj.OpenFileR(obj);
				 break;
			 }

			 }
			 break;
						}
		 case NM_CLICK:{
			 switch(id){
		 case IDC_LIST1:{
			 LV_ITEM lvItem ;
			 wchar_t bufPathName [MAX_PATH]={0};
			 int index = ListView_GetNextItem(obj.hListBox,-1, LVNI_SELECTED);
			 if(index == -1){}
			 // MessageBox(0,L"Ошибка", L"Ненаиден",  MB_OK |MB_ICONWARNING);
			 else{
				 memset(&lvItem, 0, sizeof(lvItem));
				 lvItem.mask = LVIF_TEXT;
				 lvItem.iItem = index;
				 lvItem.iSubItem = 0;
				 ListView_GetItemText(obj.hListBox, index, 0,obj.szBufl, MAX_PATH);
				 SetWindowText(obj.hEdit,obj.szBufl);
				 wcscat(bufPathName,obj.szBufl);
				 obj.bStatikL=true;
				 obj.bStatikR=false;
				 obj.Info_File(hWnd,obj.hStatic,obj.szBufl);

			 }
			 break;
						}

		 case IDC_LIST2:{
			 LV_ITEM lvItem ;
			 wchar_t bufPathName [MAX_PATH]={0};
			 int index = ListView_GetNextItem(obj.hListBox1,-1, LVNI_SELECTED);

			 if(index == -1){}
			 //MessageBox(0,L"Ошибка", L"Ненаиден",  MB_OK |MB_ICONWARNING);
			 else{
				 memset(&lvItem, 0, sizeof(lvItem));
				 lvItem.mask = LVIF_TEXT| LVIF_IMAGE| LVIF_PARAM | LVIF_STATE ;
				 lvItem.iItem = index;
				 lvItem.iSubItem = 0;

				 ListView_GetItemText(obj.hListBox1, index, 0, obj.szBufr, MAX_PATH);
				 SetWindowText(obj.hEdit,obj.szBufr);
				 wcscat(bufPathName,obj.szBufr);
				 obj.bStatikL=false;
				 obj.bStatikR=true;
				 obj.Info_File(hWnd,obj.hStatic1,obj.szBufr);

			 }
			 break;
						}

			 }

					   }
	}
	return 0;
}
///////////////////////////////////////////////////////////////////////////////////////
void On_Command(HWND hWnd, int id, HWND hChild, UINT code){
	//MessageBox(0,L"FILE_ATTRIBUTE_ARCHIVE", L"Ненаиден",  MB_OK |MB_ICONWARNING);

	if(id==IDC_BUTTON1){
		obj.SaveBut();
	}
	if(id==IDC_BUTTON3){
		wstring strTmp;
		if(obj.PathNameL.size()>1) obj.PathNameL.pop_back();

		strTmp=obj.PathNameL.back();
		if(obj.PathNameL.size()>1)strTmp+=L"\\";
		SetWindowText(obj.hStatic,strTmp.data());
		obj.FindFile(obj.hListBox,obj.hStatic);
	}
	if(id==IDC_BUTTON2){
		wstring strTmp;
		if(obj.PathNameR.size()>1) obj.PathNameR.pop_back();

		strTmp=obj.PathNameR.back();
		if(obj.PathNameR.size()>1)strTmp+=L"\\";
		SetWindowText(obj.hStatic1,strTmp.data());
		obj.FindFile(obj.hListBox1,obj.hStatic1);


	}
	switch(id)
	{
	case ID_40008:
		obj.Delete();break;

	case ID_40006:
		obj.Copy();break;

	case ID_40002:
		obj.Rename();break;

	case ID_40003:
		{
			if (obj.bStatikL == TRUE)
			{
				obj.bStatikR = FALSE;
				obj.OpenFileL(obj);
			}

			else if (obj.bStatikR == TRUE)
			{
				obj.bStatikL = FALSE;
				obj.OpenFileR(obj);
			}
			break;
		}

	case ID_40009:
		DialogBox(hInstance,MAKEINTRESOURCE(IDD_DIALOG2),hWnd,DlgProc2);
		break;
	case ID_40001:
		DialogBox(hInstance,MAKEINTRESOURCE(IDD_DIALOG3),hWnd,DlgProc3);
		break;
	case ID_40021:
		obj.Move();
		break;
	}

	switch(code){
		case EN_CHANGE:
			obj.chenge=true; 
			if(SendMessage(obj.hEdit, WM_GETTEXTLENGTH, 0, 0)==0)
				obj.chenge=false;
			break;

		case CBN_SELCHANGE:
			switch(id){
		case IDC_COMBO:{
			wchar_t buf [128]={0};

			obj.indexCB=SendMessage(obj.hCombo,CB_GETCURSEL,0,0);
			SendMessage(obj.hCombo,CB_GETLBTEXT,obj.indexCB,(LPARAM)buf);
			SetWindowText(obj.hStatic,buf);
			SetWindowText(obj.hStaticInfoL,buf);
			obj.FindFile(obj.hListBox,obj.hStatic);
			obj.VectorL(obj.hStatic,obj.PathNameL);
					   }
					   break;
		case IDC_COMBO1:{
			wchar_t buf1 [128]={0};
			obj.indexCB1=SendMessage(obj.hCombo1,CB_GETCURSEL,0,0);
			SendMessage(obj.hCombo1,CB_GETLBTEXT,obj.indexCB1,(LPARAM)buf1);
			SetWindowText(obj.hStatic1,buf1);
			SetWindowText(obj.hStaticInfoR,buf1);
			obj.FindFile(obj.hListBox1,obj.hStatic1);
			obj.VectorL(obj.hStatic1,obj.PathNameR);
						}
						break;
			}
	}
}
///////////////////////////////////////////////////////////////////////////////////////http://www.lib.csu.ru/DL/bases/prg/frolov/books/bsp/v22/ch3.html
///////////////////////////////////////////////////////////////////////////////////////http://www.vsokovikov.narod.ru/New_MSDN_API/ref_api.htm
///////////////////////////////////////////////////////////////////////////////////////SHGetFileInfo Function
///////////////////////////////////////////////////////////////////////////////////////http://www.brainbench.com/xml/bb/register/optional.xml
///////////////////////////////////////////////////////////////////////////////////////http://www.brainbench.com/xml/bb/register/optional.xml
///////////////////////////////////////////////////////////////////////////////////////http://frolov-lib.ru/books/bsp/v22/ch3_2.html
///////////////////////////////////////////////////////////////////////////////////////http://doc-for-prog.narod.ru/topics/mfc/listctrl/lvitem.html
///////////////////////////////////////////////////////////////////////////////////////http://forum.sources.ru/index.php?showtopic=197977
///////////////////////////////////////////////////////////////////////////////////////http://www.realcoding.net/articles/winapi-funktsii-raboty-s-failami-chast-2.html#GetCurrentDirectory
// набрать в MSDN -File Management Functions
//копировать с диска на диск CopyFile Function
//создание фаила             CreateFile Function
//Copies, moves, renames, or deletes a file system object.-SHFileOperation Function   http://rfteam.110mb.com/asm/8.php  http://www.cracklab.ru/pro/cpp.php?r=filesys&d=delete

