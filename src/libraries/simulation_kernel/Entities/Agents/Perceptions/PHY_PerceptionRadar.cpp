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
#include "Entities/Agents/Roles/Perception/PHY_RolePion_Perceiver.h"
#include "Entities/Agents/Units/Radars/PHY_RadarClass.h"

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadar constructor
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
PHY_PerceptionRadar::PHY_PerceptionRadar( PHY_RolePion_Perceiver& perceiver )
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
    bool bOut = radarZones_[ radarClass.GetID() ].insert( pZone ).second;
    assert( bOut );
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
    for( CIT_RadarZonesVector it = radarZones_.begin(); it != radarZones_.end(); ++it )
        if( !it->empty() )
            return true;
    for( CIT_RadarOnUnitPositionVector it = radarOnUnitPosition_.begin(); it != radarOnUnitPosition_.end(); ++it )
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
    PHY_RadarClass::T_RadarClassMap radarClasses = PHY_RadarClass::GetRadarClasses();
    for( PHY_RadarClass::CIT_RadarClassMap itRadarClass = radarClasses.begin(); itRadarClass != radarClasses.end(); ++itRadarClass )
    {
        if( !itRadarClass->second->IsActive() )
            continue;

        const PHY_PerceptionRadarData::T_ZoneSet& zones = radarZones_         [ itRadarClass->second->GetID() ];
        const bool bRadarEnabledOnPerceiverPos          = radarOnUnitPosition_[ itRadarClass->second->GetID() ];

        if( bRadarEnabledOnPerceiverPos || !zones.empty() )
            return true;
    }
    return false;
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
// Name: PHY_PerceptionRadar::Compute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionRadar::Compute( const DEC_Knowledge_Agent & /*knowledge*/ ) const
{
    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadar::Execute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
void PHY_PerceptionRadar::Execute( const TER_Agent_ABC::T_AgentPtrVector& /*perceivableAgents*/ )
{
    PHY_RadarClass::T_RadarClassMap radarClasses = PHY_RadarClass::GetRadarClasses();
    for( PHY_RadarClass::CIT_RadarClassMap itRadarClass = radarClasses.begin(); itRadarClass != radarClasses.end(); ++itRadarClass )
    {
        const PHY_PerceptionRadarData::T_ZoneSet& zones = radarZones_         [ itRadarClass->second->GetID() ];
        const bool bRadarEnabledOnPerceiverPos          = radarOnUnitPosition_[ itRadarClass->second->GetID() ];

        if( !bRadarEnabledOnPerceiverPos && zones.empty() )
            continue;

        const PHY_RolePion_Perceiver::T_RadarSet& radars = perceiver_.GetRadars( *itRadarClass->second );
        for( PHY_RolePion_Perceiver::CIT_RadarSet itRadar = radars.begin(); itRadar != radars.end(); ++itRadar )
        {
            const PHY_RadarType& radarType = **itRadar;

            IT_RadarDataMap itRadarData = radarData_.find( &radarType );
            if( itRadarData == radarData_.end() )
                itRadarData  = radarData_.insert( std::make_pair( &radarType, PHY_PerceptionRadarData( radarType ) ) ).first;
        
            itRadarData->second.Acquire( perceiver_, zones, bRadarEnabledOnPerceiverPos );
        }
    }
}
