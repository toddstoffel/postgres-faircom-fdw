%{!?pgmajorversion:%global pgmajorversion 14}
%global pginstdir /usr/pgsql-%{pgmajorversion}
%global sname faircom_fdw
%global faircom_arch arm64
%ifarch x86_64
%global faircom_arch amd64
%endif
%ifarch aarch64
%global faircom_arch arm64
%endif
%global faircom_src %{_builddir}/%{sname}-%{version}/core/faircom_libs/%{faircom_arch}
%global __brp_mangle_shebangs /bin/true

Name:           postgresql-%{pgmajorversion}-faircom-fdw
Version:        @VERSION@
Release:        1%{?dist}
Summary:        FairCom Foreign Data Wrapper for PostgreSQL

License:        Custom
URL:            https://github.com/faircom/faircom-fdw
Source0:        %{sname}-%{version}.tar.gz

BuildRequires:  postgresql%{pgmajorversion}-devel
BuildRequires:  gcc
BuildRequires:  make
Requires:       postgresql%{pgmajorversion}

%description
FairCom Foreign Data Wrapper (FDW) for PostgreSQL enables PostgreSQL
databases to query FairCom database servers as if they were native
PostgreSQL tables.

The FDW provides connection pooling, predicate pushdown optimization,
and automatic type mapping between PostgreSQL and FairCom data types.

%prep
%setup -q -n %{sname}-%{version}
find core/src core/faircom_libs/%{faircom_arch}/include core -type f -exec chmod 0644 {} +

%build
export FAIRCOM_HOME=%{faircom_src}
export FAIRCOM_INCLUDE=$FAIRCOM_HOME/include
export PG_CONFIG=%{pginstdir}/bin/pg_config

cd core
make USE_PGXS=1 PG_CONFIG=$PG_CONFIG WARN_CFLAGS="-w" \
	SHLIB_LINK="-L%{faircom_src}/lib -lmtclient -Wl,-rpath,/usr/lib/faircom/lib"

%install
export FAIRCOM_HOME=%{faircom_src}
export FAIRCOM_INCLUDE=$FAIRCOM_HOME/include
export FAIRCOM_LIB=$FAIRCOM_HOME/lib
export PG_CONFIG=%{pginstdir}/bin/pg_config

cd core
make USE_PGXS=1 PG_CONFIG=$PG_CONFIG DESTDIR=%{buildroot} install

# Install FairCom libraries
mkdir -p %{buildroot}/usr/lib/faircom/lib
mkdir -p %{buildroot}/usr/lib/faircom/include
cp -r faircom_libs/%{faircom_arch}/lib/* %{buildroot}/usr/lib/faircom/lib/
cp -r faircom_libs/%{faircom_arch}/include/* %{buildroot}/usr/lib/faircom/include/
find %{buildroot}/usr/lib/faircom/include -type f -exec chmod 0644 {} +
find %{buildroot}/usr/lib/faircom/include -type d -exec chmod 0755 {} +
chmod 0644 %{buildroot}%{pginstdir}/share/extension/%{sname}.control
chmod 0644 %{buildroot}%{pginstdir}/share/extension/%{sname}--*.sql

%files
%doc README.md
%{pginstdir}/lib/%{sname}.so
%{pginstdir}/lib/bitcode/faircom_fdw*
%{pginstdir}/share/extension/%{sname}.control
%{pginstdir}/share/extension/%{sname}--*.sql
/usr/lib/faircom/lib/*
/usr/lib/faircom/include/*

%changelog
* Mon Jan 06 2025 FairCom Corporation <support@faircom.com> - 1.0.0-1
- Initial RPM release
- Support for PostgreSQL 11, 12, 13, 14, 15, 16
- Connection pooling and caching
- Predicate pushdown optimization
- Type mapping for common PostgreSQL types
- Schema introspection
