#include "cfile.h"
#include "vecmat.h"
#include "byteswap.h"

fix cfile_read_fix(CFILE *fp)
{
	fix f;
	
	cfread(&f, sizeof(fix), 1, fp);
	return f;
}

short cfile_read_fixang(CFILE *file)
{
	fixang f;

	if (cfread( &f, sizeof(f), 1, file) != 1)
		Error( "Error reading fixang in gamesave.c" );

	f = (fixang)INTEL_SHORT((short)f);
	return f;
}

void cfile_read_vector(vms_vector *v,CFILE *file)
{
	v->x = cfile_read_fix(file);
	v->y = cfile_read_fix(file);
	v->z = cfile_read_fix(file);
}

void cfile_read_matrix(vms_matrix *m,CFILE *file)
{
	cfile_read_vector(&m->rvec,file);
	cfile_read_vector(&m->uvec,file);
	cfile_read_vector(&m->fvec,file);
}

void cfile_read_angvec(vms_angvec *v,CFILE *file)
{
	v->p = cfile_read_fixang(file);
	v->b = cfile_read_fixang(file);
	v->h = cfile_read_fixang(file);
}
