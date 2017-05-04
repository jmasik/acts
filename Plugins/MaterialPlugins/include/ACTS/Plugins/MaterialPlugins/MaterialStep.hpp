// This file is part of the ACTS project.
//
// Copyright (C) 2016 ACTS project team
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

///////////////////////////////////////////////////////////////////
// MaterialStep.hpp, ACTS project
///////////////////////////////////////////////////////////////////

#ifndef ACTS_MATERIAL_MATERIALSTEP_H
#define ACTS_MATERIAL_MATERIALSTEP_H

#include <memory>
#include "ACTS/Material/MaterialProperties.hpp"

#if !defined(__CLING__)
#include "ACTS/Utilities/Definitions.hpp"
#endif

namespace Acts {

/// @class MaterialStep
///
/// @brief Class holding the material properties at a certain point
///
/// The MaterialStep class is needed to store the material properties (material
/// and step length) at a given global position for the material mapping
/// process.
///
/// @todo Currently a specific Position struct is used instead of the
/// Acts::Vector3D
/// to simplify the creation of a ROOT dictionary for this class, since
/// Acts::Vector3D is an Eigen class. In future the Vector3D should be used
/// to guarantee consitency and avoid conversions.

class MaterialStep
{
public:
  /// @struct Position
  /// the global three dimensional position of the material step
  /// @todo replace by Acts::Vector3D
  struct Position
  {
    /// X Coordinate of the material step
    double x;
    /// Y Coordinate of the material step
    double y;
    /// Z Coordinate of the material step
    double z;

    /// Default constructor creating a position at the origin
    Position() : x(0.), y(0.), z(0.) {}

    /// Constructor to set the three coordinates
    Position(double x, double y, double z) : x(x), y(y), z(z) {}

    /// Copy Constructor
    Position(const Position& pos) : x(pos.x), y(pos.y), z(pos.z) {}

#if !defined(__CLING__)
    /// Constructor from Vector3D
    Position(const Vector3D& pos) : x(pos.x()), y(pos.y()), z(pos.z()) {}

    /// assignment operator from Vector3D
    Position&
    operator=(const Vector3D& pos)
    {
      x = pos.x();
      y = pos.y();
      z = pos.z();
      return (*this);
    }
#endif
  };

  /// Default constructor
  /// setting the position to the origin and making default material properties
  MaterialStep();

  /// Constructor to set th material properties at a certain position
  /// @param mat the material properties (material + step length) at the given
  /// position
  /// @param pos three dimensional global position of the step
  MaterialStep(const MaterialProperties& mat, const Position& pos);

  /// Copy Constructor
  MaterialStep(const MaterialStep& mstep);

  /// Implicit contructor
  /// - uses the copy constructor
  MaterialStep*
  clone() const;

  /// Default Destructor
  ~MaterialStep() = default;

  /// Assignment operator
  MaterialStep&
  operator=(const MaterialStep& mstep);

  /// return method for the position of the step
  const Position
  position() const;

  /// return method for the material properties
  const MaterialProperties&
  materialProperties() const;

private:
  /// the global three dimensional position of the material step
  Position m_position;

  /// the accumulated material of the step containing the material and the step
  /// length
  MaterialProperties m_material;
};

}  /// end of namespace

inline const Acts::MaterialStep::Position
Acts::MaterialStep::position() const
{
  return m_position;
}

/// return method for the material properties
inline const Acts::MaterialProperties&
Acts::MaterialStep::materialProperties() const
{
  return m_material;
}

#endif  // ACTS_MATERIAL_MATERIALSTEP_H
