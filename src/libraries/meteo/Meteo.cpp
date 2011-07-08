// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "Meteo.h"
#include "PHY_Lighting.h"
#include "PHY_Precipitation.h"
#include "ReadDirections.h"
#include "clients_kernel/Tools.h"
#include "protocol/ClientSenders.h"
#include "protocol/ClientPublisher_ABC.h"
#include <xeumeuleu/xml.hpp>

using namespace weather;

namespace
{
    double ConvertSpeedMosToSim( unsigned int timeStep )
    {
        return ( timeStep ) ? 1000. * timeStep / 3600. : 1.;
    }
}

// -----------------------------------------------------------------------------
// Name: Meteo constructor
// Created: ABR 2011-06-01
// -----------------------------------------------------------------------------
Meteo::Meteo( unsigned int id, unsigned int timeStep, const std::string& name /*= ""*/ )
    : pLighting_       ( &PHY_Lighting::jourSansNuage_ )
    , pPrecipitation_  ( &PHY_Precipitation::none_ )
    , id_              ( id )
    , name_            ( name )
    , conversionFactor_( ConvertSpeedMosToSim( timeStep ) )
    , modified_        ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Meteo constructor
// Created: ABR 2011-06-01
// -----------------------------------------------------------------------------
Meteo::Meteo( unsigned int id, xml::xistream& xis, const PHY_Lighting* light, unsigned int timeStep, const std::string& name /*= ""*/ )
    : pLighting_( ( light ) ? light : &PHY_Lighting::jourSansNuage_ )
    , id_       ( id )
    , name_     ( name )
    , conversionFactor_( ConvertSpeedMosToSim( timeStep ) )
    , modified_        ( false )
{
    xis >> xml::start( "cloud-cover" )
            >> xml::attribute( "floor", cloud_.nFloor_ )
            >> xml::attribute( "ceiling", cloud_.nCeiling_ )
            >> xml::attribute( "density", cloud_.nDensityPercentage_ )
        >> xml::end;
    cloud_.nDensityPercentage_ = std::min( cloud_.nDensityPercentage_, 100 );
    cloud_.rDensity_ = cloud_.nDensityPercentage_ / 100.;

    xis >> xml::start( "wind" )
            >> xml::attribute( "speed", wind_.rSpeed_ )
            >> xml::attribute( "direction", wind_.eAngle_ )
        >> xml::end;
    if( wind_.rSpeed_ < 0 )
        xis.error( "meteo: VitesseVent < 0" );
    if( wind_.eAngle_ < 0 || wind_.eAngle_ > 360 )
        xis.error( "meteo: DirectionVent not in [0..360]" );
    wind_.vDirection_ = ReadDirection( wind_.eAngle_ );

    std::string strVal;
    xis >> xml::start( "precipitation" )
            >> xml::attribute( "value", strVal )
        >> xml::end;
    pPrecipitation_ = PHY_Precipitation::FindPrecipitation( strVal );
    if( !pPrecipitation_ )
        xis.error( "Unknown Precipitation type '" + strVal + "'" );

    wind_.rSpeed_ *= conversionFactor_;
    Update( *pLighting_ );
}

// -----------------------------------------------------------------------------
// Name: Meteo constructor
// Created: ABR 2011-06-06
// -----------------------------------------------------------------------------
Meteo::Meteo( unsigned int id, const sword::WeatherAttributes& msg, unsigned int timeStep, const std::string& name /*= ""*/ )
    : id_              ( id )
    , name_            ( name )
    , conversionFactor_( ConvertSpeedMosToSim( timeStep ) )
    , modified_        ( false )
{
    Update( msg );
}

// -----------------------------------------------------------------------------
// Name: Meteo constructor
// Created: ABR 2011-06-08
// -----------------------------------------------------------------------------
Meteo::Meteo( unsigned int id, const sword::MissionParameters& msg, const PHY_Lighting& light, unsigned int timeStep, const std::string& name /*= "" */ )
    : id_              ( id )
    , name_            ( name )
    , conversionFactor_( ConvertSpeedMosToSim( timeStep ) )
    , modified_        ( false )
{
    Update( msg );
    Update( light );
}

// -----------------------------------------------------------------------------
// Name: Meteo constructor
// Created: ABR 2011-06-01
// -----------------------------------------------------------------------------
Meteo::Meteo( unsigned int id, const PHY_Lighting& light, const PHY_Precipitation& precipitation, unsigned int timeStep, const std::string& name /*= ""*/ )
    : pLighting_       ( &light )
    , pPrecipitation_  ( &precipitation )
    , id_              ( id )
    , name_            ( name )
    , conversionFactor_( ConvertSpeedMosToSim( timeStep ) )
    , modified_        ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Meteo destructor
// Created: ABR 2011-06-01
// -----------------------------------------------------------------------------
Meteo::~Meteo()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Meteo::Update
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
void Meteo::Update( const sword::WeatherAttributes& msg )
{
    // Plancher de couverture nuageuse
    cloud_.nFloor_ = msg.cloud_floor();
    // Plafond de couverture nuageuse
    cloud_.nCeiling_ = msg.cloud_ceiling();
    // Densite moyenne de couverture nuageuse
    cloud_.nDensityPercentage_ = std::min( msg.cloud_density(), 100 );
    cloud_.rDensity_ = cloud_.nDensityPercentage_ / 100.;

    // Vitesse du vent
    wind_.rSpeed_ = conversionFactor_ * msg.wind_speed();
    // Direction du vent
    wind_.eAngle_ = msg.wind_direction().heading();
    wind_.vDirection_ = weather::ReadDirection( wind_.eAngle_ );

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
// Name: Meteo::Serialize
// Created: ABR 2011-06-01
// -----------------------------------------------------------------------------
void Meteo::Serialize( xml::xostream& xos ) const
{
    assert( pPrecipitation_ );
    xos << xml::start( "wind" )
            << xml::attribute( "speed", wind_.rSpeed_ )
            << xml::attribute( "direction", wind_.eAngle_ )
        << xml::end
        << xml::start( "cloud-cover" )
            << xml::attribute( "floor", cloud_.nFloor_ )
            << xml::attribute( "ceiling", cloud_.nCeiling_ )
            << xml::attribute( "density", cloud_.nDensityPercentage_ )
        << xml::end
        << xml::start( "precipitation" )
            << xml::attribute( "value", tools::GetXmlSection( pPrecipitation_->GetID() ) )
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: Meteo::Update
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
void Meteo::Update( const sword::MissionParameters& msg )
{
    modified_ = true;

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
    wind_.rSpeed_ = conversionFactor_ * windSpeed.value().Get(0).areal();

    // Direction du vent
    const sword::MissionParameter& windDirection = msg.elem( 2 );
    if( windDirection.null_value() || !windDirection.value().Get(0).has_heading() )
        throw std::exception( "Meteo : bad attribute for windDirection" );
    wind_.eAngle_ = windDirection.value().Get(0).heading().heading();
    wind_.vDirection_ = weather::ReadDirection( wind_.eAngle_ );

    // Plancher de couverture nuageuse
    const sword::MissionParameter& cloudFloor = msg.elem( 3 );
    if( cloudFloor.null_value() || !cloudFloor.value().Get(0).has_areal() )
        throw std::exception( "Meteo : bad attribute for cloudFloor" );
    cloud_.nFloor_ = (int) cloudFloor.value().Get(0).areal();

    // Plafond de couverture nuageuse
    const sword::MissionParameter& cloudCeiling = msg.elem( 4 );
    if( cloudCeiling.null_value() || !cloudCeiling.value().Get(0).has_areal() )
        throw std::exception( "Meteo : bad attribute for cloudCeiling" );
    cloud_.nCeiling_ = (int) cloudCeiling.value().Get(0).areal();

    // Densite moyenne de couverture nuageuse
    const sword::MissionParameter& cloudDensity = msg.elem( 5 );
    if( cloudDensity.null_value() || !cloudDensity.value().Get(0).has_areal() )
        throw std::exception( "Meteo : bad attribute for cloudDensity" );
    cloud_.nDensityPercentage_ = std::min( std::max( (int) cloudDensity.value().Get(0).areal(), 0 ), 100 );
    cloud_.rDensity_ = cloud_.nDensityPercentage_ / 100.;

    // Précipitation
    const sword::MissionParameter& precipitation = msg.elem( 6 );
    if( precipitation.null_value() || !precipitation.value().Get(0).has_enumeration() )
        throw std::exception( "Meteo : bad attribute for precipitation" );
    pPrecipitation_ = PHY_Precipitation::FindPrecipitation( (sword::WeatherAttributes::EnumPrecipitationType ) precipitation.value().Get(0).enumeration() );
    if( !pPrecipitation_ )
        pPrecipitation_ = &PHY_Precipitation::none_;
}

//-----------------------------------------------------------------------------
// Name: Meteo::Update
// Created: JVT 03-08-07
//-----------------------------------------------------------------------------
void Meteo::Update( const PHY_Lighting& light )
{
    pLighting_ = &light.GetDegradedLighting( (unsigned int)( cloud_.rDensity_ * ( cloud_.nCeiling_ - cloud_.nFloor_ ) / 2000 ) );
    modified_ = true;
}

// -----------------------------------------------------------------------------
// Name: Meteo::Update
// Created: HBD 2010-04-06
// -----------------------------------------------------------------------------
void Meteo::Update( const PHY_Precipitation& precipitation)
{
    pPrecipitation_ = &precipitation;
    modified_ = true;
}

// -----------------------------------------------------------------------------
// Name: Meteo::UpdateMeteoPatch
// Created: SLG 2010-03-18
// -----------------------------------------------------------------------------
void Meteo::UpdateMeteoPatch( int /*date*/, PHY_RawVisionData_ABC& /*dataVision_*/, boost::shared_ptr< Meteo > /*meteo*/ )
{
    if( modified_ )
    {
        modified_ = false;
        SendCreation();
    }
}

// -----------------------------------------------------------------------------
// Name: Meteo::SendCreation
// Created: HBD 2010-03-31
// -----------------------------------------------------------------------------
void Meteo::SendCreation( dispatcher::ClientPublisher_ABC& publisher ) const
{
    client::ControlGlobalWeather msg;
    sword::WeatherAttributes* att = msg().mutable_attributes();
    msg().mutable_weather()->set_id( id_ );
    att->set_wind_speed( static_cast< int >( wind_.rSpeed_ / conversionFactor_ ) );
    att->mutable_wind_direction()->set_heading( wind_.eAngle_ );
    att->set_cloud_floor( cloud_.nFloor_ );
    att->set_cloud_ceiling( cloud_.nCeiling_ );
    att->set_cloud_density( cloud_.nDensityPercentage_ );
    att->set_precipitation( pPrecipitation_->GetAsnID() );
    att->set_temperature( 0 );
    att->set_lighting( pLighting_->GetAsnID() );
    msg.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Meteo::SendFullUpdate
// Created: HBD 2010-03-31
// -----------------------------------------------------------------------------
void Meteo::SendFullUpdate( dispatcher::ClientPublisher_ABC& ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Meteo::SendCreation
// Created: HBD 2010-03-31
// -----------------------------------------------------------------------------
void Meteo::SendCreation() const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Meteo::SendDestruction
// Created: HBD 2010-03-31
// -----------------------------------------------------------------------------
void Meteo::SendDestruction( dispatcher::ClientPublisher_ABC& ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Meteo::GetName
// Created: ABR 2011-06-01
// -----------------------------------------------------------------------------
inline
QString Meteo::GetName() const
{
    return name_.c_str();
}

// -----------------------------------------------------------------------------
// Name: Meteo::Select
// Created: ABR 2011-06-01
// -----------------------------------------------------------------------------
void Meteo::Select( kernel::ActionController& /*controller*/ ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Meteo::ContextMenu
// Created: ABR 2011-06-01
// -----------------------------------------------------------------------------
void Meteo::ContextMenu( kernel::ActionController& /*controller*/, const QPoint& /*where*/ ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Meteo::Activate
// Created: ABR 2011-06-01
// -----------------------------------------------------------------------------
void Meteo::Activate( kernel::ActionController& /*controller*/ ) const
{
    // NOTHING

}
