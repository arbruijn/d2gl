#include "cfile.h"

fix cfile_read_fix(CFILE *fp);
short cfile_read_fixang(CFILE *file);
void cfile_read_vector(vms_vector *v,CFILE *file);
void cfile_read_matrix(vms_matrix *m,CFILE *file);
void cfile_read_angvec(vms_angvec *v,CFILE *file);
