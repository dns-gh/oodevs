// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_LocalMeteo.h"
#include "meteo/PHY_Meteo.h"
#include "meteo/PHY_Lighting.h"
#include "meteo/MeteoManager_ABC.h"
#include "Network/NET_ASN_Tools.h"
#include "Tools/MIL_Tools.h"
#include "meteo/PHY_RawVisionData_ABC.h"
#include "protocol/SimulationSenders.h"
#include "protocol/Protocol.h"
#include <xeumeuleu/xml.h>
#pragma warning( push, 1 )
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma warning( pop )

namespace bpt = boost::posix_time;

// -----------------------------------------------------------------------------
// Name: PHY_LocalMeteo constructor
// Created: SLG 2010-03-18
// -----------------------------------------------------------------------------
PHY_LocalMeteo::PHY_LocalMeteo( xml::xistream& xis, const PHY_Lighting& light, int conversionFactor )
    : PHY_Meteo( xis, light, conversionFactor )
    , bIsPatched_( false )
{
    std::string strStartTime, strEndTime, strTopLeftPos, strTopRightPos;
    xis >> xml::attribute( "start-time", strStartTime )
        >> xml::attribute( "end-time", strEndTime )
        >> xml::attribute( "top-left", strTopLeftPos )
        >> xml::attribute( "bottom-right", strTopRightPos );
    MIL_Tools::ConvertCoordMosToSim( strTopLeftPos, upLeft_ );
    MIL_Tools::ConvertCoordMosToSim( strTopRightPos, downRight_ );
    startTime_ = ( bpt::from_iso_string( strStartTime ) - bpt::from_time_t( 0 ) ).total_seconds();
    endTime_ = ( bpt::from_iso_string( strEndTime ) - bpt::from_time_t( 0 ) ).total_seconds();
}

//-----------------------------------------------------------------------------
// Name: PHY_LocalMeteo constructor
// Created: JVT 03-08-05
//-----------------------------------------------------------------------------
PHY_LocalMeteo::PHY_LocalMeteo( const Common::MsgMeteoAttributes& asnMsg, MT_Vector2D upLeft, MT_Vector2D downRight  )
    : PHY_Meteo( asnMsg )
    , upLeft_( upLeft )
    , downRight_( downRight )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_LocalMeteo destructor
// Created: SLG 2010-03-18
// -----------------------------------------------------------------------------
PHY_LocalMeteo::~PHY_LocalMeteo()
{
    //NTOHING
}


// -----------------------------------------------------------------------------
// Name: PHY_LocalMeteo::UpdateMeteoPatch
// Created: SLG 2010-03-18
// -----------------------------------------------------------------------------
void PHY_LocalMeteo::UpdateMeteoPatch( int date, PHY_RawVisionData_ABC& dataVision_ )
{
    bool bNeedToBePatched = ( date > startTime_ &&  date < endTime_ );
    if( !bIsPatched_ && bNeedToBePatched )
    {
        dataVision_.RegisterMeteoPatch( geometry::Point2d( upLeft_.rX_, upLeft_.rY_) , geometry::Point2d( downRight_.rX_, downRight_.rY_), this );
        bIsPatched_ = true;
    }
    else if( bIsPatched_ && !bNeedToBePatched )
    {
        dataVision_.UnregisterMeteoPatch( geometry::Point2d( upLeft_.rX_, upLeft_.rY_) , geometry::Point2d( downRight_.rX_, downRight_.rY_), this );
        bIsPatched_ = false;
    }
}

