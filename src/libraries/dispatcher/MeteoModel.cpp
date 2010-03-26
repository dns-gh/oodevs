// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MeteoModel.h"
#include "clients_kernel/CoordinateConverter.h"
#include "meteo/PHY_Meteo.h"
#include "meteo/PHY_Precipitation.h"
namespace dispatcher
{

// -----------------------------------------------------------------------------
// Name: MeteoModel constructor 
// Created: HBD 2010-03-23
// -----------------------------------------------------------------------------
MeteoModel::MeteoModel( kernel::CoordinateConverter_ABC& conv )  
  :  MeteoModel_ABC()
  ,  converter_ ( conv )
  , pGlobalMeteo_()
{
    PHY_Precipitation::Initialize();
    PHY_Lighting     ::Initialize();
}

// -----------------------------------------------------------------------------
// Name: MeteoModel destructor
// Created: HBD 2010-03-23
// -----------------------------------------------------------------------------
MeteoModel::~MeteoModel()
{
    PHY_Precipitation::Terminate();
    PHY_Lighting     ::Terminate();
}

// ----------------------------------------------------                                                                                                                                  -------------------------
// Name: MeteoModel::GetLighting
// Created: HBD 2010-03-23
// -----------------------------------------------------------------------------
const PHY_Lighting& MeteoModel::GetLighting() const
{
   return pGlobalMeteo_->GetLighting();
}

// -----------------------------------------------------------------------------
// Name: MeteoModel::OnReceiveMsgGlobalMeteo
// Created: HBD 2010-03-23
// -----------------------------------------------------------------------------
void MeteoModel::OnReceiveMsgGlobalMeteo( const MsgsSimToClient::MsgControlGlobalMeteo& msg )
{
    if ( pGlobalMeteo_ )
        pGlobalMeteo_->Update( msg.attributes() );
    else
        pGlobalMeteo_ = new PHY_Meteo( msg.attributes(), this );
}

// -----------------------------------------------------------------------------
// Name: MeteoModel::OnReceiveMsgLocalMeteo
// Created: HBD 2010-03-23
// -----------------------------------------------------------------------------
void MeteoModel::OnReceiveMsgLocalMeteo( const MsgsSimToClient::MsgControlLocalMeteo& msg )
{
    geometry::Point2d vUpLeft = converter_.ConvertToGeo( geometry::Point2f( msg.top_left_coordinate().longitude(), 
        msg.top_left_coordinate().latitude() ));
    geometry::Point2d vDownRight = converter_.ConvertToGeo( geometry::Point2f( msg.bottom_right_coordinate().longitude(), 
        msg.bottom_right_coordinate().latitude() ));

    PHY_Meteo* pTmp = 0;
    if( msg.has_attributes() )
    {
        pTmp = new PHY_Meteo( msg.attributes(), this );
        RegisterMeteo( *pTmp );
    }
}


// -----------------------------------------------------------------------------
// Name: MeteoModel::RegisterMeteo
// Created: HBD 2010-03-23
// -----------------------------------------------------------------------------
void MeteoModel::RegisterMeteo( PHY_Meteo& meteo )
{
   meteos_.push_front( &meteo ); 
}
// -----------------------------------------------------------------------------
// Name: MeteoModel::UnregisterMeteo
// Created: HBD 2010-03-23
// -----------------------------------------------------------------------------
void MeteoModel::UnregisterMeteo( PHY_Meteo& meteo )
{
 meteos_.remove( &meteo ); 
}

}
