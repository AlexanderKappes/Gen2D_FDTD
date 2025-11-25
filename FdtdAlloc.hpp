#ifndef FDTD_ALLOC_HPP
#define FDTD_ALLOC_HPP

/* memory allocation macros */
#define ALLOC_1D(PNTR, NUM, TYPE) \
    PNTR = (TYPE *)calloc(NUM, sizeof(TYPE)); \
    if (!PNTR) { \
        perror("ALLOC_1D"); \
        fprintf(stderr, \
        "Allocation failed for " #PNTR ". Terminating...\n");\
    exit(-1); \
}

#define ALLOC_2D(PNTR, NUMX, NUMY, TYPE) \
    PNTR = (TYPE *)calloc((NUMX) * (NUMY), sizeof(TYPE)); \
    if (!PNTR) { \
        perror("ALLOC_2D"); \
        fprintf(stderr, \
        "Allocation failed for " #PNTR ". Terminating...\n");\
        exit(-1); \
}

#endif // FDTD_ALLOC_HPP
