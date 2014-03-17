// *****************************************************************************
//
// $Created: JVT 2004-10-21 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Perceptions/PHY_PerceptionRadar.cpp $
// $Author: Nld $
// $Modtime: 3/05/05 12:14 $
// $Revision: 7 $
// $Workfile: PHY_PerceptionRadar.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_PerceptionRadar.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h"
#include "Entities/Agents/Units/Radars/PHY_RadarClass.h"
#include "MT_Tools/MT_Logger.h"

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadar constructor
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
PHY_PerceptionRadar::PHY_PerceptionRadar( PHY_RoleInterface_Perceiver& perceiver )
    : PHY_Perception_ABC  ( perceiver )
    , radarOnUnitPosition_( PHY_RadarClass::GetNbrClasses() )
    , radarZones_         ( PHY_RadarClass::GetNbrClasses() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadar destructor
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
PHY_PerceptionRadar::~PHY_PerceptionRadar()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadar::EnableRadar
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
void PHY_PerceptionRadar::EnableRadar( const PHY_RadarClass& radarClass )
{
    assert( radarOnUnitPosition_.size() > radarClass.GetID() );
    radarOnUnitPosition_[ radarClass.GetID() ] = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadar::DisableRadar
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
void PHY_PerceptionRadar::DisableRadar( const PHY_RadarClass& radarClass )
{
    assert( radarOnUnitPosition_.size() > radarClass.GetID() );
    radarOnUnitPosition_[ radarClass.GetID() ] = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadar::EnableRadar
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
int PHY_PerceptionRadar::EnableRadar( const PHY_RadarClass& radarClass, const TER_Localisation& zone )
{
    assert( radarZones_.size() > radarClass.GetID() );
    const TER_Localisation* pZone = new TER_Localisation( zone );
    if( ! radarZones_[ radarClass.GetID() ].insert( pZone ).second )
        MT_LOG_ERROR_MSG( __FUNCTION__ << " : Insert failed" );
    int id = PHY_Perception_ABC::GetPerceptionId();
    radarId_[ id ] = pZone;
    return id;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadar::DisableRadar
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
void PHY_PerceptionRadar::DisableRadar( const PHY_RadarClass& radarClass, int id )
{
    assert( radarZones_.size() > radarClass.GetID() );
    const TER_Localisation* pLocalisation = radarId_[ id ];
    if( radarZones_[ radarClass.GetID() ].erase( pLocalisation ) == 1 )
        delete pLocalisation;
    radarId_.erase( id );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadar::HasRadarToHandle
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
bool PHY_PerceptionRadar::HasRadarToHandle() const
{
    for( auto it = radarZones_.begin(); it != radarZones_.end(); ++it )
        if( !it->empty() )
            return true;
    for( auto it = radarOnUnitPosition_.begin(); it != radarOnUnitPosition_.end(); ++it )
        if( *it )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadar::IsUsingActiveRadar
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
bool PHY_PerceptionRadar::IsUsingActiveRadar() const
{
    auto radarClasses = PHY_RadarClass::GetRadarClasses();
    for( auto it = radarClasses.begin(); it != radarClasses.end(); ++it )
        if( IsUsingActiveRadar( *it->second ) )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadar::IsUsingActiveRadar
// Created: JSR 2010-03-18
// -----------------------------------------------------------------------------
bool PHY_PerceptionRadar::IsUsingActiveRadar( const PHY_RadarClass& radarClass ) const
{
    if( !radarClass.IsActive() )
        return false;
    const PHY_PerceptionRadarData::T_ZoneSet& zones = radarZones_         [ radarClass.GetID() ];
    const bool bRadarEnabledOnPerceiverPos          = radarOnUnitPosition_[ radarClass.GetID() ];
    return bRadarEnabledOnPerceiverPos || !zones.empty();
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadar::Compute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionRadar::Compute( const MIL_Agent_ABC& /*agent*/ ) const
{
    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadar::Execute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
void PHY_PerceptionRadar::Execute( const TER_Agent_ABC::T_AgentPtrVector& /*perceivableAgents*/ )
{
    const auto radarClasses = PHY_RadarClass::GetRadarClasses(); 
    for( auto itRadarClass = radarClasses.begin(); itRadarClass != radarClasses.end(); ++itRadarClass )
    {
        const PHY_PerceptionRadarData::T_ZoneSet& zones = radarZones_         [ itRadarClass->second->GetID() ];
        const bool bRadarEnabledOnPerceiverPos          = radarOnUnitPosition_[ itRadarClass->second->GetID() ];
        if( !bRadarEnabledOnPerceiverPos && zones.empty() )
            continue;
        const auto& radars = perceiver_.GetRadars( *itRadarClass->second );
        for( auto itRadar = radars.begin(); itRadar != radars.end(); ++itRadar )
        {
            const PHY_RadarType* radarType = itRadar->first;
            double radarHeight = itRadar->second;
            auto itRadarData = radarData_.find( radarType );
            if( itRadarData == radarData_.end() )
                radarData_[ radarType ] = PHY_PerceptionRadarData( *radarType, radarHeight );
            else // Use max height for a given radar type
                radarData_[ radarType ].SetMinHeight( radarHeight );
            radarData_[ radarType ].Acquire( perceiver_, zones, bRadarEnabledOnPerceiverPos );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadar::FinalizePerception
// Created: LDC 2010-05-05
// -----------------------------------------------------------------------------
void PHY_PerceptionRadar::FinalizePerception()
{
    // NOTHING
}
