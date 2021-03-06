#define _MAX_DRIVE   3  /* maximum length of drive component */
#define _MAX_DIR    256 /* maximum length of path component */
#define _MAX_FNAME  256 /* maximum length of file name component */
#define _MAX_EXT    256 /* maximum length of extension component */
#define _MAX_PATH   256
typedef struct find_t { char name[256]; } find_t;
#define _A_VOLID 8

void _makepath(char *path, const char *drive, const char *dir, const char *fname, const char *ext);
void _splitpath( const char *path, char *node, char *dir, char *fname, char *ext );
void delay(int);
