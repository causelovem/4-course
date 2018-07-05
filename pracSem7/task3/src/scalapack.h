#ifndef SCALAPACK_H
#define SCALAPACK_H

// typedef complex<float> complex_s;
typedef complex<double> complexed;

extern "C" {
    void Cblacs_pinfo( int* mypnum, int* nprocs);
    void Cblacs_get(int context, int request, int* value);
    int Cblacs_gridinit( int* context, char * order, int np_row, int np_col);
    void Cblacs_gridinfo( int context, int*  np_row, int* np_col, int*  my_row, int*  my_col);
    void Cblacs_gridexit( int context);
    void Cblacs_exit( int error_code);
    void Cblacs_gridmap( int* context, int* map, int ld_usermap, int np_row, int np_col);

    int npreroc_(int *n, int *nb, int *iproc, int *isrcproc, int *nprocs);
    int numroc_(int *n, int *nb, int *iproc, int *isrcproc, int *nprocs);

    void descinit_(int *idescal, int *m, int *n, int *mb, int *nb, int *dummy1 , int *dummy2 , int *icon, int *procRows, int *info);

    void pzheevd_(char *jobz, char *uplo, int *n, complexed *a, int *ia, int *ja, int *desca, double *w, complexed *z, int *iz, int *jz, int *descz, complexed *work, int *lwork, double *rwork, int *lrwork, int *iwork, int *liwork, int *info);
    void pzheev_(char *jobz, char *uplo, int *n, complexed *a, int  *ia, int *ja, int *desca, double *w, complexed *z, int *iz, int *jz, int *descz, complexed *work, int *lwork, double *rwork, int *lrwork, int *info);
    
    void pzgemm_(char *transa, char *transb, int *m, int*n, int *k, complexed *alpha, complexed *a, int *ia, int *ja, int *desca, complexed *b, int *ib, int *jb, int *descb, complexed *beta, complexed *c, int *ic, int *jc, int *descc);

    void pzgeadd_ (char *TRANS, int *M, int *N, complexed *ALPHA, complexed *A, int *IA, int *JA, int *DESCA, complexed *BETA, complexed *C, int *IC, int *JC, int *DESCC);

    // void psgesvd_(char *jobu, char *jobvt, int *m, int *n, float *a, int *ia, int *ja, int *desca, float *s, float *u, int *iu, int *ju, int *descu, float *vt, int *ivt, int *jvt, int *descvt, float *work, int *lwork, int *info);
    // void pdgesvd_(char *jobu, char *jobvt, int *m, int *n, double *a, int *ia, int *ja, int *desca, double *s, double *u, int *iu, int *ju, int *descu, double *vt, int *ivt, int *jvt, int *descvt, double *work, int *lwork, int *info);
    // void pcgesvd_(char *jobu, char *jobvt, int *m, int *n, complex_s *a, int *ia, int *ja, int *desca, float *s, complex_s *u, int *iu, int *ju, int *descu, complex_s *vt, int *ivt, int *jvt, int *descvt, complex_s *work, int *lwork, float *rwork, int *info);
    // void pzgesvd_(char *jobu, char *jobvt, int *m, int *n, complexed *a, int *ia, int *ja, int *desca, double *s, complexed *u, int *iu, int *ju, int *descu, complexed *vt, int *ivt, int *jvt, int *descvt, complexed *work, int *lwork, double *rwork, int *info);
}

#endif
