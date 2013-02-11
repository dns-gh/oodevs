// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PERCEPTION_PERCEPTION_RADAR_DATA_H
#define SWORD_PERCEPTION_PERCEPTION_RADAR_DATA_H

#include "Perception_ABC.h"
#include "tools/Set.h"
#include <module_api/Model.h>
#include <boost/noncopyable.hpp>
#include <map>

namespace sword
{
namespace wrapper
{
    class Effect;
    class Node;
}
namespace perception
{
    class PerceptionObserver_ABC;
    class RadarType;

// =============================================================================
// @class  PerceptionRadarData
// Created: NLD 2005-05-02
// =============================================================================
class PerceptionRadarData : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef tools::Set< const SWORD_Model* > T_ZoneSet;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             PerceptionRadarData( const wrapper::View& entity, const RadarType& radarType );
             PerceptionRadarData( const RadarType& radarType );
    virtual ~PerceptionRadarData();
    //@}

    //! @name Operations
    //@{
    void Acquire( const wrapper::View& model, const wrapper::View& perceiver, wrapper::Effect& effect,
        PerceptionObserver_ABC& observer, const T_ZoneSet& zones, bool bAcquireOnPerceiverPosition );
    //@}

private:
    //! @name Tools
    //@{
    void AcquireTargets( const wrapper::View& model, const wrapper::View& perceiver, Perception_ABC::T_AgentPtrVector& targets, wrapper::Node& effect );
    void Update        ( const wrapper::View& model, const wrapper::View& acquisitions, PerceptionObserver_ABC& observer );
    void AddData( const wrapper::View& acquisition );
    //@}

private:
    //! @name Types
    //@{
    struct sAcquisitionData
    {
        unsigned int nFirstTimeStepPerceived_;
        bool bUpdated_;
    };
    typedef std::map< std::size_t, sAcquisitionData > T_AgentAcquisitionMap;
    //@}

private:
    //! @name Member Data
    //@{
    const RadarType* pRadarType_;
    T_AgentAcquisitionMap acquisitionData_;
    //@}
};

}
}

#endif // SWORD_PERCEPTION_PERCEPTION_RADAR_DATA_H
