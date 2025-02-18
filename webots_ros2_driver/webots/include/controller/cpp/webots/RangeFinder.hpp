// Copyright 1996-2024 Cyberbotics Ltd.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef RANGE_FINDER_HPP
#define RANGE_FINDER_HPP

#include <webots/Device.hpp>

namespace webots {
  class RangeFinder : public Device {
  public:
    explicit RangeFinder(const std::string &name) : Device(name) {}  // Use Robot::getRangeFinder() instead
    explicit RangeFinder(WbDeviceTag tag) : Device(tag) {}
    virtual ~RangeFinder() {}
    virtual void enable(int samplingPeriod);
    virtual void disable();
    int getSamplingPeriod() const;
    const float *getRangeImage() const;
    int getWidth() const;
    int getHeight() const;
    double getFov() const;
    double getMinRange() const;
    double getMaxRange() const;
    int saveImage(const std::string &filename, int quality) const;
    static float rangeImageGetDepth(const float *image, int width, int x, int y);
  };
}  // namespace webots
#endif  // RANGE_FINDER_HPP
