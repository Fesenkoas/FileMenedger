#pragma once
#include "header.h"


class Edit{
protected:
	HWND hWnd;
	HINSTANCE g_hinst;
public:
	vector<wstring> PathNameL;
	vector<wstring> PathNameR;
	vector <wstring> sizeDir;
	vector <wstring> DisksForFind;
	vector <wstring> DerictoryFind;

	wstring buf_Find;

	HWND hCombo,hCombo1,hStatic,hStatic1,hListBox,hListBox1,hEdit,hListInfo,hSaveButt,hStaticInfoL,hStaticInfoR;
	HWND hCheckA,hCheckS,hCheckNV,hCheckR;
	HWND hL2, Static_l, Static_r;

	int	indexLB,indexCB;
	int	indexLB1,indexCB1;
	int i;
	int FindNameOrText, AddOrNoDisksForFind, FileOrDirectory, AddNewFileLefOrRight;

	bool chenge,bStatikL,bStatikR;

	wchar_t szBufl[MAX_PATH];
	wchar_t szBufr[MAX_PATH];
	wchar_t full_Filename_L1[MAX_PATH];
	wchar_t full_Filename_L2[MAX_PATH];
	wchar_t full_Filename[MAX_PATH];

	LV_ITEM lvItem ;

public:
	Edit(){
		indexLB=-1;
		indexLB1=-1;
		i=0;
		hWnd=NULL;
		FindNameOrText = 0;
		AddOrNoDisksForFind = 0;
		AddNewFileLefOrRight = 1;
	}
	//////////////////////////////////////////////////////////////////////////////////////////
	Edit(HWND tmp,HINSTANCE hInstens):hWnd(tmp),g_hinst(hInstens){}
	//////////////////////////////////////////////////////////////////////////////////////////
	void Asses(HWND tmp,HINSTANCE hInstens){
		hWnd=tmp;
		g_hinst=hInstens;
		FindFile(hListBox,hStatic);
		FindFile(hListBox1,hStatic1);
		VectorL(hStatic,PathNameL);
		VectorL(hStatic1,PathNameR);
	}
	//////////////////////////////////////////////////////////////////////////////////////////
	BOOL On_InitDialog(HWND hWnd, HWND hwndFocus, LPARAM lParam){
		wstring text,buf1;
		bStatikL=false;
		bStatikR=false;
		//-------------------------------------------------------
		hEdit=GetDlgItem(hWnd,IDC_EDIT1);
		//-------------------------------------------------------
		hSaveButt=GetDlgItem(hWnd,IDC_BUTTON1);
		//-------------------------------------------------------
		hListInfo=GetDlgItem(hWnd,IDC_LIST5);
		hListBox=GetDlgItem(hWnd,IDC_LIST1);
		hListBox1=GetDlgItem(hWnd,IDC_LIST2);
		//-------------------------------------------------------
		hCombo=GetDlgItem(hWnd,IDC_COMBO);
		hCombo1=GetDlgItem(hWnd,IDC_COMBO1);
		//-------------------------------------------------------
		hStatic=GetDlgItem(hWnd,IDC_STATIC2);
		hStatic1=GetDlgItem(hWnd,IDC_STATIC1);
		hStaticInfoL=GetDlgItem(hWnd,IDC_STATIC3);
		hStaticInfoR=GetDlgItem(hWnd,IDC_STATIC4);
		//-------------------------------------------------------
		hCheckA=GetDlgItem(hWnd,IDC_CHECK1);
		hCheckS=GetDlgItem(hWnd,IDC_CHECK2);
		hCheckNV=GetDlgItem(hWnd,IDC_CHECK3);
		hCheckR=GetDlgItem(hWnd,IDC_CHECK4);
		//-------------------------------------------------------
		Localsdrive(hCombo);
		Localsdrive(hCombo1);
		//-------------------------------------------------------
		int index1= SendMessage(hCombo,CB_SELECTSTRING,-1,(LPARAM)L"C");
		int index2= SendMessage(hCombo1,CB_SELECTSTRING,-1,(LPARAM)L"C");
		SendMessage(hCombo,CB_GETLBTEXT,index1,(LPARAM)text.c_str());
		SendMessage(hCombo1,CB_GETLBTEXT,index2,(LPARAM)buf1.c_str());
		//-------------------------------------------------------
		SetWindowText(hStatic,text.data());
		SetWindowText(hStatic1,buf1.data());
		SetWindowText(hStaticInfoL,buf1.data());
		SetWindowText(hStaticInfoR,buf1.data());
		//-------------------------------------------------------
		return true;
	}
	//////////////////////////////////////////////////////////////////////////////////////////
	void Localsdrive(HWND hComboTmp){//проверку на наличие привода вылетает прога
		AddOrNoDisksForFind++;
		wchar_t buf [128]={0};
		GetLogicalDriveStrings( 128, buf );
		for(int i=0,p=1; i<128; i++,p++){
			if(p%4==0){
				wchar_t tmp[34];
				for(int j=0,h=3; j<4;h--, j++)
					tmp[j]=buf[i-h];
				UINT part=GetDriveType(tmp);
				if(part!=DRIVE_NO_ROOT_DIR){
					SendMessage(hComboTmp,CB_ADDSTRING,0,(LPARAM)tmp);
					if(AddOrNoDisksForFind == 1){
						DisksForFind.push_back(tmp);
					}
				}

			}
		}
		if(AddOrNoDisksForFind == 1){
			vector<wstring>::iterator tmp_vector = DisksForFind.begin();
			DisksForFind.erase(tmp_vector);
		}

	}
	//////////////////////////////////////////////////////////////////////////////////////////
	void FindFile(HWND hList,HWND hStaticTmp){

		SendMessage(hList, LVM_DELETEALLITEMS, (WPARAM)0,(LPARAM)0);  
		i=0;
		wchar_t buf [MAX_PATH]={0};

		GetWindowText(hStaticTmp,buf,MAX_PATH);

		WIN32_FIND_DATA FindFileData;
		HANDLE hFind;
		wcscat(buf,L"*\0");

		hFind = FindFirstFile(buf, &FindFileData);
		if (hFind == INVALID_HANDLE_VALUE){
			MessageBox(0,L"Ошибка", L"Ненаиден",  MB_OK |MB_ICONWARNING);
		} 
		else 
		{
			do {
				Viev_List(FindFileData.cFileName,hList,i);
				++i;

			} while (FindNextFile(hFind, &FindFileData) != 0);

			FindClose(hFind); 
			InitListViewImageLists(hStaticTmp,hList,i);

		}

	}
	//////////////////////////////////////////////////////////////////////////////////////////
	void Viev_List( wchar_t buf[],HWND hList,int i){
		lvItem.mask = LVIF_IMAGE|LVIF_TEXT;
		lvItem.state = 0;
		lvItem.stateMask = 0 ;
		lvItem.iItem = i;
		lvItem.iImage=i;
		lvItem.iSubItem = 0;
		lvItem.pszText =buf;	
		lvItem.cchTextMax = wcslen (buf);
		ListView_InsertItem(hList, &lvItem);
	}
	//////////////////////////////////////////////////////////////////////////////////////////
	BOOL InitListViewImageLists(HWND hStaticTmp,HWND hWndListView,int size) 
	{ 
		HIMAGELIST hSmall; 
		SHFILEINFO lp;
		hSmall = ImageList_Create(GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), ILC_MASK|ILC_COLOR32, size, 1); 
		wchar_t buf [MAX_PATH]={0};

		GetWindowText(hStaticTmp,buf,MAX_PATH);

		WIN32_FIND_DATA FindFileData;
		HANDLE hFind;
		wcscat(buf,L"*\0");

		hFind = FindFirstFileW(buf, &FindFileData);
		if (hFind == INVALID_HANDLE_VALUE){
			MessageBox(0,L"Ошибка", L"Ненаиден",  MB_OK |MB_ICONWARNING);
		} 
		else 
		{
			do {
				if(wcscmp(FindFileData.cFileName,L".")==0){
					wchar_t buf1 [MAX_PATH]={0};
					GetWindowText(hStaticTmp,buf1,MAX_PATH);
					wcscat(buf1,FindFileData.cFileName);
					SHGetFileInfo(L"C:\\",FILE_ATTRIBUTE_DEVICE,&lp,sizeof(lp),SHGFI_ICONLOCATION|SHGFI_ICON|SHGFI_SMALLICON);

					ImageList_AddIcon(hSmall,lp.hIcon);
					DestroyIcon(lp.hIcon);

				}
				if(wcscmp(FindFileData.cFileName,L"..")==0){
					wchar_t buf1 [MAX_PATH]={0};
					GetWindowText(hStaticTmp,buf1,MAX_PATH);
					wcscat(buf1,FindFileData.cFileName);
					SHGetFileInfo(L"",FILE_ATTRIBUTE_DIRECTORY,&lp,sizeof(lp),SHGFI_ICONLOCATION|SHGFI_ICON|SHGFI_SMALLICON);

					ImageList_AddIcon(hSmall,lp.hIcon);
					DestroyIcon(lp.hIcon);
				}

				wchar_t buf1 [MAX_PATH]={0};
				GetWindowText(hStaticTmp,buf1,MAX_PATH);
				wcscat(buf1,FindFileData.cFileName);
				DWORD num=GetFileAttributes(buf1);
				SHGetFileInfo(buf1,num,&lp,sizeof(lp),SHGFI_ICONLOCATION|SHGFI_ICON|SHGFI_SMALLICON);

				ImageList_AddIcon(hSmall,lp.hIcon);
				DestroyIcon(lp.hIcon);

			} while (FindNextFile(hFind, &FindFileData) != 0);

			FindClose(hFind); 
		}

		ListView_SetImageList(hWndListView, hSmall, LVSIL_SMALL); 

		return TRUE; 
	}
	//////////////////////////////////////////////////////////////////////////////////////////
	int Size_Dir(HWND hStaticTmp,wstring sTmp){
		wstring buf1;
		buf1=sTmp;
		buf1+=L"*\0";
		wchar_t buffer [MAX_PATH]={0};
		WIN32_FIND_DATA FindFileData;
		HANDLE hFind;
		hFind = FindFirstFileW(buf1.data(), &FindFileData);
		if (hFind == INVALID_HANDLE_VALUE){
			//MessageBox(0,L"Ошибка", L"Ненаиден",  MB_OK |MB_ICONWARNING);
		} 
		else{
			do{
				if(FindFileData.dwFileAttributes!=FILE_ATTRIBUTE_DIRECTORY){
					int size=0;
					size+=FindFileData.nFileSizeLow;
					return size;
				}			
				else if(FindFileData.dwFileAttributes==FILE_ATTRIBUTE_DIRECTORY){
					if(wcsstr(FindFileData.cFileName, L".")==0){
						buf1.clear();
						buf1 +=sTmp;
						buf1 += FindFileData.cFileName;
						buf1 += L"\\";
						sizeDir.push_back(buf1);
					}
				}
			}while (FindNextFile(hFind, &FindFileData) != 0);
			FindClose(hFind);
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////
	void Info_File(HWND hWnd,HWND hStaticTmp,wchar_t szBuf[]){
		SendMessage(hListInfo,LB_RESETCONTENT,0,0);
		EnableWindow(hCheckA,MF_DISABLED);
		EnableWindow(hCheckS,MF_DISABLED);
		EnableWindow(hCheckNV,MF_DISABLED);
		EnableWindow(hCheckR,MF_DISABLED);
		SendMessage(hCheckA,BM_SETCHECK,(WPARAM)BST_UNCHECKED,0);
		SendMessage(hCheckS,BM_SETCHECK,(WPARAM)BST_UNCHECKED,0);
		SendMessage(hCheckNV,BM_SETCHECK,(WPARAM)BST_UNCHECKED,0);
		SendMessage(hCheckR,BM_SETCHECK,(WPARAM)BST_UNCHECKED,0);
		wchar_t buf [MAX_PATH]={0};
		GetWindowText(hStaticTmp,buf,MAX_PATH);
		BY_HANDLE_FILE_INFORMATION hFileInfo;
		WIN32_FIND_DATA FindFileData;
		HANDLE hFind;
		SYSTEMTIME stUTC, stLocal;
		wcscat(buf,szBuf);




		hFind = FindFirstFileW(buf, &FindFileData);
		if (hFind == INVALID_HANDLE_VALUE){
			//MessageBox(0,L"Ошибка", L"Ненаиден",  MB_OK |MB_ICONWARNING);
		} 
		else {

			wchar_t buffer[MAX_PATH];
			wchar_t szTmp[MAX_PATH]={0};
			wstring str;
			str+=L"Атрибут:  ";
			//////////////////////////////////////////////////////////////////////
			FileTimeToSystemTime(&FindFileData.ftCreationTime, &stUTC);
			SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
			StringCchPrintf( buffer, MAX_PATH, L"%02d/%02d/%d  %02d:%02d",stLocal.wMonth, stLocal.wDay, stLocal.wYear,stLocal.wHour, stLocal.wMinute);
			wsprintf(szTmp,L"Время Создания:   %s",buffer);
			SendMessage(hListInfo,LB_ADDSTRING,0,(LPARAM)szTmp);

			FileTimeToSystemTime(&FindFileData.ftLastAccessTime, &stUTC);
			SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
			StringCchPrintf( buffer, MAX_PATH, L"%02d/%02d/%d  %02d:%02d",stLocal.wMonth, stLocal.wDay, stLocal.wYear,stLocal.wHour, stLocal.wMinute);
			wsprintf(szTmp,L"Время Изменения:  %s",buffer);
			SendMessage(hListInfo,LB_ADDSTRING,0,(LPARAM)szTmp);

			FileTimeToSystemTime(&FindFileData.ftLastWriteTime, &stUTC);
			SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
			StringCchPrintf( buffer, MAX_PATH, L"%02d/%02d/%d  %02d:%02d",stLocal.wMonth, stLocal.wDay, stLocal.wYear,stLocal.wHour, stLocal.wMinute);
			wsprintf(szTmp,L"Время открытия:   %s",buffer);
			SendMessage(hListInfo,LB_ADDSTRING,0,(LPARAM)szTmp);
			////////////////////////////////////////////////////////////////////////
			if(FindFileData.dwFileAttributes!=FILE_ATTRIBUTE_DIRECTORY){
				wsprintf(szTmp,L"Размер:   %d",FindFileData.nFileSizeLow);
				SendMessage(hListInfo,LB_ADDSTRING,0,(LPARAM)szTmp);
			}

			if(FindFileData.dwFileAttributes==FILE_ATTRIBUTE_ARCHIVE){
				str+=L"A ";

				SendMessage(hCheckA,BM_SETCHECK,(WPARAM)BST_CHECKED,0);

			}
			if(FindFileData.dwFileAttributes==FILE_ATTRIBUTE_READONLY){
				str+=L"R ";
				SendMessage(hCheckR,BM_SETCHECK,(WPARAM)BST_CHECKED,0);
			}
			if(FindFileData.dwFileAttributes==FILE_ATTRIBUTE_DIRECTORY){
				str+=L" ";
				int size=0;
				wstring buf_Find;
				buf_Find = buf;
				buf_Find+=L"\\";
				Size_Dir( hStaticTmp,buf_Find);
				for(;sizeDir.empty()!=true;){
					buf_Find.clear();
					buf_Find += sizeDir[0];
					vector<wstring>::iterator tmp_vector = sizeDir.begin();
					sizeDir.erase(tmp_vector);
					size+=Size_Dir( hStaticTmp,buf_Find);
				}
				wsprintf(szTmp,L"Размер:   %d",size);
				SendMessage(hListInfo,LB_ADDSTRING,0,(LPARAM)szTmp);

			}
			if(FindFileData.dwFileAttributes==FILE_ATTRIBUTE_HIDDEN||FindFileData.dwFileAttributes==18){
				str+=L"H ";
				/*EnableWindow(hCheckA,MF_ENABLED);
				EnableWindow(hCheckS,MF_ENABLED);
				EnableWindow(hCheckNV,MF_ENABLED);
				EnableWindow(hCheckR,MF_ENABLED);*/
				SendMessage(hCheckNV,BM_SETCHECK,(WPARAM)BST_CHECKED,0);
			}
			if(FindFileData.dwFileAttributes==6||FindFileData.dwFileAttributes==22){
				str+=L"S H ";
				EnableWindow(hCheckA,MF_ENABLED);
				EnableWindow(hCheckS,MF_ENABLED);
				EnableWindow(hCheckNV,MF_ENABLED);
				EnableWindow(hCheckR,MF_ENABLED);
				SendMessage(hCheckNV,BM_SETCHECK,(WPARAM)BST_CHECKED,0);
				SendMessage(hCheckS,BM_SETCHECK,(WPARAM)BST_CHECKED,0);
			}
			if(FindFileData.dwFileAttributes==17){
				str+=L"R ";
				EnableWindow(hCheckA,MF_ENABLED);
				EnableWindow(hCheckS,MF_ENABLED);
				EnableWindow(hCheckNV,MF_ENABLED);
				EnableWindow(hCheckR,MF_ENABLED);
				SendMessage(hCheckR,BM_SETCHECK,(WPARAM)BST_CHECKED,0);
			}
			if(FindFileData.dwFileAttributes==16){
				str+=L" ";
				EnableWindow(hCheckA,MF_ENABLED);
				EnableWindow(hCheckS,MF_ENABLED);
				EnableWindow(hCheckNV,MF_ENABLED);
				EnableWindow(hCheckR,MF_ENABLED);
			}
			if(FindFileData.dwFileAttributes==39){
				str+=L"A R S H";
				EnableWindow(hCheckA,MF_ENABLED);
				EnableWindow(hCheckS,MF_ENABLED);
				EnableWindow(hCheckNV,MF_ENABLED);
				EnableWindow(hCheckR,MF_ENABLED);
				SendMessage(hCheckNV,BM_SETCHECK,(WPARAM)BST_CHECKED,0);
				SendMessage(hCheckS,BM_SETCHECK,(WPARAM)BST_CHECKED,0);
				SendMessage(hCheckA,BM_SETCHECK,(WPARAM)BST_CHECKED,0);
				SendMessage(hCheckR,BM_SETCHECK,(WPARAM)BST_CHECKED,0);
			}
			if(FindFileData.dwFileAttributes==8211){
				str+=L"R H ";
				EnableWindow(hCheckA,MF_ENABLED);
				EnableWindow(hCheckS,MF_ENABLED);
				EnableWindow(hCheckNV,MF_ENABLED);
				EnableWindow(hCheckR,MF_ENABLED);
				SendMessage(hCheckNV,BM_SETCHECK,(WPARAM)BST_CHECKED,0);
				SendMessage(hCheckR,BM_SETCHECK,(WPARAM)BST_CHECKED,0);
			}
			if(FindFileData.dwFileAttributes==FILE_ATTRIBUTE_SYSTEM){
				str+=L"S ";
				/*EnableWindow(hCheckA,MF_ENABLED);
				EnableWindow(hCheckS,MF_ENABLED);
				EnableWindow(hCheckNV,MF_ENABLED);
				EnableWindow(hCheckR,MF_ENABLED);*/
				SendMessage(hCheckS,BM_SETCHECK,(WPARAM)BST_CHECKED,0);
			}
			SendMessage(hListInfo,LB_ADDSTRING,0,(LPARAM)str.data());
			FindClose(hFind); 
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////
	void SaveBut(){
		DWORD dwSist=0,dwArhiv=0,dwNviz=0,dwRead=0;
		int lenEdit = SendMessage(hEdit, WM_GETTEXTLENGTH, 0, 0);
		wchar_t*bufEdit = new wchar_t [lenEdit+1];
		wchar_t bufPathName [MAX_PATH]={0};
		wchar_t bufPathNameTo [MAX_PATH]={0};
		SendMessage(hEdit,WM_GETTEXT,lenEdit+1,(LPARAM)bufEdit);
		if(bStatikL==true){
			SendMessage(hStatic,WM_GETTEXT,MAX_PATH,(LPARAM)bufPathName);
			SendMessage(hStatic,WM_GETTEXT,MAX_PATH,(LPARAM)bufPathNameTo);
			wcscat(bufPathName,szBufl);
		}
		if(bStatikR==true){
			SendMessage(hStatic1,WM_GETTEXT,MAX_PATH,(LPARAM)bufPathName);
			SendMessage(hStatic1,WM_GETTEXT,MAX_PATH,(LPARAM)bufPathNameTo);
			wcscat(bufPathName,szBufr);
		}

		wcscat(bufPathNameTo,bufEdit);
		bStatikL=false;
		bStatikR=false;
		//----------------------------------------------------------------//

		if(SendMessage(hCheckA,BM_GETCHECK,0,0)==true)dwArhiv=FILE_ATTRIBUTE_ARCHIVE;
		if(SendMessage(hCheckS,BM_GETCHECK,0,0)==true)dwSist=FILE_ATTRIBUTE_SYSTEM;
		if(SendMessage(hCheckNV,BM_GETCHECK,0,0)==true)dwNviz=FILE_ATTRIBUTE_HIDDEN;
		if(SendMessage(hCheckR,BM_GETCHECK,0,0)==true)dwRead=FILE_ATTRIBUTE_READONLY;

		SetFileAttributes(bufPathNameTo,dwSist|dwArhiv|dwNviz|dwRead);
		//----------------------------------------------------------------//
		HANDLE hFile = CreateFile(bufPathNameTo, GENERIC_ALL,FILE_SHARE_WRITE, NULL,OPEN_EXISTING,0 , NULL);
		FILETIME ft;
		SYSTEMTIME st;

		GetSystemTime(&st);             
		SystemTimeToFileTime(&st, &ft);  
		SetFileTime(hFile,(LPFILETIME) NULL,&ft,(LPFILETIME) NULL);
		//----------------------------------------------------------------//
		MoveFile(bufPathName,bufPathNameTo);
		FindFile(hListBox,hStatic);
		FindFile(hListBox1,hStatic1);



	}
	//////////////////////////////////////////////////////////////////////////////////////////
	void VectorL(HWND hStat, vector<wstring> & vec){
		wstring str;
		vec.clear();
		wchar_t bufPathName[MAX_PATH]={0};
		GetWindowText(hStat,bufPathName,MAX_PATH);
		str=bufPathName;
		vec.push_back(str);

	}
	//////////////////////////////////////////////////////////////////////////////////////////
	BOOL Cls_OnClose(HWND hWnd){


		DestroyWindow(hWnd);
		PostQuitMessage(0); 
		return TRUE;

	}
	//////////////////////////////////////////////////////////////////////////////////////////

	void Find_File(HWND hEdit,HWND hList){
		
		wstring buf1;
		buf1+=buf_Find;
		buf1+=L"*\0";
		wchar_t buffer [MAX_PATH]={0};
		GetWindowText(hEdit, buffer, MAX_PATH);
		WIN32_FIND_DATA FindFileData;
		HANDLE hFind;
		hFind = FindFirstFileW(buf1.data(), &FindFileData);
		if(wcscmp(FindFileData.cFileName,L"..")!=0)
			
			if (hFind == INVALID_HANDLE_VALUE){
				MessageBox(0,L"Ошибка", L"Ненаиден",  MB_OK |MB_ICONWARNING);
			} 
			else{
				do{
					if(FindFileData.dwFileAttributes!=FILE_ATTRIBUTE_DIRECTORY){
	
						if(wcsstr(FindFileData.cFileName, buffer)!=0){
							buf1.clear();
							buf1 += buf_Find;
							if(buf_Find == L"D:")
								buf1 += L"\\";
							buf1 += FindFileData.cFileName;
							buf1 += L"\\";
							SendMessage(hList,LB_ADDSTRING,0,(LPARAM)buf1.data());
						}
					}			
					else if(FindFileData.dwFileAttributes==FILE_ATTRIBUTE_DIRECTORY){
						if(wcsstr(FindFileData.cFileName, L".")==0){
							buf1.clear();
							buf1 += buf_Find;
							if(buf_Find == L"D:")
								buf1 += L"\\";
							buf1 += FindFileData.cFileName;
							buf1 += L"\\";
							DerictoryFind.push_back(buf1);
						}
					}
				}while (FindNextFile(hFind, &FindFileData) != 0);
				FindClose(hFind);
			}
	}

	void Find_TextInFile(HWND hEdit,HWND hList){
		wstring buf1;
		buf1+=buf_Find;
		buf1+=L"*\0";
		wchar_t buffer [MAX_PATH]={0};
		GetWindowText(hEdit, buffer, MAX_PATH);
		WIN32_FIND_DATA FindFileData;
		HANDLE hFind;
		hFind = FindFirstFileW(buf1.data(), &FindFileData);
		if(wcscmp(FindFileData.cFileName,L"..")!=0)
			
			if (hFind == INVALID_HANDLE_VALUE){
				MessageBox(0,L"Ошибка", L"Ненаиден",  MB_OK |MB_ICONWARNING);
			} 
			else{
				do{
					if(FindFileData.dwFileAttributes!=FILE_ATTRIBUTE_DIRECTORY){
						buf1.clear();
						buf1 += buf_Find;
						if(buf_Find.size() == 2)
							buf1 += L"\\";
						buf1 += FindFileData.cFileName;
						wifstream f;
					
						if(wcsstr(buf1.c_str(), L"txt")){
							f.open(buf1.c_str());
							if(f.is_open()){
								while(!f.eof()){
									wchar_t buf_File [1000];
									f.getline(buf_File, 1000);
									if(wcsstr(buf_File, buffer)!=0){
										SendMessage(hList,LB_ADDSTRING,0,(LPARAM)buf1.data());
									}
								}
								f.close();
							}
						}
					}			
					else if(FindFileData.dwFileAttributes==FILE_ATTRIBUTE_DIRECTORY){
						if(wcsstr(FindFileData.cFileName, L".")==0){
							buf1.clear();
							buf1 += buf_Find;
							if(buf_Find == L"D:")
								buf1 += L"\\";
							buf1 += FindFileData.cFileName;
							buf1 += L"\\";
							DerictoryFind.push_back(buf1);
						}
					}
				}while (FindNextFile(hFind, &FindFileData) != 0);
				FindClose(hFind);
			}
	}



};

class Dir:public Edit{
public:
	Dir(){}
	//////////////////////////////////////////////////////////////////////////////////////////
	int Pereimenovat(LPCWSTR pPath,LPCWSTR pPathRename)
	{
		//int index = ListView_GetNextItem(hListBox,-1, LVNI_SELECTED);
		SHFILEOPSTRUCTW sh;
		ZeroMemory(&sh,sizeof(SHFILEOPSTRUCTW));
		sh.hwnd=NULL;
		sh.wFunc=FO_RENAME;//Что делаем с файлом переименовать
		sh.pFrom=pPath; //имя директория,путь в формате Unicode, завершающийся 2 \0\0;
		sh.pTo=pPathRename;//как переименовываем 
		sh.fFlags=FOF_RENAMEONCOLLISION|FOF_WANTMAPPINGHANDLE|FOF_FILESONLY|FOF_ALLOWUNDO|FOF_SILENT|FOF_NOCONFIRMATION;//не требовать подтверждения и не выводить прогресс-бар
		sh.fAnyOperationsAborted=FALSE;//это поле сообщи,если нажата отмена выполнение функции
		return SHFileOperationW(&sh);//функция копирует,перемещает,переименовывает,удаляет файлы
	}
	//////////////////////////////////////////////////////////////////////////////////////////
	void Rename()
	{
		//код можно укаратить на половину
		int index=0,index1=0;
		index = ListView_GetNextItem(hListBox,-1, LVNI_SELECTED);
		index1 = ListView_GetNextItem(hListBox1,-1, LVNI_SELECTED);
		if(index==-1 && index1==-1)
		{
			MessageBox(hListBox1,L"Файл не выделен!", L"", MB_OK);
		}
		else if(bStatikL==true)
		{
			LV_ITEM item;
			memset(&item, 0, sizeof(item));

			item.mask = LVIF_TEXT;
			item.iSubItem=0;
			item.iItem=index;
			wchar_t baf [MAX_PATH]={0};
			ListView_GetItemText(hListBox,index,0,baf,MAX_PATH);
			GetWindowText(hCombo,full_Filename,200);
			wcscat(full_Filename,baf);
			DWORD dwAttr = GetFileAttributes(full_Filename);
			if(dwAttr==FILE_ATTRIBUTE_SYSTEM){MessageBox(hListBox,L"Файл системный,переименовать невозможно!", L"", MB_OK);}
			else
			{
				wcscat(full_Filename,L"\0");
				SetFocus(ListView_EditLabel(hListBox,index));
			}
		} 
		else if(bStatikR==true)
		{
			LV_ITEM item;
			memset(&item, 0, sizeof(item));

			item.mask = LVIF_TEXT;
			item.iSubItem=0;
			item.iItem=index1;
			wchar_t baf [MAX_PATH]={0};
			ListView_GetItemText(hListBox1,index1,0,baf,MAX_PATH);
			GetWindowText(hCombo1,full_Filename,200);
			wcscat(full_Filename,baf);
			DWORD dwAttr = GetFileAttributes(full_Filename);
			if(dwAttr==FILE_ATTRIBUTE_SYSTEM){MessageBox(hListBox,L"Файл системный,переименовать невозможно!", L"", MB_OK);}
			else
			{
				wcscat(full_Filename,L"\0");
				SetFocus(ListView_EditLabel(hListBox1,index1));
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////
	int Remove_dirW(LPCWSTR pPath)
	{
		SHFILEOPSTRUCTW sh;
		ZeroMemory(&sh,sizeof(SHFILEOPSTRUCTW));
		sh.hwnd=NULL;
		sh.wFunc=FO_DELETE;//Что делаем с файлом удалить
		sh.pFrom=pPath; //удаляемая директория,путь в формате Unicode, завершающийся 2 \0\0;
		sh.pTo=NULL;// куде его перемещать, переносить,но т.к. мы его удаляет то NULL
		sh.fFlags=FOF_NOCONFIRMATION | FOF_SILENT;//не требовать подтверждения и не выводить прогресс-бар
		sh.fAnyOperationsAborted=FALSE;//это поле сообщи,если нажата отмена выполнение функции
		return SHFileOperationW(&sh);//функция копирует,перемещает,переименовывает,удаляет файлы
	}
	//////////////////////////////////////////////////////////////////////////////////////////
	int Copy_dirW(LPCWSTR pPath,LPCWSTR pPathCopy)
	{
		SHFILEOPSTRUCTW sh;
		ZeroMemory(&sh,sizeof(SHFILEOPSTRUCTW));
		sh.hwnd=NULL;
		sh.wFunc=FO_COPY;//Что делаем с файлом удалить
		sh.pFrom=pPath; //удаляемая директория,путь в формате Unicode, завершающийся 2 \0\0;
		sh.pTo=pPathCopy;// куде его перемещать, переносить,но т.к. мы его удаляет то NULL
		sh.fFlags=FOF_RENAMEONCOLLISION;//не требовать подтверждения и не выводить прогресс-бар
		sh.fAnyOperationsAborted=FALSE;//это поле сообщи,если нажата отмена выполнение функции
		return SHFileOperationW(&sh);//функция копирует,перемещает,переименовывает,удаляет файлы
	}
	//////////////////////////////////////////////////////////////////////////////////////////
	int Peremestit_dirW(LPCWSTR pPath,LPCWSTR pPathCopy)
	{
		SHFILEOPSTRUCTW sh;
		ZeroMemory(&sh,sizeof(SHFILEOPSTRUCTW));
		sh.hwnd=NULL;
		sh.wFunc=FO_MOVE;//Что делаем с файлом удалить
		sh.pFrom=pPath; //удаляемая директория,путь в формате Unicode, завершающийся 2 \0\0;
		sh.pTo=pPathCopy;// куде его перемещать, переносить,но т.к. мы его удаляет то NULL
		sh.fFlags=FOF_RENAMEONCOLLISION;//не требовать подтверждения и не выводить прогресс-бар
		sh.fAnyOperationsAborted=FALSE;//это поле сообщи,если нажата отмена выполнение функции
		return SHFileOperationW(&sh);//функция копирует,перемещает,переименовывает,удаляет файлы
	}
	void Move()
	{

		int index=0,index1=0;
		index = ListView_GetNextItem(hListBox,-1, LVNI_SELECTED);
		index1 = ListView_GetNextItem(hListBox1,-1, LVNI_SELECTED);
		if(index==-1 && index1==-1)
		{
			MessageBox(hListBox1,L"Файл не выделен!", L"", MB_OK);
		}
		else if(bStatikL==true)
		{

			LV_ITEM item;
			memset(&item, 0, sizeof(item));
			item.mask = LVIF_TEXT;
			item.iSubItem=0;
			item.iItem=index;
			wchar_t baf [MAX_PATH]={0};
			wchar_t bef [MAX_PATH]={0};
			ListView_GetItemText(hListBox,index,0,baf,MAX_PATH);
			wchar_t full_Filename[MAX_PATH]={0};
			GetWindowText(hCombo,full_Filename,200);
			GetWindowText(hCombo1,bef,200);
			wcscat(bef,baf);
			wcscat(full_Filename,baf);
			DWORD dwAttr = GetFileAttributes(full_Filename);
			if(dwAttr==FILE_ATTRIBUTE_SYSTEM){MessageBox(hListBox,L"Файл системный,переместить невозможно!", L"", MB_OK);}
			else
			{
				wcscat(full_Filename,L"\0");
				wcscat(bef,L"\0");
				Peremestit_dirW(full_Filename,bef);
				SendMessage(hListBox,LVM_DELETEITEM,index,0);
				int col=ListView_GetItemCount(hListBox1);//LVM_SETITEMTEXT изменение текста в поделементе
				Viev_List(baf,hListBox1,col+1);
				index=-1;
			}

		} 
		else if(bStatikR==true)
		{
			LV_ITEM item;
			memset(&item, 0, sizeof(item));

			item.mask = LVIF_TEXT;
			item.iSubItem=0;
			item.iItem=index1;
			wchar_t baf [MAX_PATH]={0};
			wchar_t bef [MAX_PATH]={0};
			ListView_GetItemText(hListBox1,index1,0,baf,MAX_PATH);
			wchar_t full_Filename[MAX_PATH]={0};
			GetWindowText(hCombo1,full_Filename,200);
			GetWindowText(hCombo,bef,200);
			wcscat(bef,baf);
			wcscat(full_Filename,baf);
			DWORD dwAttr = GetFileAttributes(full_Filename);
			if(dwAttr==FILE_ATTRIBUTE_SYSTEM){MessageBox(hListBox1,L"Файл системный,переместить невозможно!", L"", MB_OK);}
			else
			{
				wcscat(full_Filename,L"\0");
				wcscat(bef,L"\0");
				Peremestit_dirW(full_Filename,bef);
				SendMessage(hListBox1,LVM_DELETEITEM,index1,0);
				int col=ListView_GetItemCount(hListBox);
				Viev_List(baf,hListBox,col+1);
				index1=-1;
			}
		}
		FindFile(hListBox,hStatic);
		FindFile(hListBox1,hStatic1);

	}
	//////////////////////////////////////////////////////
	void Delete(){
		int idd=MessageBox(hWnd, L"Оно вам надо?", L"FaR",  MB_YESNO |MB_ICONWARNING);
		if (IDYES==idd){
		int index=0,index1=0;
		index = ListView_GetNextItem(hListBox,-1, LVNI_SELECTED);
		index1 = ListView_GetNextItem(hListBox1,-1, LVNI_SELECTED);
		if(index==-1 && index1==-1)
		{
			MessageBox(hListBox,L"Файл не выделен!", L"", MB_OK);
		}
		else if(bStatikL==true)
		{
			LV_ITEM item;
			memset(&item, 0, sizeof(item));

			item.mask = LVIF_TEXT;
			item.iSubItem=0;
			item.iItem=index;
			wchar_t baf [MAX_PATH]={0};
			ListView_GetItemText(hListBox,index,0,baf,MAX_PATH);
			SendMessage(hListBox,LVM_DELETEITEM,index,0);
			wchar_t full_Filename[MAX_PATH]={0};
			GetWindowText(hCombo,full_Filename,200);
			wcscat(full_Filename,baf);
			DWORD dwAttr = GetFileAttributes(full_Filename);
			if(dwAttr==FILE_ATTRIBUTE_SYSTEM){MessageBox(hListBox,L"Файл системный,удалить невозможно!", L"", MB_OK);}
			else
			{

				wcscat(full_Filename,L"\0");
				Remove_dirW(full_Filename);
			}

		} 
		else if(bStatikR==true)
		{
			LV_ITEM item;
			memset(&item, 0, sizeof(item));

			item.mask = LVIF_TEXT;
			item.iSubItem=0;
			item.iItem=index1;
			wchar_t baf [MAX_PATH]={0};
			ListView_GetItemText(hListBox1,index1,0,baf,MAX_PATH);
			SendMessage(hListBox1,LVM_DELETEITEM,index1,0);
			wchar_t full_Filename[MAX_PATH]={0};
			GetWindowText(hCombo1,full_Filename,200);
			wcscat(full_Filename,baf);
			DWORD dwAttr = GetFileAttributes(full_Filename);
			if(dwAttr==FILE_ATTRIBUTE_SYSTEM){MessageBox(hListBox,L"Файл системный,удалить невозможно!", L"", MB_OK);}
			else
			{

				wcscat(full_Filename,L"\0");
				Remove_dirW(full_Filename);
			}

		}
		FindFile(hListBox,hStatic);
		FindFile(hListBox1,hStatic1);
					}
			else if (IDNO==idd){
			       //break;
					}

	}
	//////////////////////////////////////////////////////////////////////////////////////////
	void Copy()
	{

		int index=0,index1=0;
		index = ListView_GetNextItem(hListBox,-1, LVNI_SELECTED);
		index1 = ListView_GetNextItem(hListBox1,-1, LVNI_SELECTED);
		if(index==-1 && index1==-1)
		{
			MessageBox(hListBox1,L"Файл не выделен!", L"", MB_OK);
		}
		else if(bStatikL==true)
		{

			LV_ITEM item;
			memset(&item, 0, sizeof(item));
			item.mask = LVIF_TEXT;
			item.iSubItem=0;
			item.iItem=index;
			wchar_t baf [MAX_PATH]={0};
			wchar_t bef [MAX_PATH]={0};
			ListView_GetItemText(hListBox,index,0,baf,MAX_PATH);
			wchar_t full_Filename[MAX_PATH]={0};
			GetWindowText(hCombo,full_Filename,200);
			GetWindowText(hCombo1,bef,200);
			wcscat(bef,baf);
			wcscat(full_Filename,baf);
			DWORD dwAttr = GetFileAttributes(full_Filename);
			if(dwAttr==FILE_ATTRIBUTE_SYSTEM){MessageBox(hListBox,L"Файл системный,копировать невозможно!", L"", MB_OK);}
			else
			{
				wcscat(full_Filename,L"\0");
				wcscat(bef,L"\0");
				Copy_dirW(full_Filename,bef);
				int col=ListView_GetItemCount(hListBox1);//LVM_SETITEMTEXT изменение текста в поделементе
				Viev_List(baf,hListBox1,col+1);
				index=-1;
			}

		} 
		else if(bStatikR==true)
		{
			LV_ITEM item;
			memset(&item, 0, sizeof(item));

			item.mask = LVIF_TEXT;
			item.iSubItem=0;
			item.iItem=index1;
			wchar_t baf [MAX_PATH]={0};
			wchar_t bef [MAX_PATH]={0};
			ListView_GetItemText(hListBox1,index1,0,baf,MAX_PATH);
			wchar_t full_Filename[MAX_PATH]={0};
			GetWindowText(hCombo1,full_Filename,200);
			GetWindowText(hCombo,bef,200);
			wcscat(bef,baf);
			wcscat(full_Filename,baf);
			DWORD dwAttr = GetFileAttributes(full_Filename);
			if(dwAttr==FILE_ATTRIBUTE_SYSTEM){MessageBox(hListBox1,L"Файл системный,копировать невозможно!", L"", MB_OK);}
			else
			{
				wcscat(full_Filename,L"\0");
				wcscat(bef,L"\0");
				Copy_dirW(full_Filename,bef);
				int col=ListView_GetItemCount(hListBox);
				Viev_List(baf,hListBox,col+1);
				index1=-1;
			}
		}
		FindFile(hListBox,hStatic);
		FindFile(hListBox1,hStatic1);

	}
	//////////////////////////////////////////////////////////////////////////////////////////
	void FileOpen_L1(HWND hWnd, HWND hStmp,HWND hLisTmp,wchar_t* full_Filename)
	{

		if (!wcsrchr(full_Filename, L'.')&& !wcsstr(full_Filename, L".."))
		{
			wcscat(full_Filename, L"\\");
			SetWindowText(hStmp, full_Filename);
			FindFile(hLisTmp,hStmp);
		}
		else 
		{
			auto char buff[120 + MAX_PATH];
			int res;
			if((res = ShellExecute(hWnd, L"open", full_Filename, NULL, NULL, SW_SHOW)<(HINSTANCE)32))
			{ 
				switch (res) 
				{
				case 0: 
					sprintf(buff,"The operating system is out of memory or resources (\"%s\").",full_Filename);
					MessageBoxA(hWnd, "Error", buff, MB_ICONERROR);
					break;
				case ERROR_FILE_NOT_FOUND:
					sprintf(buff,"The specified file (\"%s\") was not found.",full_Filename);
					MessageBoxA(hWnd, "Error", buff, MB_ICONERROR);
					break;
				case ERROR_PATH_NOT_FOUND:
					sprintf(buff,"The specified path was not found for (\"%s\").",full_Filename);
					MessageBoxA(hWnd, "Error", buff, MB_ICONERROR);
					break;
				case ERROR_BAD_FORMAT: 
					sprintf(buff,"The .EXE file (\"%s\") is invalid (non-Win32 .EXE or error in .EXE image).",full_Filename);
					MessageBoxA(hWnd, "Error", buff, MB_ICONERROR);
					break;
				case SE_ERR_ACCESSDENIED:
					sprintf(buff,"The operating system denied access to the specified file (\"%s\").",full_Filename);
					MessageBoxA(hWnd, "Error", buff, MB_ICONERROR);
					break;
				case SE_ERR_ASSOCINCOMPLETE:
					sprintf(buff,"The filename association to (\"%s\") is incomplete or invalid.",full_Filename);
					MessageBoxA(hWnd, "Error", buff, MB_ICONERROR);
					break;
				case SE_ERR_DDEBUSY:
					sprintf(buff,"The DDE transaction could not be completed because other DDE transactions were being processed for (\"%s\").",full_Filename);
					MessageBoxA(hWnd, "Error", buff, MB_ICONERROR);
					break;
				case SE_ERR_DDEFAIL:
					sprintf(buff,"The DDE transaction failed to (\"%s\").",full_Filename);
					MessageBoxA(hWnd, "Error", buff, MB_ICONERROR);
					break;
				case SE_ERR_DDETIMEOUT:
					sprintf(buff,"The DDE transaction could not be completed because the request timed out (\"%s\").",full_Filename);
					MessageBoxA(hWnd, "Error", buff, MB_ICONERROR);
					break;
				case SE_ERR_DLLNOTFOUND:
					sprintf(buff,"The specified dynamic-link library was not found (\"%s\").",full_Filename);
					MessageBoxA(hWnd, "Error", buff, MB_ICONERROR);
					break;
				case SE_ERR_NOASSOC:
					sprintf(buff,"There is no application associated with the given filename extension for (\"%s\").",full_Filename);
					MessageBoxA(hWnd, "Error", buff, MB_ICONERROR);
					break;
				case SE_ERR_OOM:
					sprintf(buff,"There was not enough memory to complete the operation.");
					MessageBoxA(hWnd, "Error", buff, MB_ICONERROR);
					break;
				case SE_ERR_SHARE:
					sprintf(buff,"A sharing violation occurred with (\"%s\").",full_Filename);
					MessageBoxA(hWnd, "Error", buff, MB_ICONERROR);
					break;
				default:
					sprintf(buff,"unknown error starting (\"%s\").",full_Filename);
					MessageBoxA(hWnd, "Error", buff, MB_ICONERROR);\
				};
			}
		}
	}

	void OpenFileL(Dir &obj)
	{
		LV_ITEM lvItem ;

		wchar_t bufTmp[MAX_PATH]={0};
		int index = ListView_GetNextItem(obj.hListBox,-1, LVNI_SELECTED);
		if(index == -1){}
		else{
			wstring str;
			int check = ListView_GetNextItem(obj.hListBox,-1,LVNI_SELECTED);
			if(check==-1){}
			else
			{
				LV_ITEM item_L1;
				memset(&item_L1, 0, sizeof(item_L1));
				item_L1.mask = LVIF_TEXT;
				item_L1.iSubItem=0;
				item_L1.iItem=check;

				wchar_t buf_name [MAX_PATH]={0};
				ListView_GetItemText(obj.hListBox,check,0,buf_name,MAX_PATH);

				GetWindowText(obj.hStatic, obj.full_Filename_L1, MAX_PATH);
				wcscat(obj.full_Filename_L1, buf_name);
				str=obj.full_Filename_L1;
				obj.PathNameL.push_back(str);
				///////////////////////////////////
				if (wcsstr(obj.full_Filename_L1, L".."))
				{
					wstring strTmp;

					if(obj.PathNameL.size()>1)obj.PathNameL.pop_back();
					obj.PathNameL.erase(obj.PathNameL.end()-1, obj.PathNameL.end());

					strTmp=obj.PathNameL.back();
					if(obj.PathNameL.size()>1)strTmp+=L"\\";

					SetWindowText(obj.hStatic,strTmp.data());
					obj.FindFile(obj.hListBox,obj.hStatic);

				}
				else if ((wcscmp(buf_name, L".")==0)){
					wstring strTmp;
					obj.PathNameL.erase(obj.PathNameL.begin()+1, obj.PathNameL.end());

					strTmp=obj.PathNameL.back();
					SetWindowText(obj.hStatic,strTmp.data());
					obj.FindFile(obj.hListBox,obj.hStatic);
				}
				else
					///////////////////////////////////////////
					obj.FileOpen_L1(hWnd, obj.hStatic, obj.hListBox, obj.full_Filename_L1);
			}

		}
	}

	void OpenFileR(Dir &obj)
	{
						 LV_ITEM lvItem ;
				 wchar_t bufTmp[MAX_PATH]={0};
				 int index = ListView_GetNextItem(obj.hListBox1,-1, LVNI_SELECTED);
				 if(index == -1){}
				 else{

					 int check = ListView_GetNextItem(obj.hListBox1,-1,LVNI_SELECTED);
					 if(check==-1){}
					 else
					 {
						  wstring str;
						 LV_ITEM item_L2;
						 memset(&item_L2, 0, sizeof(item_L2));
						 item_L2.mask = LVIF_TEXT;
						 item_L2.iSubItem=0;
						 item_L2.iItem=check;

						 wchar_t buf_name [MAX_PATH]={0};
						 ListView_GetItemText(obj.hListBox1,check,0,buf_name,MAX_PATH);

						 GetWindowText(obj.hStatic1, obj.full_Filename_L2, MAX_PATH);
						 wcscat(obj.full_Filename_L2, buf_name);
						  str=obj.full_Filename_L2;
				         obj.PathNameR.push_back(str);
                         ///////////////////////////////////
						 if (wcsstr(obj.full_Filename_L2, L".."))
						 {
							 wstring strTmp;
                            
							 if(obj.PathNameR.size()>1)obj.PathNameR.pop_back();
							 obj.PathNameR.erase(obj.PathNameR.end()-1, obj.PathNameR.end());

							 strTmp=obj.PathNameR.back();
							 if(obj.PathNameR.size()>1)strTmp+=L"\\";

							 SetWindowText(obj.hStatic1,strTmp.data());
							 obj.FindFile(obj.hListBox1,obj.hStatic1);

						 }
						 else if ((wcscmp(buf_name, L".")==0)){
							 wstring strTmp;
							 obj.PathNameR.erase(obj.PathNameR.begin()+1, obj.PathNameR.end());

							 strTmp=obj.PathNameR.back();
							 SetWindowText(obj.hStatic1,strTmp.data());
							 obj.FindFile(obj.hListBox1,obj.hStatic1);
						 }
						 else
							 ///////////////////////////////////////////
						 obj.FileOpen_L1(hWnd,obj.hStatic1,obj.hListBox1, obj.full_Filename_L2);
					 }
				 }
	}
	//////////////////////////////////////////////////////////////////////////////////////////
};