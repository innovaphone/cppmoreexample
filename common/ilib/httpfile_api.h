
/*-----------------------------------------------------------------------------------------------*/
/* httpfile_api.h                                                                                */
/* copyright (c) innovaphone 2015                                                                */
/*                                                                                               */
/* common api for the platform specific httpfile implementations                                 */
/*-----------------------------------------------------------------------------------------------*/

class httpfile_api {
public:
    static class httpfile_api * find(const char * name);
    static class httpfile_api * find_next_left(const char * name);

    virtual const byte * GetData() = 0;
    virtual size_t GetSize() = 0;
};
