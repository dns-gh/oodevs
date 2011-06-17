// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************


#include "simulation_kernel_pch.h"
#include "PHY_HumanFactor.h"
#include "protocol/Protocol.h"
#include "MT_Tools/MT_Logger.h"
#include "MT_Tools/MT_Stl.h"
#include <xeumeuleu/xml.hpp>


// -----------------------------------------------------------------------------
// Name: PHY_HumanFactor constructor
// Created: LDC 2011-06-16
// -----------------------------------------------------------------------------
PHY_HumanFactor::PHY_HumanFactor( const std::string& strName, unsigned int nType )
    : strName_                       ( strName )
    , nType_                         ( nType   )
    , rCoefMaxSpeedModificator_      ( 1. )
    , rCoefReloadingTimeModificator_ ( 1. )
    , rCoefPhModificator_            ( 1. )
    , rCoefPostureTimeModificator_   ( 1. )
    , rCoefSensorDistanceModificator_( 1. )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanFactor::Read
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
void PHY_HumanFactor::Read( xml::xistream& xis )
{
    xis >> xml::attribute( "max-speed", rCoefMaxSpeedModificator_ )
        >> xml::attribute( "loading-time", rCoefReloadingTimeModificator_ )
        >> xml::attribute( "ph", rCoefPhModificator_ )
        >> xml::attribute( "posture-setup-time", rCoefPostureTimeModificator_ )
        >> xml::attribute( "sensor-distance", rCoefSensorDistanceModificator_ );

    if( rCoefMaxSpeedModificator_ <= 0 )
        xis.error( "max-speed <= 0" );
    if( rCoefReloadingTimeModificator_ <= 0 )
        xis.error( "loading-time <= 0" );
    if( rCoefPhModificator_ <= 0 )
        xis.error( "ph <= 0" );
    if( rCoefPostureTimeModificator_ <= 0 )
        xis.error( "posture-setup-time <= 0" );
    if( rCoefSensorDistanceModificator_ <= 0 )
        xis.error( "sensor-distance <= 0" );
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanFactor::GetName
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
const std::string& PHY_HumanFactor::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanFactor::GetCoefMaxSpeedModificator
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
double PHY_HumanFactor::GetCoefMaxSpeedModificator() const
{
    return rCoefMaxSpeedModificator_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanFactor::GetCoefReloadingTimeModificator
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
double PHY_HumanFactor::GetCoefReloadingTimeModificator() const
{
    return rCoefReloadingTimeModificator_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanFactor::GetCoefPhModificator
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
double PHY_HumanFactor::GetCoefPhModificator() const
{
    return rCoefPhModificator_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanFactor::GetCoefPostureTimeModificator
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
double PHY_HumanFactor::GetCoefPostureTimeModificator() const
{
    return rCoefPostureTimeModificator_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanFactor::GetCoefSensorDistanceModificator
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
double PHY_HumanFactor::GetCoefSensorDistanceModificator() const
{
    return rCoefSensorDistanceModificator_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanFactor::operator==
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
bool PHY_HumanFactor::operator==( const PHY_HumanFactor& rhs ) const
{
    return nType_ == rhs.nType_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanFactor::operator!=
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
bool PHY_HumanFactor::operator!=( const PHY_HumanFactor& rhs ) const
{
    return nType_ != rhs.nType_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanFactor::GetID
// Created: JVT 2004-11-30
// -----------------------------------------------------------------------------
unsigned int PHY_HumanFactor::GetID() const
{
    return (unsigned int)nType_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanFactor::GetWeight
// Created: NLD 2004-12-01
// -----------------------------------------------------------------------------
double PHY_HumanFactor::GetWeight() const
{
    return 1.;
}
