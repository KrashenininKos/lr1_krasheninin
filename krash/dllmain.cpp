// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"
#include <string>
#include <Windows.h>
#include <iostream>
#include "dllHeader.h"

BOOL APIENTRY krash( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

extern "C" __declspec(dllexport) std::string isExist()
{   int aASCIIcode = 65;//97
    int zASCIIcode = 90;//122
    std::string dllString = "";
    std::string Exist = "+";
    std::string notExist = "-";
    std::string commonStaff = "\\\\.\\";
    for (aASCIIcode; aASCIIcode < zASCIIcode; aASCIIcode++)
    {   std::string fullPath = commonStaff + char(aASCIIcode) + ":";
        HANDLE fileHandle = CreateFileA(fullPath.c_str(), GENERIC_READ,FILE_SHARE_READ | FILE_SHARE_WRITE,NULL, OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
        if (fileHandle != INVALID_HANDLE_VALUE)
        {
            dllString += Exist;
            CloseHandle(fileHandle);
        }
        else
        {
            dllString += notExist;
        } 
        
    }
    return dllString;
}

extern "C" __declspec(dllexport) bool isNTFSExist(char userLetter,NTFSBoot* structureToSend)
{
    std::string commonStaff = "\\\\.\\";
    std::string fullPath = commonStaff + userLetter + ":";
    HANDLE fileHandle = CreateFileA(fullPath.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    ULONGLONG startOffset = 0;
    BYTE buffer[512];
    memset(buffer, 0, 512);
    DWORD bytesToRead = 512;
    DWORD bytesRead;
    LARGE_INTEGER sectorOffset;
    sectorOffset.QuadPart = 0;
    unsigned long currentPosition = SetFilePointer(fileHandle, sectorOffset.LowPart, NULL, FILE_BEGIN);
    bool stopSpamTrash = ReadFile(fileHandle, buffer, bytesToRead, &bytesRead, NULL);
    *structureToSend = *reinterpret_cast<NTFSBoot*>(buffer);
    CloseHandle(fileHandle);
    BYTE* gotByte= structureToSend->sysName;
    std::string resultSysName = std::string((char*)(gotByte), 8);
    if (resultSysName == "NTFS    ")
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
    // изменения 
}
