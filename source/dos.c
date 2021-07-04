#include <stdio.h>
#include <string.h>
#include "dos.h"

void _makepath(char *path, const char *drive, const char *dir, const char *fname, const char *ext) {
	*path = 0;
	if (drive)
		sprintf(path, "%c:", *drive);
	if (dir)
		strcat(path, dir);
	if (fname) {
		if (*fname && *fname != '\\' && dir && *dir && dir[strlen(dir) - 1] != '\\')
			strcat(path, "\\");
		strcat(path, fname);
	}
	if (ext) {
		if (*ext != '.')
			strcat(path, ".");
		strcat(path, ext);
	}
}
#if 0
void _splitpath(const char *path, char *drive, char *dir, char *fname, char *ext) {
	const char *p = strchr(path, '.');
	if (p == NULL)
		p = path + strlen(path);
	if (drive)
		*drive = 0;
	if (dir)
		*dir = 0;
	if (fname) {
		memcpy(fname, path, p - path);
		fname[p - path] = 0;
	}
	if (ext)
		strcpy(ext, p);
}
#endif