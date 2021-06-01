//

#include "lwDirectoryBrowser.h" 

LW_BEGIN

// lwDirectoryBrowser
LW_STD_IMPLEMENTATION(lwDirectoryBrowser)

lwDirectoryBrowser::lwDirectoryBrowser()
: _proc(0), _param(0)
{
}

LW_RESULT lwDirectoryBrowser::_Go(const char* file, DWORD flag)
{
    LW_RESULT ret = LW_RET_OK;

    WIN32_FIND_DATA wfd;

    HANDLE handle = ::FindFirstFile(file, &wfd);

    if(handle == INVALID_HANDLE_VALUE)
        goto __ret;

    char file_path[260];
    char file_spec[64];

    strcpy(file_path, file);
    char* p = strrchr(file_path, '\\');
    if(p == 0)
        goto __ret;

    strcpy(file_spec, &p[1]);
    p[1] = '\0';

    do
    {
        if(wfd.cFileName[0] == '.')
        {
            if((wfd.cFileName[1] == '\0')
                || (wfd.cFileName[1] == '.' && wfd.cFileName[2] == '\0'))
            {
                continue;
            }
        }

        if((!(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) && (flag & DIR_BROWSE_FILE))
        {
            if(LW_FAILED((*_proc)(file_path, &wfd, _param)))
            {
                ret = LW_RET_OK_1;
                goto __ret;
            }
        }
        else if((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && (flag & DIR_BROWSE_DIRECTORY))
        {
            if(LW_FAILED((*_proc)(file_path, &wfd, _param)))
            {
                ret = LW_RET_OK_1;
                goto __ret;
            }

            char sub_file[260];
            sprintf(sub_file, "%s%s\\%s", file_path, wfd.cFileName, file_spec);

            if((ret = _Go(sub_file, flag)) == LW_RET_OK_1)
                goto __ret;
        }

    } while(::FindNextFile(handle, &wfd));


__ret:
    ::FindClose(handle);

    return ret;
}

LW_RESULT lwDirectoryBrowser::Browse(const char *file, DWORD flag)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_proc == 0)
        goto __ret;

    //char* p = strrchr(file, '\\');
    //if((p == 0) || (p[1] == '\0'))
    //    goto __ret;

    ret = _Go(file, flag);

__ret:
    return ret;
}

LW_END
