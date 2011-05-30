// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "PHY_Meteo.h"
#include "PHY_Lighting.h"
#include "PHY_Precipitation.h"
#include "MeteoManager_ABC.h"
#include "ReadDirections.h"
#include "PHY_RawVisionData_ABC.h"
#include "protocol/Protocol.h"
#include "protocol/ServerPublisher_ABC.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/ClientSenders.h"
#include <xeumeuleu/xml.hpp>
#pragma warning( push, 0 )
#include <qstring.h>
#pragma warning( pop )

using namespace weather;

namespace
{
    int ConvertSpeedMosToSim( unsigned int timeStep )
    {
        return static_cast< int >( 1000. * timeStep / 3600. );
    }
}

//-----------------------------------------------------------------------------
// Name: PHY_Meteo constructor
// Created: JVT 03-08-05
//-----------------------------------------------------------------------------
PHY_Meteo::PHY_Meteo( unsigned int id, xml::xistream& xis, const PHY_Lighting& light, unsigned int timeStep )
    : pLighting_       ( &PHY_Lighting::jourSansNuage_ )
    , pPrecipitation_  ( &PHY_Precipitation::none_ )
    , nRefCount_       ( 0 )
    , id_              ( id )
    , isChanged_       ( false )
    , conversionFactor_( ConvertSpeedMosToSim( timeStep ) )
{
    unsigned int nVal;
    xis >> xml::start( "cloud-cover" )
            >> xml::attribute( "floor", nPlancherCouvertureNuageuse_ )
            >> xml::attribute( "ceiling", nPlafondCouvertureNuageuse_ )
            >> xml::attribute( "density", nVal )
        >> xml::end;
    rDensiteCouvertureNuageuse_ = std::min( nVal, 100u ) / 100.;

    xis >> xml::start( "wind" )
            >> xml::attribute( "speed", wind_.rWindSpeed_ )
            >> xml::attribute( "direction", wind_.eWindAngle_ )
        >> xml::end;
    if( wind_.rWindSpeed_ < 0 )
        xis.error( "meteo: VitesseVent < 0" );

    wind_.rWindSpeed_ = conversionFactor_ * wind_.rWindSpeed_;
    if( wind_.eWindAngle_ < 0 || wind_.eWindAngle_ > 360 )
        xis.error( "meteo: DirectionVent not in [0..360]" );
    wind_.vWindDirection_ = ReadDirection( wind_.eWindAngle_ );

    std::string strVal;
    xis >> xml::start( "precipitation" )
            >> xml::attribute( "value", strVal )
        >> xml::end;
    pPrecipitation_ = PHY_Precipitation::FindPrecipitation( strVal );
    if( !pPrecipitation_ )
        xis.error( "Unknown Precipitation type '" + strVal + "'" );
    Update( light );
}

//-----------------------------------------------------------------------------
// Name: PHY_Meteo constructor
// Created: JVT 03-08-05
//-----------------------------------------------------------------------------
PHY_Meteo::PHY_Meteo( unsigned int id, const sword::WeatherAttributes& asnMsg, MeteoManager_ABC* listener, unsigned int timeStep )
    : pLighting_       ( &PHY_Lighting::jourSansNuage_ )
    , pPrecipitation_  ( &PHY_Precipitation::none_ )
    , nRefCount_       ( 0 )
    , listener_        ( listener )
    , id_              ( id )
    , isChanged_       ( false )
    , conversionFactor_( ConvertSpeedMosToSim( timeStep ) )
{
    Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: PHY_Meteo constructor
// Created: JSR 2010-04-12
// -----------------------------------------------------------------------------
PHY_Meteo::PHY_Meteo( unsigned int id, const sword::MissionParameters& asnMsg, MeteoManager_ABC* listener, unsigned int timeStep )
    : pLighting_       ( &PHY_Lighting::jourSansNuage_ )
    , pPrecipitation_  ( &PHY_Precipitation::none_ )
    , nRefCount_       ( 0 )
    , listener_        ( listener )
    , id_              ( id )
    , isChanged_       ( false )
    , conversionFactor_( ConvertSpeedMosToSim( timeStep ) )
{
    Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: PHY_Meteo constructor
// Created: HBD 2010-04-06
// -----------------------------------------------------------------------------
PHY_Meteo::PHY_Meteo( const PHY_Lighting& light, PHY_Precipitation& precipitation, unsigned int timeStep )
    : pLighting_       ( &light )
    , pPrecipitation_  ( &precipitation )
    , nRefCount_       ( 0 )
    , listener_        ( 0 )
    , id_              ( 0 )
    , isChanged_       ( false )
    , conversionFactor_( ConvertSpeedMosToSim( timeStep ) )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: PHY_Meteo destructor
// Created: JVT 03-08-05
//-----------------------------------------------------------------------------
PHY_Meteo::~PHY_Meteo()
{
    if( listener_ )
        listener_->UnregisterMeteo( boost::shared_ptr< PHY_Meteo >( this ) );
    assert( nRefCount_ == 0 );
}

// -----------------------------------------------------------------------------
// Name: PHY_Meteo::Update
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
void PHY_Meteo::Update( const sword::WeatherAttributes& msg )
{
    // Plancher de couverture nuageuse
    nPlancherCouvertureNuageuse_ = msg.cloud_floor();

    // Plafond de couverture nuageuse
    nPlafondCouvertureNuageuse_ = msg.cloud_ceiling();

    // Densite moyenne de couverture nuageuse
    rDensiteCouvertureNuageuse_ = std::min( std::max( msg.cloud_density(), 0 ), 100 ) / 100.;

    // Vitesse du vent
    wind_.rWindSpeed_ = conversionFactor_ * msg.wind_speed();

    // Direction du vent
    wind_.eWindAngle_ = msg.wind_direction().heading();
    wind_.vWindDirection_ = weather::ReadDirection( wind_.eWindAngle_ );

    // Précipitation
    pPrecipitation_ = PHY_Precipitation::FindPrecipitation( msg.precipitation() );
    if( !pPrecipitation_ )
        pPrecipitation_ = &PHY_Precipitation::none_;

    // Lighting
    pLighting_ = PHY_Lighting::FindLighting( msg.lighting() );
    if( !pLighting_ )
        pLighting_ = &PHY_Lighting::jourSansNuage_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Meteo::Update
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
void PHY_Meteo::Update( const sword::MissionParameters& msg )
{
    isChanged_ = true;

    // Temperature
    const sword::MissionParameter& temperature = msg.elem( 0 );
    if( temperature.null_value() || !temperature.value().Get(0).has_areal() )
        throw std::exception( "Meteo : bad attribute for temperature" );
    // TODO
    // temperature_ = parametre.value().areal();

    // Vitesse du vent
    const sword::MissionParameter& windSpeed = msg.elem( 1 );
    if( windSpeed.null_value() || !windSpeed.value().Get(0).has_areal() )
        throw std::exception( "Meteo : bad attribute for windSpeed" );
    wind_.rWindSpeed_ = conversionFactor_ * windSpeed.value().Get(0).areal();

    // Direction du vent
    const sword::MissionParameter& windDirection = msg.elem( 2 );
    if( windDirection.null_value() || !windDirection.value().Get(0).has_heading() )
        throw std::exception( "Meteo : bad attribute for windDirection" );
    wind_.eWindAngle_ = windDirection.value().Get(0).heading().heading();
    wind_.vWindDirection_ = weather::ReadDirection( wind_.eWindAngle_ );

    // Plancher de couverture nuageuse
    const sword::MissionParameter& cloudFloor = msg.elem( 3 );
    if( cloudFloor.null_value() || !cloudFloor.value().Get(0).has_areal() )
        throw std::exception( "Meteo : bad attribute for cloudFloor" );
    nPlancherCouvertureNuageuse_ = (int) cloudFloor.value().Get(0).areal();

    // Plafond de couverture nuageuse
    const sword::MissionParameter& cloudCeiling = msg.elem( 4 );
    if( cloudCeiling.null_value() || !cloudCeiling.value().Get(0).has_areal() )
        throw std::exception( "Meteo : bad attribute for cloudCeiling" );
    nPlafondCouvertureNuageuse_ = (int) cloudCeiling.value().Get(0).areal();

    // Densite moyenne de couverture nuageuse
    const sword::MissionParameter& cloudDensity = msg.elem( 5 );
    if( cloudDensity.null_value() || !cloudDensity.value().Get(0).has_areal() )
        throw std::exception( "Meteo : bad attribute for cloudDensity" );
    rDensiteCouvertureNuageuse_ = std::min( std::max( (int) cloudDensity.value().Get(0).areal(), 0 ), 100 ) / 100.;

     // Précipitation
    const sword::MissionParameter& precipitation = msg.elem( 6 );
    if( precipitation.null_value() || !precipitation.value().Get(0).has_enumeration() )
        throw std::exception( "Meteo : bad attribute for precipitation" );
    pPrecipitation_ = PHY_Precipitation::FindPrecipitation( (sword::WeatherAttributes::EnumPrecipitationType ) precipitation.value().Get(0).enumeration() );
    if( !pPrecipitation_ )
        pPrecipitation_ = &PHY_Precipitation::none_;

    // Lighting - $$$$ NLD ??????? C'est quoi ce sac de noeud ...
    if( listener_ )
        pLighting_ = &( listener_->GetLighting() );
    else
        pLighting_ = &PHY_Lighting::jourSansNuage_;
}

//-----------------------------------------------------------------------------
// Name: PHY_Meteo::Update
// Created: JVT 03-08-07
//-----------------------------------------------------------------------------
void PHY_Meteo::Update( const PHY_Lighting& light )
{
    pLighting_ = &light.GetDegradedLighting( (unsigned int)( rDensiteCouvertureNuageuse_ * ( nPlafondCouvertureNuageuse_ - nPlancherCouvertureNuageuse_ ) / 2000 ) );
    isChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_Meteo::Update
// Created: HBD 2010-04-06
// -----------------------------------------------------------------------------
void PHY_Meteo::Update( const PHY_Precipitation& precipitation)
{
    pPrecipitation_ = &precipitation;
    isChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_Meteo::SetListener
// Created: HBD 2010-03-10
// -----------------------------------------------------------------------------
void PHY_Meteo::SetListener( MeteoManager_ABC* listener )
{
    listener_ = listener;
}

// -----------------------------------------------------------------------------
// Name: PHY_Meteo::UpdateMeteoPatch
// Created: SLG 2010-03-18
// -----------------------------------------------------------------------------
void PHY_Meteo::UpdateMeteoPatch( int /*date*/, PHY_RawVisionData_ABC& /*dataVision_*/ )
{
    if( isChanged_ )
    {
        isChanged_ = false;
        SendCreation();
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_Meteo::SendCreation
// Created: HBD 2010-03-31
// -----------------------------------------------------------------------------
void PHY_Meteo::SendCreation( dispatcher::ClientPublisher_ABC& publisher ) const
{
    client::ControlGlobalWeather msg;
    sword::WeatherAttributes* att = msg().mutable_attributes();
    msg().mutable_weather()->set_id( id_ );
    att->set_wind_speed( static_cast< int >( wind_.rWindSpeed_ / conversionFactor_ ) );
    att->mutable_wind_direction()->set_heading( 0 );
    att->set_cloud_floor( nPlancherCouvertureNuageuse_ );
    att->set_cloud_ceiling( nPlafondCouvertureNuageuse_ );
    att->set_cloud_density( static_cast< int >( rDensiteCouvertureNuageuse_ * 100. + 0.01 ) );
    att->set_precipitation( pPrecipitation_->GetAsnID() );
    att->set_temperature( 0 );
    att->set_lighting( pLighting_->GetAsnID() );
    msg.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: PHY_Meteo::SendFullUpdate
// Created: HBD 2010-03-31
// -----------------------------------------------------------------------------
void PHY_Meteo::SendFullUpdate( dispatcher::ClientPublisher_ABC& ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Meteo::Select
// Created: HBD 2010-03-31
// -----------------------------------------------------------------------------
void PHY_Meteo::Select( kernel::ActionController& ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Meteo::ContextMenu
// Created: HBD 2010-03-31
// -----------------------------------------------------------------------------
void PHY_Meteo::ContextMenu( kernel::ActionController&, const QPoint& ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Meteo::Activate
// Created: HBD 2010-03-31
// -----------------------------------------------------------------------------
void PHY_Meteo::Activate( kernel::ActionController& ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Meteo::GetName
// Created: HBD 2010-03-31
// -----------------------------------------------------------------------------
QString PHY_Meteo::GetName() const
{
    return QString();
}

// -----------------------------------------------------------------------------
// Name: PHY_Meteo::GetId
// Created: HBD 2010-03-31
// -----------------------------------------------------------------------------
unsigned long PHY_Meteo::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Meteo::SendCreation
// Created: HBD 2010-03-31
// -----------------------------------------------------------------------------
void PHY_Meteo::SendCreation() const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Meteo::SendDestruction
// Created: HBD 2010-03-31
// -----------------------------------------------------------------------------
void PHY_Meteo::SendDestruction( dispatcher::ClientPublisher_ABC& ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Meteo::IsInside
// Created: HBD 2010-04-01
// -----------------------------------------------------------------------------
bool PHY_Meteo::IsInside( const geometry::Point2f& ) const
{
    return true;
}
