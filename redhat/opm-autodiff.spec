#
# spec file for package opm-autodiff
#

%define tag rc4

Name:           opm-autodiff
Version:        2013.10
Release:        0
Summary:        Open Porous Media - core library
License:        GPL-3.0
Group:          Development/Libraries/C and C++
Url:            http://www.opm-project.org/
Source0:        https://github.com/OPM/%{name}/archive/release/%{version}/%{tag}.tar.gz#/%{name}-%{version}.tar.gz
BuildRequires:  blas-devel lapack-devel dune-common-devel
BuildRequires:  git suitesparse-devel doxygen bc boost148-devel
BuildRequires:  opm-parser-devel opm-core-devel dune-cornerpoint-devel
BuildRequires:  tinyxml-devel dune-istl-devel eigen3-devel ert.ecl-devel
%{?el6:BuildRequires: cmake28 devtoolset-2}
%{!?el6:BuildRequires: cmake gcc gcc-c++}
BuildRoot:      %{_tmppath}/%{name}-%{version}-build
Requires:       libopm-autodiff1 = %{version}

%description
The Open Porous Media (OPM) initiative provides a set of open-source tools centered around the simulation of flow and transport of fluids in porous media. The goal of the initiative is to establish a sustainable environment for the development of an efficient and well-maintained software suite.

%package -n libopm-autodiff1
Summary:        Open Porous Media - automatic differentiation library
Group:          System/Libraries

%description -n libopm-autodiff1
The Open Porous Media (OPM) initiative provides a set of open-source tools centered around the simulation of flow and transport of fluids in porous media. The goal of the initiative is to establish a sustainable environment for the development of an efficient and well-maintained software suite.

%package devel
Summary:        Development and header files for opm-autodiff
Group:          Development/Libraries/C and C++
Requires:       libopm-autodiff1 = %{version}

%description devel
This package contains the development and header files for opm-autodiff

%package doc
Summary:        Documentation files for opm-autodiff
Group:          Documentation
BuildArch:	noarch

%description doc
This package contains the documentation files for opm-autodiff

%package bin
Summary:        Applications in opm-autodiff
Group:          Scientific
Requires:       libopm-autodiff1 = %{version}

%description bin
This package contains the applications for opm-autodiff

%prep
%setup -q -n %{name}-release-%{version}-%{tag}

%build
%{?el6:scl enable devtoolset-2 bash}
%{?el6:cmake28} %{?!el6:cmake} -DBUILD_SHARED_LIBS=1 -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=%{_prefix} -DCMAKE_INSTALL_DOCDIR=share/doc/%{name}-%{version} -DUSE_RUNPATH=OFF %{?el6:-DCMAKE_CXX_COMPILER=/opt/rh/devtoolset-2/root/usr/bin/g++ -DCMAKE_C_COMPILER=/opt/rh/devtoolset-2/root/usr/bin/gcc} -DBOOST_LIBRARYDIR=%{_libdir}/boost148 -DBOOST_INCLUDEDIR=/usr/include/boost148
make

%install
make install DESTDIR=${RPM_BUILD_ROOT}
make install-html DESTDIR=${RPM_BUILD_ROOT}

%clean
rm -rf %{buildroot}

%post -n libopm-autodiff1 -p /sbin/ldconfig

%postun -n libopm-autodiff1 -p /sbin/ldconfig

%files doc
%{_docdir}/*

%files -n libopm-autodiff1
%defattr(-,root,root,-)
%{_libdir}/*.so.*

%files devel
%defattr(-,root,root,-)
%{_libdir}/*.so
%{_libdir}/dunecontrol/*
%{_libdir}/pkgconfig/*
%{_includedir}/*
%{_datadir}/cmake/*

%files bin
%{_bindir}/*
