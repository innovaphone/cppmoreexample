
NAMESPACE_BEGIN

extern "C" int isnprintf (char * buffer, int length, const char * format, ...);
extern "C" int ivsnprintf (char * buffer, int length, const char * format, va_list ap);
extern "C" int ivsnprintf_ (char * out, int len, const char * fmt0, va_list ap);

extern "C" int isprintf(char * buffer, const char * format, ...);
extern "C" int ivsprintf(char * buffer, const char * format, va_list ap);
extern "C" bool strmatchi(const char * ref, const char * s, char ** ptr);
extern "C" int _strcasecmp(const char * c1, const char * c2);
extern "C" void amemcpy(dword * dst, const dword * src, int len);
extern "C" void amemclr(dword * dst, int len);
extern "C" int packet2args(class packet * data, char * buf, int buf_size, int & argc, char * argv[], bool comment, bool escape=false);
extern "C" class packet * args2packet(int argc, char * argv[]);
extern "C" void * read_checked(void ** p);
extern "C" time_t tm2time_t(struct tm * tm);

extern "C" dword fix_log2(dword n);
extern "C" dword fix_log10(dword n);

NAMESPACE_END
