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
    Update( light );
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
    pPrecipitation_ = PHY_Precipitation::FindPrecipitation( protocol::FromProtoPrecipitation(
                msg.precipitation() ));
    if( !pPrecipitation_ )
        pPrecipitation_ = &PHY_Precipitation::none_;
    // Lighting
    pLighting_ = PHY_Lighting::FindLighting( protocol::FromProtoLighting( msg.lighting() ));
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
    void CheckRealParameter( const sword::MissionParameter& parameter, const std::string& text, bool positive = true )
    {
        if( parameter.null_value() || !parameter.value().Get( 0 ).has_areal() )
            throw MASA_BADPARAM( "parameters[" + text + "] must be a Areal" );
        if( positive )
        {
            float value = parameter.value().Get( 0 ).areal();
            if( value < 0.f )
                throw MASA_BADPARAM( "parameters[" + text + "] must be a positive number" );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: Meteo::Update
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
void Meteo::Update( const sword::MissionParameters& msg )
{
    modified_ = true;
    if( msg.elem_size() < 6u )
        throw MASA_BADPARAM( "invalid parameters count, 7 parameters expected" );

    // Temperature
    const sword::MissionParameter& temperature = msg.elem( 0 );
    CheckRealParameter( temperature, "0", false );
    // Vitesse du vent
    const sword::MissionParameter& windSpeed = msg.elem( 1 );
    CheckRealParameter( windSpeed, "1" );
    // Direction du vent
    const sword::MissionParameter& windDirection = msg.elem( 2 );
    if( windDirection.null_value() || !windDirection.value().Get( 0 ).has_heading() )
        throw MASA_BADPARAM( "parameters[2] must be a Heading" );
    int angle = windDirection.value().Get( 0 ).heading().heading();
    if( angle < 0 || angle > 360 )
        throw MASA_BADPARAM( "parameters[2] must be a number between 0 and 360" );

    // Plancher de couverture nuageuse
    const sword::MissionParameter& cloudFloor = msg.elem( 3 );
    CheckRealParameter( cloudFloor, "3" );
    // Plafond de couverture nuageuse
    const sword::MissionParameter& cloudCeiling = msg.elem( 4 );
    CheckRealParameter( cloudCeiling, "4" );
    // Densite moyenne de couverture nuageuse
    const sword::MissionParameter& cloudDensity = msg.elem( 5 );
    CheckRealParameter( cloudDensity, "5" );
    // Précipitation
    const sword::MissionParameter& precipitation = msg.elem( 6 );
    if( precipitation.null_value() || !precipitation.value().Get( 0 ).has_enumeration() )
        throw MASA_BADPARAM( "parameters[6] must be an Enumeration" );
    const PHY_Precipitation* pPrecipitation = PHY_Precipitation::FindPrecipitation(
        protocol::FromProtoPrecipitation( static_cast< sword::WeatherAttributes::EnumPrecipitationType >( precipitation.value().Get( 0 ).enumeration() ) ));
    if( !pPrecipitation )
        throw MASA_BADPARAM( "parameters[6] must be a precipitation Enumeration" );
    pPrecipitation_ = pPrecipitation;

    temperature_ = static_cast< int >( temperature.value().Get( 0 ).areal() );
    wind_.rSpeed_ = conversionFactor_ * windSpeed.value().Get( 0 ).areal();
    wind_.eAngle_ = angle;
    wind_.vDirection_ = weather::ReadDirection( wind_.eAngle_ );
    cloud_.nFloor_ = static_cast< int >( cloudFloor.value().Get( 0 ).areal() );
    cloud_.nCeiling_ =  static_cast< int >( cloudCeiling.value().Get( 0 ).areal() );
    cloud_.nDensityPercentage_ = std::min( std::max( static_cast< int >( cloudDensity.value().Get( 0 ).areal() ), 0 ), 100 );
    cloud_.rDensity_ = cloud_.nDensityPercentage_ / 100.;
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
    att->set_precipitation( protocol::ToProtoPrecipitation( pPrecipitation_->GetID() ));
    att->set_temperature( temperature_ );
    att->set_lighting( protocol::ToProtoLighting( pLighting_->GetID() ));
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

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

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
// Name: Meteo::IsPatched
// Created: ABR 2012-03-21
// -----------------------------------------------------------------------------
bool Meteo::IsPatched() const
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
