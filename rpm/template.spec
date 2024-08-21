%bcond_without tests
%bcond_without weak_deps

%global __os_install_post %(echo '%{__os_install_post}' | sed -e 's!/usr/lib[^[:space:]]*/brp-python-bytecompile[[:space:]].*$!!g')
%global __provides_exclude_from ^/opt/ros/rolling/.*$
%global __requires_exclude_from ^/opt/ros/rolling/.*$

Name:           ros-rolling-webots-ros2-tests
Version:        2023.1.3
Release:        1%{?dist}%{?release_suffix}
Summary:        ROS webots_ros2_tests package

License:        Apache License 2.0
URL:            http://wiki.ros.org/webots_ros2
Source0:        %{name}-%{version}.tar.gz

Requires:       ros-rolling-ros-workspace
BuildRequires:  python%{python3_pkgversion}-devel
BuildRequires:  ros-rolling-rclpy
BuildRequires:  ros-rolling-ros-workspace
BuildRequires:  ros-rolling-ros2bag
BuildRequires:  ros-rolling-rosbag2-storage-default-plugins
BuildRequires:  ros-rolling-webots-ros2-driver
Provides:       %{name}-devel = %{version}-%{release}
Provides:       %{name}-doc = %{version}-%{release}
Provides:       %{name}-runtime = %{version}-%{release}

%if 0%{?with_tests}
BuildRequires:  python%{python3_pkgversion}-pytest
BuildRequires:  ros-rolling-ament-copyright
BuildRequires:  ros-rolling-geometry-msgs
BuildRequires:  ros-rolling-launch
BuildRequires:  ros-rolling-launch-testing
BuildRequires:  ros-rolling-launch-testing-ament-cmake
BuildRequires:  ros-rolling-launch-testing-ros
BuildRequires:  ros-rolling-sensor-msgs
BuildRequires:  ros-rolling-std-msgs
BuildRequires:  ros-rolling-std-srvs
BuildRequires:  ros-rolling-tf2-ros
BuildRequires:  ros-rolling-webots-ros2-epuck
BuildRequires:  ros-rolling-webots-ros2-mavic
BuildRequires:  ros-rolling-webots-ros2-tesla
BuildRequires:  ros-rolling-webots-ros2-tiago
BuildRequires:  ros-rolling-webots-ros2-turtlebot
BuildRequires:  ros-rolling-webots-ros2-universal-robot
%endif

%description
System tests for `webots_ros2` packages.

%prep
%autosetup -p1

%build
# In case we're installing to a non-standard location, look for a setup.sh
# in the install tree and source it.  It will set things like
# CMAKE_PREFIX_PATH, PKG_CONFIG_PATH, and PYTHONPATH.
if [ -f "/opt/ros/rolling/setup.sh" ]; then . "/opt/ros/rolling/setup.sh"; fi
%py3_build

%install
# In case we're installing to a non-standard location, look for a setup.sh
# in the install tree and source it.  It will set things like
# CMAKE_PREFIX_PATH, PKG_CONFIG_PATH, and PYTHONPATH.
if [ -f "/opt/ros/rolling/setup.sh" ]; then . "/opt/ros/rolling/setup.sh"; fi
%py3_install -- --prefix "/opt/ros/rolling"

%if 0%{?with_tests}
%check
# Look for a directory with a name indicating that it contains tests
TEST_TARGET=$(ls -d * | grep -m1 "\(test\|tests\)" ||:)
if [ -n "$TEST_TARGET" ] && %__python3 -m pytest --version; then
# In case we're installing to a non-standard location, look for a setup.sh
# in the install tree and source it.  It will set things like
# CMAKE_PREFIX_PATH, PKG_CONFIG_PATH, and PYTHONPATH.
if [ -f "/opt/ros/rolling/setup.sh" ]; then . "/opt/ros/rolling/setup.sh"; fi
%__python3 -m pytest $TEST_TARGET || echo "RPM TESTS FAILED"
else echo "RPM TESTS SKIPPED"; fi
%endif

%files
/opt/ros/rolling

%changelog
* Wed Aug 21 2024 Cyberbotics <support@cyberbotics.com> - 2023.1.3-1
- Autogenerated by Bloom

* Mon Apr 08 2024 Cyberbotics <support@cyberbotics.com> - 2023.1.2-1
- Autogenerated by Bloom

* Wed Mar 06 2024 Cyberbotics <support@cyberbotics.com> - 2023.1.1-3
- Autogenerated by Bloom

