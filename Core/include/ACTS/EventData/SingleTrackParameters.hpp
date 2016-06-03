// This file is part of the ACTS project.
//
// Copyright (C) 2016 ACTS project team
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef ACTS_SINGLETRACKPARAMETERS_H
#define ACTS_SINGLETRACKPARAMETERS_H 1

// STL include(s)
#include <type_traits>

// ACTS includes
#include "ACTS/Utilities/Definitions.hpp"
#include "ACTS/EventData/TrackParametersBase.hpp"
#include "ACTS/EventData/detail/coordinate_transformations.hpp"

namespace Acts
{
  /**
   * @class SingleTrackParameters
   *
   * @brief base class for a single set of track parameters
   *
   * This class implements the interface for charged/neutral track parameters for the case that it
   * represents a single set of track parameters (opposed to a list of different sets of track
   * parameters as used by e.g. GSF or multi-track fitters).
   *
   * The track parameters and their uncertainty are defined in local reference frame which depends on
   * the associated surface of the track parameters.
   *
   * @tparam ChargePolicy type for distinguishing charged and neutral tracks/particles
   *         (must be either ChargedPolicy or NeutralPolicy)
   */
  template<class ChargePolicy>
  class SingleTrackParameters : public TrackParametersBase
  {
    static_assert(std::is_same<ChargePolicy,ChargedPolicy>::value or std::is_same<ChargePolicy,NeutralPolicy>::value,
                  "ChargePolicy must either be 'Acts::ChargedPolicy' or 'Acts::NeutralPolicy");
  public:
    // public typedef's
    typedef typename TrackParametersBase::ParVector_t  ParVector_t;  ///< vector type for stored track parameters
    typedef typename TrackParametersBase::CovMatrix_t  CovMatrix_t;  ///< type of covariance matrix
    typedef std::unique_ptr<CovMatrix_t>               CovPtr_t;     ///< type for unique pointer to covariance matrix

    /**
     * @brief default virtual destructor
     */
    virtual ~SingleTrackParameters() = default;

    /**
     * @brief virtual constructor
     */
    virtual SingleTrackParameters<ChargePolicy>* clone() const override = 0;

    /**
     * @copydoc TrackParametersBase::position
     */
    virtual ActsVectorD<3> position() const final
    {
      return m_vPosition;
    }

    /**
     * @copydoc TrackParametersBase::momentum
     */
    virtual ActsVectorD<3> momentum() const final
    {
      return m_vMomentum;
    }

    /**
     * @brief equality operator
     *
     * @return @c true of both objects have the same charge policy, parameter values,
     *         position and momentum, otherwise @c false
     */
    virtual bool operator==(const TrackParametersBase& rhs) const override
    {
      auto casted = dynamic_cast<decltype(this)>(&rhs);
      if(!casted)
        return false;

      return (m_oChargePolicy == casted->m_oChargePolicy && m_oParameters == casted->m_oParameters && m_vPosition == casted->m_vPosition && m_vMomentum == casted->m_vMomentum);
    }

    /**
     * @copydoc TrackParametersBase::charge
     */
    virtual double charge() const final
    {
      return m_oChargePolicy.getCharge();
    }

    /**
     * @copydoc TrackParametersBase::getParameterSet
     */
    virtual const FullParameterSet& getParameterSet() const final
    {
      return m_oParameters;
    }

  protected:
    /**
     * @brief standard constructor for track parameters of charged particles
     *
     * @param cov unique pointer to covariance matrix (nullptr is accepted)
     * @param parValues vector with parameter values
     * @param position 3D vector with global position
     * @param momentum 3D vector with global momentum
     */
    template<typename T = ChargePolicy,std::enable_if_t<std::is_same<T,ChargedPolicy>::value,int> = 0>
    SingleTrackParameters(CovPtr_t cov,
                          const ParVector_t& parValues,
                          const ActsVectorD<3>& position,
                          const ActsVectorD<3>& momentum):
      TrackParametersBase(),
      m_oChargePolicy(detail::coordinate_transformation::parameters2charge(parValues)),
      m_oParameters(std::move(cov),parValues),
      m_vPosition(position),
      m_vMomentum(momentum)
    {}

    /**
     * @brief standard constructor for track parameters of neutral particles
     *
     * @param cov unique pointer to covariance matrix (nullptr is accepted)
     * @param parValues vector with parameter values
     * @param position 3D vector with global position
     * @param momentum 3D vector with global momentum
     */
    template<typename T = ChargePolicy,std::enable_if_t<std::is_same<T,NeutralPolicy>::value,int> = 0>
    SingleTrackParameters(CovPtr_t cov,
                          const ParVector_t& parValues,
                          const ActsVectorD<3>& position,
                          const ActsVectorD<3>& momentum):
      TrackParametersBase(),
      m_oChargePolicy(),
      m_oParameters(std::move(cov),parValues),
      m_vPosition(position),
      m_vMomentum(momentum)
    {}

    /**
     * @brief default copy constructor
     */
    SingleTrackParameters(const SingleTrackParameters<ChargePolicy>& copy) = default;

    /**
     * @brief default move constructor
     */
    SingleTrackParameters(SingleTrackParameters<ChargePolicy>&& copy) = default;

    /**
     * @brief copy assignment operator
     *
     * @param rhs object to be copied
     */
    SingleTrackParameters<ChargePolicy>& operator=(const SingleTrackParameters<ChargePolicy>& rhs)
    {
      // check for self-assignment
      if(this != &rhs)
      {
        m_oChargePolicy = rhs.m_oChargePolicy;
        m_oParameters   = rhs.m_oParameters;
        m_vPosition     = rhs.m_vPosition;
        m_vMomentum     = rhs.m_vMomentum;
      }

      return *this;
    }

    /**
     * @brief move assignment operator
     *
     * @param rhs object to be movied into `*this`
     */
    SingleTrackParameters<ChargePolicy>& operator=(SingleTrackParameters<ChargePolicy>&& rhs)
    {
      // check for self-assignment
      if(this != &rhs)
      {
        m_oChargePolicy = std::move(rhs.m_oChargePolicy);
        m_oParameters   = std::move(rhs.m_oParameters);
        m_vPosition     = std::move(rhs.m_vPosition);
        m_vMomentum     = std::move(rhs.m_vMomentum);
      }

      return *this;
    }

    /**
     * @brief update global momentum from current parameter values
     *
     * @note This function is triggered when called with an argument of a type
     *       different from Acts::local_parameter
     */
    template<typename T>
    void updateGlobalCoordinates(const T&)
    {
      m_vMomentum = detail::coordinate_transformation::parameters2globalMomentum(getParameterSet().getParameters());
    }

    /**
     * @brief update global position from current parameter values
     *
     * @note This function is triggered when called with an argument of a type Acts::local_parameter
     */
    void updateGlobalCoordinates(const local_parameter&)
    {
      m_vPosition = detail::coordinate_transformation::parameters2globalPosition(getParameterSet().getParameters(),this->associatedSurface());
    }

    ChargePolicy      m_oChargePolicy;  ///< charge policy object distinguishing between charged and neutral tracks
    FullParameterSet  m_oParameters;    ///< ParameterSet object holding the parameter values and covariance matrix
    ActsVectorD<3>    m_vPosition;      ///< 3D vector with global position
    ActsVectorD<3>    m_vMomentum;      ///< 3D vector with global momentum
  };
}  // end of namespace Acts

#endif // ACTS_SINGLETRACKPARAMETERS_h