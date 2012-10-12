// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "PerceptionRadar.h"
#include "PerceptionLevel.h"
#include "RadarClass.h"
#include "RadarType.h"
#include "wrapper/View.h"
#include "wrapper/Hook.h"
#include "wrapper/Effect.h"
#include <boost/bind.hpp>
#include <boost/foreach.hpp>

using namespace sword;
using namespace sword::perception;

DECLARE_HOOK( IsUsingActiveRadar, bool, ( const SWORD_Model* entity ) )

// -----------------------------------------------------------------------------
// Name: PerceptionRadar constructor
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
PerceptionRadar::PerceptionRadar( const wrapper::View& entity, PerceptionObserver_ABC& observer )
    : observer_           ( observer )
    , radarOnUnitPosition_( RadarClass::GetNbrClasses() )
    , radarZones_         ( RadarClass::GetNbrClasses() )
{
    EnableRadar( entity[ "perceptions/radars/radar" ], RadarClass::radar_ );
    EnableRadar( entity[ "perceptions/radars/tapping" ], RadarClass::tapping_ );
    EnableRadar( entity[ "perceptions/radars/tapping-radar" ], RadarClass::tappingRadar_ );
    entity[ "perceptions/localized-radars/radar" ].VisitIdentifiedChildren( boost::bind( &PerceptionRadar::EnableLocalizedRadar, this, _2, boost::cref( RadarClass::radar_ ) ) );
    entity[ "perceptions/localized-radars/tapping" ].VisitIdentifiedChildren( boost::bind( &PerceptionRadar::EnableLocalizedRadar, this, _2, boost::cref( RadarClass::tapping_ ) ) );
    entity[ "perceptions/localized-radars/tapping-radar" ].VisitIdentifiedChildren( boost::bind( &PerceptionRadar::EnableLocalizedRadar, this, _2, boost::cref( RadarClass::tappingRadar_ ) ) );
}

// -----------------------------------------------------------------------------
// Name: PerceptionRadar destructor
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
PerceptionRadar::~PerceptionRadar()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PerceptionRadar::EnableRadar
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
void PerceptionRadar::EnableRadar( const wrapper::View& radar, const RadarClass& radarClass )
{
    if( radar[ "activated" ] )
    {
        assert( radarOnUnitPosition_.size() > radarClass.GetID() );
        radarOnUnitPosition_[ radarClass.GetID() ] = true;
    }
}

// -----------------------------------------------------------------------------
// Name: PerceptionRadar::EnableRadar
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
void PerceptionRadar::EnableLocalizedRadar( const wrapper::View& radar, const RadarClass& radarClass )
{
    assert( radarZones_.size() > radarClass.GetID() );
    const wrapper::View& zone = radar[ "localization" ];
    if( !radarZones_[ radarClass.GetID() ].insert( zone ).second )
        throw std::runtime_error( "Insertion failed in perception radar" );
}

// -----------------------------------------------------------------------------
// Name: PerceptionRadar::PrepareRadarData
// Created: SLI 2012-08-30
// -----------------------------------------------------------------------------
void PerceptionRadar::PrepareRadarData( const wrapper::View& entity, T_RadarTypesMap& types ) const
{
    for( std::size_t i = 0; i < entity[ "components" ].GetSize(); ++i )
    {
        const wrapper::View& component = entity[ "components" ].GetElement( i );
        if( !component[ "can-perceive" ] )
            break;
        for( std::size_t j = 0; j < component[ "radars" ].GetSize(); ++j )
        {
            const wrapper::View& radar = component[ "radars" ].GetElement( j );
            const RadarType* radarType = RadarType::Find( radar[ "type" ] );
            if( !radarType )
                throw std::runtime_error( "unknown radar type " + radar[ "type" ] );
            types[ radarType->GetClass().GetID() ].insert( radarType );
        }
    }
}

namespace
{
    template< typename T >
    void AddRadarType( const std::string& key, const wrapper::View& entity, T& radarTypes )
    {
        const RadarType* radarType = RadarType::Find( key );
        radarTypes[ radarType ] = boost::shared_ptr< PerceptionRadarData >( new PerceptionRadarData( entity, *radarType ) );
    }
}

// -----------------------------------------------------------------------------
// Name: PerceptionRadar::Execute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
void PerceptionRadar::ExecuteAgents( const wrapper::View& model, const wrapper::View& perceiver, const SurfacesAgent_ABC& /*surfaces*/, const T_AgentPtrVector& /*perceivableAgents*/ )
{
    if( !GET_HOOK( IsUsingActiveRadar )( perceiver ) )
        return;
    T_RadarDataMap radarData;
    T_RadarTypesMap radarTypes;
    wrapper::Effect effect( perceiver[ "perceptions/radars/acquisitions" ] );
    perceiver[ "perceptions/radars/acquisitions" ].VisitNamedChildren( boost::bind( &::AddRadarType< T_RadarDataMap >, _1, boost::cref( perceiver ), boost::ref( radarData ) ) );
    PrepareRadarData( perceiver, radarTypes );
    RadarClass::T_RadarClassMap radarClasses = RadarClass::GetRadarClasses();
    for( RadarClass::CIT_RadarClassMap itRadarClass = radarClasses.begin(); itRadarClass != radarClasses.end(); ++itRadarClass )
    {
        const PerceptionRadarData::T_ZoneSet& zones = radarZones_[ itRadarClass->second->GetID() ];
        const bool bRadarEnabledOnPerceiverPos = radarOnUnitPosition_[ itRadarClass->second->GetID() ];

        if( !bRadarEnabledOnPerceiverPos && zones.empty() )
            continue;
        const T_RadarSet& radars = radarTypes[ itRadarClass->second->GetID() ];
        for( T_RadarSet::const_iterator itRadar = radars.begin(); itRadar != radars.end(); ++itRadar )
        {
            const RadarType& radarType = **itRadar;
            T_RadarDataMap::iterator itRadarData = radarData.find( &radarType );
            if( itRadarData == radarData.end() )
            {
                itRadarData  = radarData.insert( std::make_pair( &radarType, T_Data( new PerceptionRadarData( radarType ) ) ) ).first;
                radarTypes[ radarType.GetClass().GetID() ].insert( &radarType );
            }
            itRadarData->second->Acquire( model, perceiver, effect, observer_, zones, bRadarEnabledOnPerceiverPos ); // new radar type, add it with effect
        }
    }
    effect.Post();
}

// -----------------------------------------------------------------------------
// Name: PerceptionRadar::HasRadar
// Created: SLI 2012-08-30
// -----------------------------------------------------------------------------
bool PerceptionRadar::HasRadar( const wrapper::View& entity, std::size_t radarType ) const
{
    const RadarClass* radar = RadarClass::Find( radarType );
    if( !radar )
        throw std::runtime_error( "Invalid radar in AgentHasTappingRadar" );
    T_RadarTypesMap radarTypes;
    PrepareRadarData( entity, radarTypes );
    T_RadarTypesMap::const_iterator it = radarTypes.find( radar->GetID() );
    return it != radarTypes.end() && !it->second.empty();
}
