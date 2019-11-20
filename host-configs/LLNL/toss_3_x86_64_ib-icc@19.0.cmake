set(CONFIG_NAME "toss_3_x86_64_ib-icc@19.0" CACHE PATH "")



set(CMAKE_C_COMPILER /usr/tce/packages/intel/intel-19.0.4/compilers_and_libraries_2019.4.227/linux/bin/intel64/icc CACHE PATH "")
set(CMAKE_CXX_COMPILER /usr/tce/packages/intel/intel-19.0.4/compilers_and_libraries_2019.4.227/linux/bin/intel64/icpc CACHE PATH "")
set(CMAKE_Fortran_COMPILER /usr/tce/packages/intel/intel-19.0.4/compilers_and_libraries_2019.4.227/linux/bin/intel64/ifort CACHE PATH "")

set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG -march=native -mtune=native" CACHE STRING "")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -march=native -mtune=native" CACHE STRING "")
set(CMAKE_Fortran_FLAGS_RELEASE "-O3 -DNDEBUG -march=native -mtune=native" CACHE STRING "")

set(ENABLE_FORTRAN OFF CACHE BOOL "")
set(ENABLE_MPI ON CACHE BOOL "")

set(MPI_HOME             /usr/tce/packages/mvapich2/mvapich2-2.3-intel-19.0.4 CACHE PATH "")
set(MPI_C_COMPILER       ${MPI_HOME}/bin/mpicc   CACHE PATH "")
set(MPI_CXX_COMPILER     ${MPI_HOME}/bin/mpicxx  CACHE PATH "")
set(MPI_Fortran_COMPILER ${MPI_HOME}/bin/mpifort CACHE PATH "")

set(MPIEXEC              /usr/bin/srun CACHE PATH "")
set(MPIEXEC_NUMPROC_FLAG "-n" CACHE STRING "")

set(GEOSX_TPL_ROOT_DIR /usr/workspace/settgast/Codes/geosx/thirdPartyLibs CACHE PATH "")
set(GEOSX_TPL_DIR ${GEOSX_TPL_ROOT_DIR}/install-${CONFIG_NAME}-release CACHE PATH "")

set(SPHINX_EXECUTABLE /collab/usr/gapps/python/build/spack-toss3.2/opt/spack/linux-rhel7-x86_64/gcc-4.9.3/python-2.7.14-7rci3jkmuht2uiwp433afigveuf4ocnu/bin/sphinx-build CACHE PATH "")
set(UNCRUSTIFY_EXECUTABLE ${GEOSX_TPL_DIR}/uncrustify/bin/uncrustify CACHE PATH "")
message("UNCRUSTIFY_EXECUTABLE = ${UNCRUSTIFY_EXECUTABLE}")
set(DOXYGEN_EXECUTABLE /usr/gapps/GEOS/geosx/thirdPartyLibs/doxygen/bin/doxygen CACHE PATH "")

set(ENABLE_GTEST_DEATH_TESTS ON CACHE BOOL "")

set(ENABLE_PAMELA ON CACHE BOOL "")
set(ENABLE_PVTPackage ON CACHE BOOL "")
set(ENABLE_GEOSX_PTP ON CACHE BOOL "" FORCE)


set(ENABLE_CALIPER ON CACHE BOOL "")
set(ENABLE_PAPI ON CACHE BOOL "")
set(PAPI_PREFIX /usr/tce/packages/papi/papi-5.4.3 CACHE PATH "")

set(ENABLE_OPENMP ON CACHE BOOL "")
set(CUDA_ENABLED OFF CACHE BOOL "")

set(ENABLE_MKL ON CACHE BOOL "")
set(MKL_ROOT /usr/tce/packages/intel/intel-19.0.4/compilers_and_libraries_2019.4.227/linux/mkl)
set(MKL_INCLUDE_DIRS /usr/tce/packages/mkl/mkl-2019.0/include CACHE STRING "")
set(MKL_LIBRARIES ${MKL_ROOT}/lib/intel64/libmkl_intel_lp64.so
                  ${MKL_ROOT}/lib/intel64/libmkl_intel_thread.so
                  ${MKL_ROOT}/lib/intel64/libmkl_core.so
                  CACHE STRING "")


set( ENABLE_TOTALVIEW_OUTPUT ON CACHE BOOL "Enables Totalview custom view" FORCE)

# PETSc doesn't seem to work correctly with clang.
set(ENABLE_PETSC OFF CACHE BOOL "")
