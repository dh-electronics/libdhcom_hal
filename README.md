# DHCOM Hardware Abstraction Library

## Build Steps

* build directory should be out of source tree (when using Eclipse IDE)
* create build directory
```bash
devel@Stretch:/work$ mkdir build
```
* assume the project source code (see above) is locate within directory _dhcom-hal_
* run cmake to create Makefiles
```bash
devel@Stretch:/work/build$ cmake -D CMAKE_TOOLCHAIN_FILE=../dhcom-hal/arm-linux-gnueabihf-toolchain.CMakeCrossSysroot.txt CMakeLists.txt ../dhcom-hal/
```
* build binaries
```bash
devel@Stretch:/work/build$ make
```

* build debian package (tested for Debian 10)
```bash
devel@Stretch:/work/build$ cpack
```

## Latest Build

[![pipeline status](https://dhplgl01/sw0028/dhcom_hal/badges/master/pipeline.svg)](https://dhplgl01/sw0028/dhcom_hal/commits/master)

- [libdhcom-hal_x.x-x_armel.deb][ci-artifacts-build-armel]
- [libdhcom-hal_x.x-x_armhf.deb][ci-artifacts-build-armhf]

[ci-artifacts-build-armel]: https://dhplgl01/sw0028/dhcom_hal/-/jobs/artifacts/master/download?job=build-dhcom-hal-arm32v5
[ci-artifacts-build-armhf]: https://dhplgl01/sw0028/dhcom_hal/-/jobs/artifacts/master/download?job=build-dhcom-hal-arm32v7
