git clone https://github.com/tianocore/edk2.git
cd edk2
git submodule update --init --recursive
make -C BaseTools
source edksetup.sh

build \
  -a X64 \
  -t GCC5 \
  -p OvmfPkg/OvmfPkgX64.dsc \
  -b DEBUG \
  -D DEBUG_ON_SERIAL_PORT \
  -D SOURCE_DEBUG_ENABLE




edk2 ❱❱❱ ls Build/OvmfX64/DEBUG_GCC5/FV
DXEFV.ext     DXEFV.Fv.txt  FVMAIN_COMPACT.ext     FVMAIN_COMPACT.Fv.txt   Guid.xref     OVMF.fd       PEIFV.Fv      PEIFV.inf  SECFV.Fv.map
DXEFV.Fv      DXEFV.inf     FVMAIN_COMPACT.Fv      FVMAIN_COMPACT.inf      MEMFD.fd      OVMF_VARS.fd  PEIFV.Fv.map  SECFV.ext  SECFV.Fv.txt
DXEFV.Fv.map  Ffs           FVMAIN_COMPACT.Fv.map  GuidedSectionTools.txt  OVMF_CODE.fd  PEIFV.ext     PEIFV.Fv.txt  SECFV.Fv   SECFV.inf

