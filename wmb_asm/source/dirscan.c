/*
Wmb Asm, the SDK, and all software in the Wmb Asm package are licensed under the MIT license:
Copyright (c) 2008 - 2010 yellowstar

Permission is hereby granted, free of charge, to any person obtaining a copy of this
software and associated documentation files (the �Software�), to deal in the Software
without restriction, including without limitation the rights to use, copy, modify, merge,
publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies
or substantial portions of the Software.

THE SOFTWARE IS PROVIDED �AS IS�, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#define BUILDING_SDK

#include "wmb_asm_sdk.h"

inline void AllocDir(FILE_LIST *files)
{

        files->filename = (char*)malloc(256);
        strcpy(files->filename,"");
        files->next = (FILE_LIST*)malloc(sizeof(FILE_LIST));
        memset(files->next,0,sizeof(FILE_LIST));
}

void FreeDirectory(FILE_LIST *filelist)
{

    FILE_LIST *files = filelist;
    FILE_LIST *next_file = files;
    files=next_file;
    while(files!=NULL)
    {
        next_file = files->next;
        free(files->filename);
        free(files);
        files = next_file;
    }
}

FILE_LIST *ScanDirectory(FILE_LIST *filelist, char *dirname, char *ext)
{

    FILE_LIST *files=NULL;
    FILE_LIST *cur_files=NULL;
    DIR *dir = NULL;
    struct dirent *ent = NULL;
    char *DirName = (char*)malloc(256);
    strncpy(DirName,dirname,256);
    if(filelist!=NULL)
    {
        files = filelist;
        while(files->next!=NULL)
        {
            files = files->next;
        }

        if(files->filename!=NULL && files->next!=NULL)files = files->next;


    }

    cur_files = files;

    FILE *f = fopen(DirName,"rb");
    if(f!=NULL)
    {
        if(ext!=NULL)
        {
            if(strcmp((const char*)&DirName[strlen(DirName)-4],(const char*)ext)!=0)
            {
                return files;
            }
        }

        if(cur_files==NULL)
        {
            cur_files = (FILE_LIST*)malloc(sizeof(FILE_LIST));
            memset(cur_files,0,sizeof(FILE_LIST));
        }

        if(cur_files->filename==NULL)
        AllocDir(cur_files);

        fclose(f);
        strcpy(cur_files->filename,DirName);
        return files;
    }

    char c = DirName[strlen(DirName)];

    if(c!='/' && c!='\\')
    {
    sprintf(DirName,"%s\\",DirName);
    }

    dir=opendir(DirName);
    if(dir==NULL)
    {
        return NULL;
    }

    char *str;
    str = (char*)malloc(256);
    strcpy(str,"");
    int skip=0;

    if(files==NULL)
    {
        if(files==NULL)
        {
            files = (FILE_LIST*)malloc(sizeof(FILE_LIST));
            memset(files,0,sizeof(FILE_LIST));
        }

        AllocDir(files);
        cur_files = files;
    }

    ent=(struct dirent*)1;
    while(ent!=NULL)
    {
        ent = readdir(dir);
        if(ent==NULL)break;

        if(skip<2)
        {
            skip++;
            continue;
        }
        else
        {

            snprintf(str, 256, "%s%s",DirName,ent->d_name);
            f = fopen(str,"rb");

            if(f==NULL)
            {

                char *Str = (char*)malloc(256);
                strncpy(Str,str, 256);

                    if(ScanDirectory(cur_files,Str,ext)==NULL)
                    {
                        printf("Failed to open file or directory %s\n",Str);
                    }
                    else
                    {
                        while(cur_files->next!=NULL)
                        {
                            cur_files = cur_files->next;
                        }

                        if(cur_files->filename!=NULL && cur_files->next!=NULL)cur_files = cur_files->next;
                    }

                free(Str);
            }
            else
            {
                fclose(f);

                    if(ext!=NULL)
                    {
                        if(strcmp((const char*)&ent->d_name[strlen(ent->d_name)-4],(const char*)ext)!=0)
                        {
                            continue;
                        }
                    }

                if(cur_files->filename==NULL)
                AllocDir(cur_files);

                strcpy(cur_files->filename,str);

                cur_files = cur_files->next;
            }
        }
    }

    closedir(dir);
    free(str);
    free(DirName);

    return files;
}
