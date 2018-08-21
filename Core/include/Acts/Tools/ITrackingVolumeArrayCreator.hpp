// This file is part of the Acts project.
//
// Copyright (C) 2016-2018 Acts project team
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

///////////////////////////////////////////////////////////////////
// ITrackingVolumeArrayCreator.h, Acts project
///////////////////////////////////////////////////////////////////

#pragma once
#include <memory>
#include <vector>
#include "Acts/Utilities/BinnedArray.hpp"
#include "Acts/Utilities/BinningType.hpp"

namespace Acts {

class TrackingVolume;

/// A std::shared_ptr to a tracking volume
typedef std::shared_ptr<const TrackingVolume> TrackingVolumePtr;
typedef std::shared_ptr<TrackingVolume>       MutableTrackingVolumePtr;

/// A BinnedArray of a std::shared_tr to a TrackingVolume
typedef BinnedArray<TrackingVolumePtr> TrackingVolumeArray;
/// A std::vector of a std::shared_ptr to a TrackingVolume
typedef std::vector<TrackingVolumePtr> TrackingVolumeVector;

/// @class ITrackingVolumeArrayCreator
///
/// Interface class ITrackingVolumeArrayCreators It inherits from IAlgTool.
///
/// It is designed to centralize the code to create
/// Arrays of Tracking Volumes for both:
///
///   - confinement in another TrackingVolume
///   - navigation and glueing
///
/// Arrays for glueing and confinement are often the same,
/// therefore the newly created TrackingVolumeArray is done by a shared_ptr
class ITrackingVolumeArrayCreator
{
public:
  /// Virtual destructor
  virtual ~ITrackingVolumeArrayCreator() = default;

  /// TrackingVolumeArrayCreator interface method - creates array depending on
  /// the binning type
  ///
  /// @param vols are the TrackingVolumes ordered in a tracker
  /// @param bVal is the binning value for the volume binning
  ///
  /// @return sahred pointer to a new TrackingVolumeArray
  virtual std::shared_ptr<const TrackingVolumeArray>
  trackingVolumeArray(const TrackingVolumeVector& vols,
                      BinningValue                bVal) const = 0;
};
}  // namespace