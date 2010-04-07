//*****************************************************************************
//
// $Created: JVT 03-08-05 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Meteo/PHY_Meteo.cpp $
// $Author: Jvt $
// $Modtime: 29/10/04 10:35 $
// $Revision: 2 $
// $Workfile: PHY_Meteo.cpp $
//
//*****************************************************************************
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
#include "clients_kernel/ModelVisitor_ABC.h"

#include <qstring.h>
#include <xeumeuleu/xml.h>

using namespace dispatcher;
//-----------------------------------------------------------------------------
// Name: PHY_Meteo constructor
// Created: JVT 03-08-05
//-----------------------------------------------------------------------------
PHY_Meteo::PHY_Meteo( unsigned int id, xml::xistream& xis, const PHY_Lighting& light, int conversionFactor )
    : pLighting_        ( &PHY_Lighting::jourSansNuage_ )
    , pPrecipitation_   ( &PHY_Precipitation::none_ )
    , nRefCount_        ( 0 )
    , conversionFactor_ ( conversionFactor )
    , id_               ( id )
{
    unsigned int nVal;
    xis >> xml::start( "cloud-cover" )
            >> xml::attribute( "floor", nPlancherCouvertureNuageuse_ )
            >> xml::attribute( "ceiling", nPlafondCouvertureNuageuse_ )
            >> xml::attribute( "density", nVal )
        >> xml::end();
    rDensiteCouvertureNuageuse_ = std::min( nVal, (unsigned int)100 ) / 100.;

    unsigned int nAngle;
    Common::MsgHeading heading;
    xis >> xml::start( "wind" )
            >> xml::attribute( "speed", wind_.rWindSpeed_ )
            >> xml::attribute( "speed", nAngle )
        >> xml::end();
    if( wind_.rWindSpeed_ < 0 )
        xis.error( "meteo: VitesseVent < 0" );
    
        wind_.rWindSpeed_ = conversionFactor_ * /*MIL_Tools::ConvertSpeedMosToSim*/( wind_.rWindSpeed_ );
    if( nAngle < 0 || nAngle > 360 )
        xis.error( "meteo: DirectionVent not in [0..360]" );
    ReadDirections::ReadDirection( heading, wind_.vWindDirection_ );

    std::string strVal;
    xis >> xml::start( "precipitation" )
            >> xml::attribute( "value", strVal )
        >> xml::end();
    pPrecipitation_ = PHY_Precipitation::FindPrecipitation( strVal );
    if( !pPrecipitation_ )
        xis.error( "Unknown Precipitation type '" + strVal + "'" );
    Update( light );
}

//-----------------------------------------------------------------------------
// Name: PHY_Meteo constructor
// Created: JVT 03-08-05
//-----------------------------------------------------------------------------
PHY_Meteo::PHY_Meteo( unsigned int id, const Common::MsgMeteoAttributes& asnMsg, MeteoManager_ABC* listener )
    : pLighting_     ( &PHY_Lighting::jourSansNuage_ )
    , pPrecipitation_( &PHY_Precipitation::none_ )
    , nRefCount_     ( 0 )
    , listener_      ( listener )
    , id_            ( id )
{
    Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: PHY_Meteo constructor
// Created: HBD 2010-04-06
// -----------------------------------------------------------------------------
PHY_Meteo::PHY_Meteo( const PHY_Lighting& light, PHY_Precipitation& precipitation )
: pLighting_     ( &light )
, pPrecipitation_( &precipitation )
, nRefCount_     ( 0 )
, listener_      ( 0 )
, id_            ( 0 )
{

}


//-----------------------------------------------------------------------------
// Name: PHY_Meteo destructor
// Created: JVT 03-08-05
//-----------------------------------------------------------------------------
PHY_Meteo::~PHY_Meteo()
{
    if( listener_ )
        listener_->UnregisterMeteo( *this );
    assert( nRefCount_ == 0 );
}

// -----------------------------------------------------------------------------
// Name: PHY_Meteo::Update
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
void PHY_Meteo::Update( const Common::MsgMeteoAttributes& msg )
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
    ReadDirections::ReadDirection( msg.wind_direction(), wind_.vWindDirection_ );

    // Précipitation
    pPrecipitation_ = PHY_Precipitation::FindPrecipitation( msg.precipitation() );
    if( !pPrecipitation_ )
        pPrecipitation_ = &PHY_Precipitation::none_;
}

//-----------------------------------------------------------------------------
// Name: PHY_Meteo::Update
// Created: JVT 03-08-07
//-----------------------------------------------------------------------------
void PHY_Meteo::Update( const PHY_Lighting& light )
{
    pLighting_ = &light.GetDegradedLighting( (unsigned int)( rDensiteCouvertureNuageuse_ * ( nPlafondCouvertureNuageuse_ - nPlancherCouvertureNuageuse_ ) / 2000 ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_Meteo::Update
// Created: HBD 2010-04-06
// -----------------------------------------------------------------------------
void PHY_Meteo::Update( const PHY_Precipitation& precipitation)
{
    pPrecipitation_ = &precipitation;
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
void PHY_Meteo::UpdateMeteoPatch( int /*date*/, PHY_RawVisionData_ABC& /*dataVision_ */)
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Meteo::SendCreation
// Created: HBD 2010-03-31
// -----------------------------------------------------------------------------
void PHY_Meteo::SendCreation( dispatcher::ClientPublisher_ABC& publisher) const
{
    client::ControlGlobalMeteo msg;
    Common::MsgMeteoAttributes* att = msg().mutable_attributes();
    msg().set_oid( id_ );
    att->set_wind_speed(  wind_.rWindSpeed_ / conversionFactor_ );
    att->mutable_wind_direction()->set_heading( 0 );
    att->set_cloud_floor( nPlancherCouvertureNuageuse_ );
    att->set_cloud_ceiling( nPlafondCouvertureNuageuse_ );
    att->set_cloud_density( rDensiteCouvertureNuageuse_ );
    att->set_precipitation( pPrecipitation_->GetAsnID() );
    att->set_temperature( 0 );
    msg.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: PHY_Meteo::SendFullUpdate
// Created: HBD 2010-03-31
// -----------------------------------------------------------------------------
void PHY_Meteo::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Meteo::Select
// Created: HBD 2010-03-31
// -----------------------------------------------------------------------------
void PHY_Meteo::Select( kernel::ActionController& controller ) const
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Meteo::ContextMenu
// Created: HBD 2010-03-31
// -----------------------------------------------------------------------------
void PHY_Meteo::ContextMenu( kernel::ActionController& controller, const QPoint& where ) const
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Meteo::Activate
// Created: HBD 2010-03-31
// -----------------------------------------------------------------------------
void PHY_Meteo::Activate( kernel::ActionController& controller ) const
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Meteo::Accept
// Created: HBD 2010-03-31
// -----------------------------------------------------------------------------
void PHY_Meteo::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
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
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Meteo::SendDestruction
// Created: HBD 2010-03-31
// -----------------------------------------------------------------------------
void PHY_Meteo::SendDestruction( dispatcher::ClientPublisher_ABC& publisher ) const
{
    //NOTHING
}
 

// -----------------------------------------------------------------------------
// Name: PHY_Meteo::IsInside
// Created: HBD 2010-04-01
// -----------------------------------------------------------------------------
bool PHY_Meteo::IsInside( const geometry::Point2f& point ) const
{
    return true;
}
