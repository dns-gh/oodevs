// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "MeteoLocal.h"
#include "PHY_Precipitation.h"
#include "PHY_Lighting.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Tools.h"
#include "protocol/ClientSenders.h"
#include "protocol/ClientPublisher_ABC.h"
#include <xeumeuleu/xml.hpp>

using namespace weather;

unsigned int MeteoLocal::localCounter_ = 2;

namespace
{
    QDateTime MakeDate( const std::string& str )
    {
        QString extended( str.c_str() );
        extended.insert( 13, ':' ); extended.insert( 11, ':' );
        extended.insert(  6, '-' ); extended.insert(  4, '-' );
        return QDateTime::fromString( extended, Qt::ISODate );
    }

    std::string MakeGDHString( const QDateTime& datetime )
    {
        // $$$$ SBO 2008-04-25: ...
        QString str = datetime.toString( Qt::ISODate );
        str.remove( ':' );
        str.remove( '-' );
        return str.toAscii().constData();
    }
}

// -----------------------------------------------------------------------------
// Name: MeteoLocal constructor
// Created: ABR 2011-06-01
// -----------------------------------------------------------------------------
MeteoLocal::MeteoLocal( const MeteoLocal& meteo )
    : Meteo( meteo.GetId(), &meteo.GetLighting(), meteo.GetPrecipitation(), 0, meteo.GetTemperature(), meteo.GetName() )
    , converter_( &meteo.GetCoordinateConverter() )
    , created_( false )
{
    conversionFactor_ = meteo.GetConversionFactor();
    wind_ = meteo.GetWind();
    cloud_ = meteo.GetCloud();
    temperature_ = meteo.GetTemperature();
    topLeft_ = meteo.GetTopLeft();
    bottomRight_ = meteo.GetBottomRight();
    startTime_ = meteo.GetStartTime();
    endTime_ = meteo.GetEndTime();
}

// -----------------------------------------------------------------------------
// Name: MeteoLocal constructor
// Created: SBO 2006-12-20
// -----------------------------------------------------------------------------
MeteoLocal::MeteoLocal( const kernel::CoordinateConverter_ABC& converter, const std::string& name /*= ""*/ )
    : Meteo( localCounter_, 0, name + QString::number( localCounter_ - 1).toAscii().constData() )
    , converter_( &converter )
    , created_( false )
{
    localCounter_++;
}

// -----------------------------------------------------------------------------
// Name: MeteoLocal constructor
// Created: SBO 2006-12-20
// -----------------------------------------------------------------------------
MeteoLocal::MeteoLocal( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter, const std::string& name /*= ""*/ )
    : Meteo( localCounter_, xis, 0, 0, name + QString::number( localCounter_ - 1 ).toAscii().constData() )
    , converter_( &converter )
    , created_  ( false )
{
    std::string topLeft, bottomRight, startTime, endTime, lighting ;
    xis >> xml::attribute( "start-time", startTime )
        >> xml::attribute( "end-time", endTime )
        >> xml::attribute( "top-left", topLeft )
        >> xml::attribute( "bottom-right", bottomRight )
        >> xml::optional >> xml::attribute( "lighting", lighting ); // $$$$ ABR 2012-10-19: Remove optional on the next version
    const weather::PHY_Lighting* pLighting = weather::PHY_Lighting::FindLighting( lighting );
    if( pLighting )
        pLighting_ = pLighting;
    else
        localLighting_ = false;
    topLeft_ = converter_->ConvertToXY( topLeft );
    bottomRight_ = converter_->ConvertToXY( bottomRight );
    startTime_ = MakeDate( startTime );
    endTime_ = MakeDate( endTime );
    localCounter_++;
}

// -----------------------------------------------------------------------------
// Name: MeteoLocal constructor
// Created: ABR 2011-06-06
// -----------------------------------------------------------------------------
MeteoLocal::MeteoLocal( const sword::ControlLocalWeatherCreation& msg, const kernel::CoordinateConverter_ABC& converter, unsigned int timeStep, const std::string& name /*= ""*/ )
    : Meteo( msg.weather().id(), msg.attributes(), timeStep, name + QString::number( msg.weather().id() - 1 ).toAscii().constData() )
    , converter_  ( &converter )
    , topLeft_    ( converter_->ConvertFromGeo( geometry::Point2d( msg.top_left().longitude(), msg.top_left().latitude() ) ) )
    , bottomRight_( converter_->ConvertFromGeo( geometry::Point2d( msg.bottom_right().longitude(), msg.bottom_right().latitude() ) ) )
    , startTime_  ( MakeDate( msg.start_date().data() ) )
    , endTime_    ( MakeDate( msg.end_date().data() ) )
    , created_    ( false )
{
    if( msg.weather().id() >= localCounter_ )
        localCounter_ = msg.weather().id() + 1;
}

// -----------------------------------------------------------------------------
// Name: MeteoLocal constructor used by dispatcher only
// Created: ABR 2011-06-08
// -----------------------------------------------------------------------------
MeteoLocal::MeteoLocal( const sword::ControlLocalWeatherCreation& msg, unsigned int timeStep, const std::string& name /*= ""*/  )
    : Meteo( msg.weather().id(), msg.attributes(), timeStep, name )
    , converter_  ( 0 )
    , topLeft_    ( static_cast< float >( msg.top_left().longitude() ), static_cast< float >( msg.top_left().latitude() ) ) // $$$$ ABR 2011-06-08: Warning, no coordinate converter for dispatcher
    , bottomRight_( static_cast< float >( msg.bottom_right().longitude() ), static_cast< float >( msg.bottom_right().latitude() ) )
    , startTime_  ( MakeDate( msg.start_date().data() ) )
    , endTime_    ( MakeDate( msg.end_date().data() ) )
    , created_    ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MeteoLocal destructor
// Created: SBO 2006-12-20
// -----------------------------------------------------------------------------
MeteoLocal::~MeteoLocal()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MeteoLocal::Serialize
// Created: SBO 2006-12-20
// -----------------------------------------------------------------------------
void MeteoLocal::Serialize( xml::xostream& xos ) const
{
    const QString start = ( startTime_.isValid() ? startTime_ : QDateTime() ).toString( "yyyyMMddThhmmss" );
    const QString end   = ( endTime_.isValid() ? endTime_ : QDateTime() ).toString( "yyyyMMddThhmmss" );
    xos << xml::attribute( "start-time", start.isNull() ? "19700101Thhmmss" : start.toAscii().constData() )
        << xml::attribute( "end-time", end.isNull() ? "19700101Thhmmss" : end.toAscii().constData() )
        << xml::attribute( "top-left", converter_->ConvertToMgrs( topLeft_ ) )
        << xml::attribute( "bottom-right", converter_->ConvertToMgrs( bottomRight_ ) );
    if( localLighting_ && pLighting_ )
        xos << xml::attribute( "lighting", pLighting_->GetName() );
    Meteo::Serialize( xos );
}

// -----------------------------------------------------------------------------
// Name: MeteoLocal::SetPosition
// Created: SBO 2006-12-21
// -----------------------------------------------------------------------------
void MeteoLocal::SetPosition( const geometry::Point2f& topLeft, const geometry::Point2f& bottomRight )
{
    topLeft_ = topLeft;
    bottomRight_ = bottomRight;
}

// -----------------------------------------------------------------------------
// Name: MeteoLocal::SetPeriod
// Created: SBO 2010-05-17
// -----------------------------------------------------------------------------
void MeteoLocal::SetPeriod( const QDateTime& start, const QDateTime& end )
{
    if( startTime_ != start || endTime_ != end )
        modified_ = true;
    startTime_ = start;
    endTime_ = end;
}

// -----------------------------------------------------------------------------
// Name: MeteoLocal::SetCreated
// Created: ABR 2011-06-07
// -----------------------------------------------------------------------------
void MeteoLocal::SetCreated( bool created )
{
    created_ = created;
}

// -----------------------------------------------------------------------------
// Name: MeteoLocal::GetTopLeft
// Created: SBO 2006-12-21
// -----------------------------------------------------------------------------
const geometry::Point2f MeteoLocal::GetTopLeft() const
{
    return topLeft_;
}

// -----------------------------------------------------------------------------
// Name: MeteoLocal::GetBottomRight
// Created: SBO 2006-12-21
// -----------------------------------------------------------------------------
const geometry::Point2f MeteoLocal::GetBottomRight() const
{
    return bottomRight_;
}

// -----------------------------------------------------------------------------
// Name: MeteoLocal::GetStartTime
// Created: SBO 2010-05-17
// -----------------------------------------------------------------------------
const QDateTime MeteoLocal::GetStartTime() const
{
    return startTime_;
}

// -----------------------------------------------------------------------------
// Name: MeteoLocal::GetEndTime
// Created: SBO 2010-05-17
// -----------------------------------------------------------------------------
const QDateTime MeteoLocal::GetEndTime() const
{
    return endTime_;
}

// -----------------------------------------------------------------------------
// Name: MeteoLocal::IsCreated
// Created: ABR 2011-06-07
// -----------------------------------------------------------------------------
bool MeteoLocal::IsCreated() const
{
    return created_;
}

// -----------------------------------------------------------------------------
// Name: MeteoLocal::GetCoordinateConverter
// Created: ABR 2011-06-01
// -----------------------------------------------------------------------------
const kernel::CoordinateConverter_ABC& MeteoLocal::GetCoordinateConverter() const
{
    return *converter_;
}

// -----------------------------------------------------------------------------
// Name: MeteoLocal::IsInside
// Created: ABR 2011-06-06
// -----------------------------------------------------------------------------
bool MeteoLocal::IsInside( const geometry::Point2f& point ) const
{
    const geometry::Rectangle2f rect( geometry::Point2f( topLeft_.X(), bottomRight_.Y() ), geometry::Point2f( bottomRight_.X(), topLeft_.Y() ) );
    return rect.IsInside( point );
}

// -----------------------------------------------------------------------------
// Name: MeteoLocal::IsValid
// Created: ABR 2011-06-07
// -----------------------------------------------------------------------------
bool MeteoLocal::IsValid() const
{
    return !created_ || startTime_ < endTime_ && startTime_.isValid() && endTime_.isValid() && !( topLeft_ == geometry::Point2f( 0, 0 ) && bottomRight_ == geometry::Point2f( 0, 0 ) );
}

// -----------------------------------------------------------------------------
// Name: MeteoLocal::Update
// Created: ABR 2011-06-08
// -----------------------------------------------------------------------------
void MeteoLocal::Update( const sword::ControlLocalWeatherCreation& msg )
{
    Meteo::Update( msg.attributes() );
    assert( converter_ );
    topLeft_     = converter_->ConvertFromGeo( geometry::Point2d( msg.top_left().longitude(), msg.top_left().latitude() ) );
    bottomRight_ = converter_->ConvertFromGeo( geometry::Point2d( msg.bottom_right().longitude(), msg.bottom_right().latitude() ) );
    startTime_   = MakeDate( msg.start_date().data() );
    endTime_     = MakeDate( msg.end_date().data() );
}

// -----------------------------------------------------------------------------
// Name: MeteoLocal::SendCreation used by dispatcher only
// Created: ABR 2011-06-08
// -----------------------------------------------------------------------------
void MeteoLocal::SendCreation( dispatcher::ClientPublisher_ABC& publisher ) const
{
    client::ControlLocalWeatherCreation msg;
    sword::WeatherAttributes* att = msg().mutable_attributes();
    msg().mutable_weather()->set_id( id_ );
    att->set_wind_speed( static_cast< int >( wind_.rSpeed_ / conversionFactor_ ) );
    att->mutable_wind_direction()->set_heading( wind_.eAngle_ );
    att->set_cloud_floor ( cloud_.nFloor_ );
    att->set_cloud_ceiling( cloud_.nCeiling_ );
    att->set_cloud_density( static_cast< int >( cloud_.rDensity_ ) );
    att->set_precipitation( pPrecipitation_->GetAsnID() );
    att->set_temperature( temperature_ );
    att->set_lighting( pLighting_->GetAsnID() );
    msg().mutable_top_left()->set_longitude( topLeft_.X() );
    msg().mutable_top_left()->set_latitude( topLeft_.Y() );
    msg().mutable_bottom_right()->set_longitude( bottomRight_.X() );
    msg().mutable_bottom_right()->set_latitude( bottomRight_.Y() );
    msg().mutable_start_date()->set_data( MakeGDHString( startTime_ ) );
    msg().mutable_end_date()->set_data( MakeGDHString( endTime_ ) );
    msg.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: MeteoLocal::SendDestruction used by dispatcher only
// Created: ABR 2011-06-08
// -----------------------------------------------------------------------------
void MeteoLocal::SendDestruction( dispatcher::ClientPublisher_ABC& publisher ) const
{
    client::ControlLocalWeatherDestruction msg;
    msg().mutable_weather()->set_id( id_ );
    msg.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Meteo::IsYounger
// Created: ABR 2012-03-23
// -----------------------------------------------------------------------------
bool MeteoLocal::IsOlder( const weather::Meteo& other ) const
{
    if( const MeteoLocal* localOther = dynamic_cast< const MeteoLocal* >( &other ) )
    {
        return startTime_ < localOther->GetStartTime();
    }
    else
        return true;
}
