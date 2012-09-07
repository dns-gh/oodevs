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
//#include "simulation_terrain/TER_Agent_ABC.h"
#include <set>
#include <map>
#include <boost/noncopyable.hpp>

class TER_Localisation;

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
    typedef std::set< const TER_Localisation* > T_ZoneSet;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             PerceptionRadarData( const wrapper::View& entity, wrapper::Effect& effect, const RadarType& radarType );
             PerceptionRadarData( wrapper::Effect& effect, const RadarType& radarType );
    virtual ~PerceptionRadarData();
    //@}

    //! @name Operations
    //@{
    void Acquire( const wrapper::View& model, const wrapper::View& perceiver, PerceptionObserver_ABC& observer, const T_ZoneSet& zones, bool bAcquireOnPerceiverPosition );
    //@}

private:
    //! @name Tools
    //@{
    void AcquireTargets( const wrapper::View& model, const wrapper::View& perceiver, Perception_ABC::T_AgentPtrVector& targets, wrapper::Node& effect );
    void Update        ( const wrapper::View& model, PerceptionObserver_ABC& observer, wrapper::Node& effect );
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
    typedef std::map< std::string, sAcquisitionData > T_AgentAcquisitionMap;
    //@}

private:
    //! @name Member Data
    //@{
    const RadarType* pRadarType_;
    wrapper::Effect& effect_;
    T_AgentAcquisitionMap acquisitionData_;
    //@}
};

}
}

#endif // SWORD_PERCEPTION_PERCEPTION_RADAR_DATA_H
