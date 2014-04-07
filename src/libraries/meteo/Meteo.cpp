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
#include "protocol/ClientSenders.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/EnumMaps.h"
#include "protocol/MessageParameters.h"
#include "Tools/NET_AsnException.h"
#include <tools/Exception.h>
#include <xeumeuleu/xml.hpp>

using namespace weather;

BOOST_CLASS_EXPORT_IMPLEMENT( Meteo )

namespace
{
    double ConvertSpeedMosToSim( unsigned int timeStep )
    {
        return ( timeStep ) ? 1000. * timeStep / 3600. : 1.;
    }
}

#define MASA_BADPARAM( name ) MASA_BADPARAM_ASN( sword::MagicActionAck_ErrorCode, sword::MagicActionAck::error_invalid_parameter, name )


// -----------------------------------------------------------------------------
// Name: Meteo constructor
// Created: JSR 2011-11-22
// -----------------------------------------------------------------------------
Meteo::Meteo()
{
    // NOTHING
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
    , temperature_     ( 20 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Meteo constructor
// Created: ABR 2011-06-01
// -----------------------------------------------------------------------------
Meteo::Meteo( unsigned int id, xml::xistream& xis, const PHY_Lighting* light, unsigned int timeStep, const std::string& name /*= ""*/ )
    : pLighting_       ( ( light ) ? light : &PHY_Lighting::jourSansNuage_ )
    , id_              ( id )
    , name_            ( name )
    , conversionFactor_( ConvertSpeedMosToSim( timeStep ) )
    , modified_        ( false )
    , temperature_     ( 20 )
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
        throw MASA_EXCEPTION( xis.context() + "meteo: VitesseVent < 0" );
    if( wind_.eAngle_ < 0 || wind_.eAngle_ >= 360 )
        throw MASA_EXCEPTION( xis.context() + "meteo: DirectionVent not in [0..360[" );
    wind_.vDirection_ = ReadDirection( wind_.eAngle_ );
    xis >> xml::optional
        >> xml::start( "temperature" )
            >> xml::attribute( "value", temperature_ )
        >> xml::end;
    std::string strVal;
    xis >> xml::start( "precipitation" )
            >> xml::attribute( "value", strVal )
        >> xml::end;
    pPrecipitation_ = PHY_Precipitation::FindPrecipitation( strVal );
    if( !pPrecipitation_ )
        throw MASA_EXCEPTION( xis.context() + "Unknown Precipitation type '" + strVal + "'" );

    wind_.rSpeed_ *= conversionFactor_;
    SetLighting( *pLighting_ );
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
    , temperature_     ( 20 )
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
    , temperature_     ( 20 )
{
    Update( msg );
    SetLighting( light );
}

// -----------------------------------------------------------------------------
// Name: Meteo constructor
// Created: ABR 2011-06-01
// -----------------------------------------------------------------------------
Meteo::Meteo( unsigned int id, const PHY_Lighting& light, const PHY_Precipitation& precipitation,
              unsigned int timeStep, unsigned int temperature, const std::string& name /*= ""*/ )
    : pLighting_       ( &light )
    , pPrecipitation_  ( &precipitation )
    , id_              ( id )
    , name_            ( name )
    , conversionFactor_( ConvertSpeedMosToSim( timeStep ) )
    , modified_        ( false )
    , temperature_     ( temperature )
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
    modified_ = true;

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

    // Temperature
    temperature_ = msg.temperature();

    // Précipitation
    pPrecipitation_ = PHY_Precipitation::FindPrecipitation( protocol::FromProto(
                msg.precipitation() ));
    if( !pPrecipitation_ )
        pPrecipitation_ = &PHY_Precipitation::none_;
    // Lighting
    pLighting_ = PHY_Lighting::FindLighting( protocol::FromProto( msg.lighting() ));
    if( !pLighting_ )
        pLighting_ = &PHY_Lighting::jourSansNuage_;
}

namespace
{
    const char* GetXmlSection( E_WeatherType weather )
    {
        switch( weather  )
        {
        case eWeatherType_None:      return "PasDePrecipitation";
        case eWeatherType_SandStorm: return "TempeteDeSable";
        case eWeatherType_Fog:       return "Brouillard";
        case eWeatherType_Drizzle:   return "Crachin";
        case eWeatherType_Rain:      return "Pluie";
        case eWeatherType_Snow:      return "Neige";
        case eWeatherType_Smoke:     return "Fumigene";
        default:
            return "Unknown";
        }
    }
}

// -----------------------------------------------------------------------------
// Name: Meteo::Serialize
// Created: ABR 2011-06-01
// -----------------------------------------------------------------------------
void Meteo::Serialize( xml::xostream& xos ) const
{
    assert( pPrecipitation_ );
    xos << xml::start( "wind" )
            << xml::attribute( "speed", wind_.rSpeed_ / conversionFactor_ )
            << xml::attribute( "direction", wind_.eAngle_ )
        << xml::end
        << xml::start( "cloud-cover" )
            << xml::attribute( "floor", cloud_.nFloor_ )
            << xml::attribute( "ceiling", cloud_.nCeiling_ )
            << xml::attribute( "density", cloud_.nDensityPercentage_ )
        << xml::end
        << xml::start( "temperature" )
            << xml::attribute( "value", temperature_ )
        << xml::end
        << xml::start( "precipitation" )
            << xml::attribute( "value", GetXmlSection( pPrecipitation_->GetID() ) )
        << xml::end;
}

namespace
{
    float GetReal( const sword::MissionParameters& params, int i, bool positive = true )
    {
        const float value = protocol::GetReal( params, i );
        if( positive && value < 0.f )
            throw MASA_BADPARAM( STR( "parameters[" << i << "] must be a positive number" ) );
        return value;
    }
}

// -----------------------------------------------------------------------------
// Name: Meteo::Update
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
void Meteo::Update( const sword::MissionParameters& msg )
{
    protocol::Check( protocol::GetCount( msg ) >= 7, " at least 7 parameters expected" );
    const int temperature = static_cast< int >( GetReal( msg, 0, false ));
    const float windSpeed = GetReal( msg, 1 );
    const int angle = protocol::GetHeading( msg, 2 );
    if( angle < 0 || angle > 360 )
        throw MASA_BADPARAM( "parameters[2] must be a number between 0 and 360" );
    const int cloudFloor = static_cast< int >( GetReal( msg, 3 ) );
    const int cloudCeiling = static_cast< int >( GetReal( msg, 4 ) );
    const int cloudDensity = static_cast< int >( GetReal( msg, 5 ) );
    const auto precipitation = GET_ENUMERATION( sword::WeatherAttributes::EnumPrecipitationType, msg, 6 );
    const PHY_Precipitation* pPrecipitation = PHY_Precipitation::FindPrecipitation(
        protocol::FromProto( precipitation ) );
    if( !pPrecipitation )
        throw MASA_BADPARAM( "parameters[6] must be a precipitation Enumeration" );

    modified_ = true;
    pPrecipitation_ = pPrecipitation;
    temperature_ = temperature;
    wind_.rSpeed_ = conversionFactor_ * windSpeed;
    wind_.eAngle_ = angle;
    wind_.vDirection_ = weather::ReadDirection( wind_.eAngle_ );
    cloud_.nFloor_ = cloudFloor;
    cloud_.nCeiling_ = cloudCeiling;
    cloud_.nDensityPercentage_ = std::min( std::max( cloudDensity, 0 ), 100 );
    cloud_.rDensity_ = cloud_.nDensityPercentage_ / 100.;
}

// -----------------------------------------------------------------------------
// Name: Meteo::Update
// Created: ABR 2011-07-27
// -----------------------------------------------------------------------------
void Meteo::Update( const Meteo& other )
{
    wind_             = other.GetWind();
    cloud_            = other.GetCloud();
    temperature_      = other.temperature_;
    pLighting_        = &other.GetLighting();
    pPrecipitation_   = &other.GetPrecipitation();
    conversionFactor_ = other.GetConversionFactor();
    modified_ = true;
}

// -----------------------------------------------------------------------------
// Name: Meteo::SendCreationIfModified
// Created: SLG 2010-03-18
// -----------------------------------------------------------------------------
void Meteo::SendCreationIfModified()
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
    att->set_precipitation( protocol::ToProto( pPrecipitation_->GetID() ));
    att->set_temperature( temperature_ );
    att->set_lighting( protocol::ToProto( pLighting_->GetID() ));
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
// Name: Meteo::SendDestruction
// Created: ABR 2011-11-28
// -----------------------------------------------------------------------------
void Meteo::SendDestruction() const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Meteo::GetTemperature
// Created: LGY 2011-08-24
// -----------------------------------------------------------------------------
int Meteo::GetTemperature() const
{
    return temperature_;
}

// -----------------------------------------------------------------------------
// Name: Meteo::SetTemperature
// Created: LGY 2011-08-24
// -----------------------------------------------------------------------------
void Meteo::SetTemperature( int temperature )
{
    temperature_ = temperature;
}

// -----------------------------------------------------------------------------
// Name: Meteo::GetID
// Created: JSR 2011-11-22
// -----------------------------------------------------------------------------
unsigned long Meteo::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: Meteo::GetName
// Created: JSR 2011-11-22
// -----------------------------------------------------------------------------
const std::string& Meteo::GetName() const
{
    return name_;
}

//-----------------------------------------------------------------------------
// Name: Meteo::GetLighting
// Created: JVT 03-08-05
//-----------------------------------------------------------------------------
const PHY_Lighting& Meteo::GetLighting() const
{
    assert( pLighting_ );
    return *pLighting_;
}

//-----------------------------------------------------------------------------
// Name: Meteo::GetPrecipitation
// Created: JVT 03-08-05
//-----------------------------------------------------------------------------
const PHY_Precipitation& Meteo::GetPrecipitation() const
{
    assert( pPrecipitation_ );
    return *pPrecipitation_;
}

// -----------------------------------------------------------------------------
// Name: Meteo::GetWind
// Created: JVT 2004-10-28
// -----------------------------------------------------------------------------
const WindData& Meteo::GetWind() const
{
    return wind_;
}

// -----------------------------------------------------------------------------
// Name: Meteo::GetCloud
// Created: ABR 2011-06-01
// -----------------------------------------------------------------------------
const Meteo::sCloudData& Meteo::GetCloud() const
{
    return cloud_;
}

// -----------------------------------------------------------------------------
// Name: Meteo::SetPrecipitation
// Created: ABR 2011-06-01
// -----------------------------------------------------------------------------
void Meteo::SetPrecipitation( const PHY_Precipitation& precipitation )
{
    if( pPrecipitation_ != &precipitation )
        modified_ = true;
    pPrecipitation_ = &precipitation;
}

// -----------------------------------------------------------------------------
// Name: Meteo::SetLighting
// Created: ABR 2011-06-01
// -----------------------------------------------------------------------------
void Meteo::SetLighting( const PHY_Lighting& light )
{
    if( pLighting_ != &light )
        modified_ = true;
    pLighting_ = &light;
}

// -----------------------------------------------------------------------------
// Name: Meteo::SetWind
// Created: ABR 2011-06-01
// -----------------------------------------------------------------------------
void Meteo::SetWind( const WindData& wind )
{
    if( wind_.eAngle_ != wind.eAngle_ || wind_.rSpeed_ != wind.rSpeed_ )
        modified_ = true;
    wind_ = wind;
}

// -----------------------------------------------------------------------------
// Name: Meteo::SetCloud
// Created: ABR 2011-06-01
// -----------------------------------------------------------------------------
void Meteo::SetCloud( const sCloudData& cloud )
{
    if( cloud_.nCeiling_ != cloud.nCeiling_ || cloud_.nFloor_ != cloud.nFloor_ || cloud_.nDensityPercentage_ != cloud.nDensityPercentage_ || cloud_.rDensity_ != cloud.rDensity_ )
        modified_ = true;
    cloud_ = cloud;
}

// -----------------------------------------------------------------------------
// Name: Meteo::GetConversionFactor
// Created: ABR 2011-06-06
// -----------------------------------------------------------------------------
double Meteo::GetConversionFactor() const
{
    return conversionFactor_;
}

// -----------------------------------------------------------------------------
// Name: Meteo::IsModified
// Created: ABR 2011-06-07
// -----------------------------------------------------------------------------
bool Meteo::IsModified() const
{
    return modified_;
}

// -----------------------------------------------------------------------------
// Name: Meteo::SetModified
// Created: ABR 2011-06-07
// -----------------------------------------------------------------------------
void Meteo::SetModified( bool modified )
{
    modified_ = modified;
}

// -----------------------------------------------------------------------------
// Name: Meteo::IsInside
// Created: ABR 2011-06-06
// -----------------------------------------------------------------------------
bool Meteo::IsInside( const geometry::Point2f& /*point*/ ) const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: Meteo::IsYounger
// Created: ABR 2012-03-23
// -----------------------------------------------------------------------------
bool Meteo::IsOlder( const weather::Meteo& /*other*/ ) const
{
    return true;
}
