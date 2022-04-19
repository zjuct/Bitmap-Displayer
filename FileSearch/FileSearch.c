#include"FileSearch.h"
#include<Windows.h>
#include<strsafe.h>

#define MAX_SIZE 100
#define MAX_LEN  150

static char* res[MAX_SIZE];

int pos = 0;
//在directory中递归地查找名为target的文件,返回一个字符串数组
void InitSearch() {
    for (int i = 0;i < MAX_SIZE;i++) {
        res[i] = (char*)malloc(sizeof(char) * MAX_LEN);
    }
    pos = 0;
}

void reset() {
    pos = 0;
}

char** file_search(char* target,char* directory) {

	char path[100];
	strcpy(path, directory, strlen(directory));
	StringCchCat(path, 10, "\\*");
	query(target, path);
    if (pos == 0) {
        return NULL;
    }
    else {
        return res;
    }
}

void query(char* target, char* directory) {
	WIN32_FIND_DATA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	hFind = FindFirstFile(directory, &ffd);

	if (hFind = INVALID_HANDLE_VALUE) {
        return;
	}
    do
    {
        if (strcmp(ffd.cFileName, ".") == 0 || strcmp(ffd.cFileName, "..") == 0) {
            continue;
        }
        if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            TCHAR newpath[60];
            StringCchCopy(newpath, strlen(directory), directory);
            StringCchCat(newpath, 60, ffd.cFileName);
            StringCchCat(newpath, 60, "\\*");
            traverse(newpath);
            //_tprintf(TEXT("  %s   <DIR>\n"), ffd.cFileName);
        }
        else
        {
            if (strcmp(target, ffd.cFileName) == 0) {
                char resolute_path[100];
                strcpy(resolute_path, directory);
                strcat(resolute_path, "\\");
                strcat(resolute_path, ffd.cFileName);
                strcpy(res[pos++], resolute_path);
            }
        }
    } while (FindNextFile(hFind, &ffd) != 0);
}