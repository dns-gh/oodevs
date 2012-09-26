// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PERCEPTION_PERCEPTION_RADAR_H
#define SWORD_PERCEPTION_PERCEPTION_RADAR_H

#include "Perception_ABC.h"
#include "PerceptionRadarData.h"
#include <boost/shared_ptr.hpp>

namespace sword
{
namespace perception
{
    class PerceptionObserver_ABC;
    class RadarClass;

// =============================================================================
// @class  PerceptionRadar
// Created: JVT 2004-10-21
// Modified: JVT 2004-10-28
// =============================================================================
class PerceptionRadar : public Perception_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             PerceptionRadar( const wrapper::View& entity, PerceptionObserver_ABC& observer );
    virtual ~PerceptionRadar();
    //@}

    //! @name Operations
    //@{
    virtual void ExecuteAgents( const wrapper::View& model, const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const T_AgentPtrVector& perceivableAgents );
    bool HasRadar( const wrapper::View& entity, std::size_t radarType ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< PerceptionRadarData::T_ZoneSet >   T_RadarZonesVector;
    typedef std::vector< bool >                             T_RadarOnUnitPositionVector;
    typedef boost::shared_ptr< PerceptionRadarData > T_Data;
    typedef std::set< const RadarType* > T_RadarSet;
    typedef std::map< int, T_RadarSet > T_RadarTypesMap;
    typedef std::map< const RadarType*, T_Data > T_RadarDataMap;
    typedef std::map< int, wrapper::View > T_RadarIdMap;
    //@}

private:
    //! @name Add/Remove Points
    //@{
    void EnableRadar( const wrapper::View& radar, const RadarClass& radarClass );
    void EnableLocalizedRadar( const wrapper::View& radar, const RadarClass& radarClass );
    void PrepareRadarData( const wrapper::View& entity, T_RadarTypesMap& types ) const;
    //@}

private:
    //! @name Member Data
    //@{
    PerceptionObserver_ABC&     observer_;
    T_RadarZonesVector          radarZones_;
    T_RadarOnUnitPositionVector radarOnUnitPosition_;
    T_RadarTypesMap             radarTypes_;
    T_RadarDataMap              radarData_;
    T_RadarIdMap                radarId_;
    //@}
};

}
}

#endif // SWORD_PERCEPTION_PERCEPTION_RADAR_H
