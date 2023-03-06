/*---------------------------------------------------------------------------*/
/* zip.h                                                                     */
/* copyright (c) innovaphone 2021                                            */
/*                                                                           */
/*---------------------------------------------------------------------------*/

NAMESPACE_BEGIN

class Zip {
protected:
    byte * buf;
    byte * tmp;
    unsigned pos;
    unsigned tmpPos;
    unsigned count;

public:
    Zip();
    void Init(byte * out, byte * temp);
    void AddFile(const char * fileName, const byte * data, unsigned len);
    unsigned Final();

    static unsigned WriteLocalFileHeader(byte * out, const char * fileName, unsigned fileSize, dword crc);
    static unsigned WriteCentralDirectoryHeader(byte * out, const char * fileName, unsigned fileSize, dword crc, unsigned localHeaderOffset);
    static unsigned WriteEndOfCentralDirectory(byte * out, unsigned fileCount, unsigned centralDirectoryHeaderSize, unsigned centralDirectoryHeaderOffset);
};

NAMESPACE_END
