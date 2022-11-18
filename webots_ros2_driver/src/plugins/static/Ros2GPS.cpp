// Copyright 1996-2021 Cyberbotics Ltd.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <webots_ros2_driver/plugins/static/Ros2GPS.hpp>

namespace webots_ros2_driver
{

  void Ros2GPS::init(webots_ros2_driver::WebotsNode *node, std::unordered_map<std::string, std::string> &parameters)
  {
    Ros2SensorPlugin::init(node, parameters);
    mIsEnabled = false;
    mGPS = mNode->robot()->getGPS(parameters["name"]);

    assert(mGPS != NULL);

    if (mGPS->getCoordinateSystem() == webots::GPS::WGS84)
    {
      mGPSPublisher = mNode->create_publisher<sensor_msgs::msg::NavSatFix>(mTopicName, rclcpp::SensorDataQoS().reliable());
      mGPSMessage.header.frame_id = mFrameName;
      mGPSMessage.position_covariance_type = sensor_msgs::msg::NavSatFix::COVARIANCE_TYPE_UNKNOWN;
      mGPSMessage.status.service = sensor_msgs::msg::NavSatStatus::SERVICE_GPS;
    }
    else
    {
      mPointPublisher = mNode->create_publisher<geometry_msgs::msg::PointStamped>(mTopicName, rclcpp::SensorDataQoS().reliable());
      mPointMessage.header.frame_id = mFrameName;
    }
    mSpeedPublisher = mNode->create_publisher<std_msgs::msg::Float32>(mTopicName + "/speed", rclcpp::SensorDataQoS().reliable());
    mSpeedVectorPublisher = mNode->create_publisher<geometry_msgs::msg::Vector3>(mTopicName + "/speed_vector", rclcpp::SensorDataQoS().reliable());

    if (mAlwaysOn) {
      mGPS->enable(mPublishTimestepSyncedMs);
      mIsEnabled = true;
    }
  }

  void Ros2GPS::step()
  {
    if (!preStep())
      return;

    if (mIsEnabled) {
      if (mPointPublisher != nullptr)
        publishPoint();
      else if (mGPSPublisher != nullptr)
        publishGPS();
      else
        assert(false);

      publishSpeed();
      publishSpeedVector();
    }

    if (mAlwaysOn)
      return;

    // Enable/Disable sensor
    const bool pointSubscriptionsExist = mPointPublisher != nullptr && mPointPublisher->get_subscription_count() > 0;
    const bool gpsSubscriptionsExist = mGPSPublisher != nullptr && mGPSPublisher->get_subscription_count() > 0;
    const bool speedSubscriptionsExist = mSpeedPublisher->get_subscription_count() > 0;
    const bool speedVectorSubscriptionsExist = mSpeedVectorPublisher->get_subscription_count() > 0;
    const bool shouldBeEnabled = pointSubscriptionsExist || gpsSubscriptionsExist || speedSubscriptionsExist
                              || speedVectorSubscriptionsExist;
    if (shouldBeEnabled != mIsEnabled)
    {
      if (shouldBeEnabled)
        mGPS->enable(mPublishTimestepSyncedMs);
      else
        mGPS->disable();
      mIsEnabled = shouldBeEnabled;
    }
  }

  void Ros2GPS::publishPoint()
  {
    mPointMessage.header.stamp = mNode->get_clock()->now();
    const double *values = mGPS->getValues();
    mPointMessage.point.x = values[0];
    mPointMessage.point.y = values[1];
    mPointMessage.point.z = values[2];
    mPointPublisher->publish(mPointMessage);
  }

  void Ros2GPS::publishGPS()
  {
    mGPSMessage.header.stamp = mNode->get_clock()->now();
    const double *values = mGPS->getValues();
    mGPSMessage.latitude = values[0];
    mGPSMessage.longitude = values[1];
    mGPSMessage.altitude = values[2];
    mGPSPublisher->publish(mGPSMessage);
  }

  void Ros2GPS::publishSpeed()
  {
    mSpeedMessage.data = mGPS->getSpeed();
    mSpeedPublisher->publish(mSpeedMessage);
  }

  void Ros2GPS::publishSpeedVector()
  {
    const double *values = mGPS->getSpeedVector();
    mSpeedVectorMessage.x = values[0];
    mSpeedVectorMessage.y = values[1];
    mSpeedVectorMessage.z = values[2];
    mSpeedVectorPublisher->publish(mSpeedVectorMessage);
  }
}
