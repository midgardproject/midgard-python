%define major_version 10.05.0

%{!?python_sitearch: %global python_sitearch %(%{__python} -c "from distutils.sysconfig import get_python_lib; print get_python_lib(1)")}

Name:           python-midgard2
Version:        %{major_version}
Release:        1%{?dist}
Summary:        Python extension for Midgard

Group:          Development/Languages
License:        LGPLv2+
URL:            http://www.midgard-project.org/
Source0:        %{url}download/%{name}-%{version}.tar.gz
BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

BuildRequires:  python-devel
BuildRequires:  pygobject2-devel
BuildRequires:  midgard2-core-devel >= %{major_version}
BuildRequires:  python-setuptools

%description
The %{name} package contains a dynamic shared object that will 
add Midgard support to Python. Midgard is a persistent storage framework 
which is used e.g. by the Midgard Content Management System. Python is 
an interpreted, interactive, object-oriented programming language often 
compared to Tcl, Perl, Scheme or Java. If you need Midgard support for 
Python applications, you will need to install this package in addition 
to the python package.


%prep
%setup -q


%build
CFLAGS="$RPM_OPT_FLAGS" %{__python} setup.py build


%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $(dirname $RPM_BUILD_ROOT)
mkdir $RPM_BUILD_ROOT
%{__python} setup.py install -O1 --skip-build --root $RPM_BUILD_ROOT


%clean
rm -rf $RPM_BUILD_ROOT


%files
%defattr(-,root,root,-)
%doc COPYING
%{python_sitearch}/*


%changelog
* Mon Nov 02 2009 Jarkko Ala-Louvesniemi <jval@puv.fi> 9.03.99-2
- Added python-setuptools to build requirements

* Fri Oct 30 2009 Jarkko Ala-Louvesniemi <jval@puv.fi> 9.03.99-1
- Initial package using the Fedora spec file template.
