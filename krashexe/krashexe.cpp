#include <Windows.h>
#include <iostream>
#include <string>
#include "mainHeader.h"

void terminalShow()
{
    std::cout << "\n" << "insert any character to exit" << std::endl;
    std::string x;
    std::cin >> x;
}

int main()
{
    HMODULE hLib = LoadLibrary(L"krash.dll");
    if (hLib == NULL)
    {
        std::cout << "No able to attach dll !" << std::endl;
    }
    else
    {
        std::cout << "Library attached successfully !" << std::endl;
        isFileExistType isFileExist = (isFileExistType)GetProcAddress(hLib, "isExist");
        isNTFSExistType isNTFSExist = (isNTFSExistType)GetProcAddress(hLib, "isNTFSExist");
        if (!isFileExist or !isNTFSExist)
        {
            std::cout << "Got an error with getting some process adress...stopping" << std::endl;
        }
        else
        {   
            std::cout << "Succsess with getting all proc addresses !" << std::endl;
            std::string allFilesList = isFileExist();
            int existingVolumesCount = 0;
            int symbolZ = 25;
            char volumeExist = '+';
    
            for (byte symbol = 0; symbol <= symbolZ; symbol++)
            {
                if (allFilesList[symbol] == volumeExist)
                {
                    existingVolumesCount++;
                    std::cout << "  " << char(65+symbol) << " volume found!"<<std::endl;
                }
            };
            if (existingVolumesCount == 0)
            {
                std::cout << "No volumes found ...stopping" << std::endl;
            }
            else
            {
                bool needToRepeat = TRUE;
                std::cout << "Summary volumes found : " << existingVolumesCount << " !" << std::endl;
                while (needToRepeat == TRUE)
                {
                    std::cout << "Select volume you are interested in " << std::endl;
                    char userVolume;
                    std::cin >> userVolume;
                    while ((int(userVolume) < 65) or (int(userVolume) > 90) and (int(userVolume) < 97) or (int(userVolume) > 122))
                    {
                        std::cout << "No existing drive (insert a letter from A to Z in any case)" << std::endl;
                        std::cin >> userVolume;
                    }
                    int userVolumeToInt = NULL;
                    if (int(userVolume) > 95)
                    {
                        userVolumeToInt = int(userVolume) - 32;
                    }
                    else
                    {
                        userVolumeToInt = int(userVolume);
                    }
                    std::cout << "Choosen volume: " << char(userVolumeToInt) << std::endl;
                    NTFSBoot_main myStructure;
                    if (isNTFSExist(userVolume,myStructure) == TRUE)
                    {
                        needToRepeat = FALSE;
                    }
                    else
                    {
                        std::cout << "Choosen one volume is not NTFS, try again"<<std::endl;
                        needToRepeat = TRUE;
                    }
                    
                    std::cout << " FS type " << myStructure.sysName << std::endl;
                    std::cout << "Bytes per sector " << myStructure.bytesOnSector << std::endl;
                    std::cout << " Sectors per cluster: " << WORD(myStructure.sectorOnCluster)<< std::endl;
                    std::cout << " Descriptor:" << WORD(myStructure.mediaDescriptor) << std::endl;
                    std::cout << " Sectors per track: " << myStructure.sectorPerTrack << std::endl;
                    std::cout << " Number of heads: " << myStructure.numberOfHeaders << std::endl;
                    std::cout << " Number of sectors in the volume: " << myStructure.sectorsInTheVolume << std::endl;
                    std::cout << " LCN of VCN 0 of the $MFT: " << myStructure.lcnOfMFT << std::endl;
                    std::cout << " LCN of VCN 0 of the $MFTMirr: " << myStructure.lcnOfSMFTMirr << std::endl;
                    std::cout << " Clusters per MFT Record: " << myStructure.clusterPerMftRecord << std::endl;
                    std::cout << " Clusters per Index Record: " << WORD(myStructure.clusterPerIndexRecord) << std::endl;
                    std::cout << " Volume serial number: " << myStructure.volumeID << std::endl;
                }
            }
        }
        FreeLibrary(hLib);
        terminalShow();
    }
}

