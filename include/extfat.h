#pragma once

    // This header defines the layout of data on an extfat disk image.
    
    // For the details, please refer to

    //            https://learn.microsoft.com/en-gb/windows/win32/fileio/exfat-specification
    typedef struct 
    {
        char JumpBoot[3];
        char FileSystemName[8];
        char MustBeZero[53];
        long int PartitionOffset;
        long int VolumeLength;
        int FatOffset;
        int FatLength;
        int ClusterHeapOffset;
        int ClusterCount;
        int FirstClusterOfRootDirectory;
        int VolumeSerialNumber;
        short int FileSystemRevision;
        short int VolumeFlags;
        unsigned char BytesPerSectorShift;
        unsigned char SectorsPerClusterShift;
        unsigned char NumberOfFats;
        unsigned char DriveSelect;
        unsigned char PercentInUse;
        unsigned char Reserved[7];
        unsigned char BootCode[390];
        short int BootSignature;
        unsigned char ExcessSpace;
    } Main_Boot;

    //adding extfat data structure
    typedef struct
    {
    Main_Boot *M_Boot;
    Main_Boot *B_Boot;
    void *FAT;
    void *Data;
    char fileName[50];
    int fd;
    int SectorSize;
    int FileLength;
    int ByteSize;
    int tabSize;
    char info;
    } fileInfo;