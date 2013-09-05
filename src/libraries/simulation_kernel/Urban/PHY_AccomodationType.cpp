// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_AccomodationType.h"
#include "MT_Tools/MT_Logger.h"

PHY_AccomodationType::T_AccomodationMap PHY_AccomodationType::accomodations_;

// -----------------------------------------------------------------------------
// Name: PHY_AccomodationType::Initialize
// Created: JSR 2011-02-17
// -----------------------------------------------------------------------------
void PHY_AccomodationType::Initialize( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing accomodations" );
    xis >> xml::start( "urban" )
            >> xml::optional >> xml::start( "accommodations" )
                >> xml::list( "accommodation", &PHY_AccomodationType::ReadAccomodation )
            >> xml::end
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PHY_AccomodationType::Terminate
// Created: JSR 2011-02-17
// -----------------------------------------------------------------------------
void PHY_AccomodationType::Terminate()
{
    for( CIT_AccomodationMap itAccomodation = accomodations_.begin(); itAccomodation != accomodations_.end(); ++itAccomodation )
        delete itAccomodation->second;
    accomodations_.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_AccomodationType::Find
// Created: JSR 2011-02-17
// -----------------------------------------------------------------------------
const PHY_AccomodationType* PHY_AccomodationType::Find( const std::string& strName )
{
    CIT_AccomodationMap itAccomodation = accomodations_.find( strName );
    if( itAccomodation == accomodations_.end() )
        return 0;
    return itAccomodation->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_AccomodationType::GetAccomodations
// Created: JSR 2011-02-17
// -----------------------------------------------------------------------------
const PHY_AccomodationType::T_AccomodationMap& PHY_AccomodationType::GetAccomodations()
{
    return accomodations_;
}

// -----------------------------------------------------------------------------
// Name: PHY_AccomodationType::GetNominalCapacity
// Created: JSR 2011-03-22
// -----------------------------------------------------------------------------
float PHY_AccomodationType::GetNominalCapacity() const
{
    return nominalCapacity_;
}

// -----------------------------------------------------------------------------
// Name: PHY_AccomodationType::GetMaxCapacity
// Created: JSR 2011-03-22
// -----------------------------------------------------------------------------
float PHY_AccomodationType::GetMaxCapacity() const
{
    return maxCapacity_;
}

// -----------------------------------------------------------------------------
// Name: PHY_AccomodationType constructor
// Created: JSR 2011-02-17
// -----------------------------------------------------------------------------
PHY_AccomodationType::PHY_AccomodationType( const std::string& role, float nominalCapacity, float maxCapacity )
    : role_           ( role )
    , nominalCapacity_( nominalCapacity )
    , maxCapacity_    ( maxCapacity )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_AccomodationType destructor
// Created: JSR 2011-02-17
// -----------------------------------------------------------------------------
PHY_AccomodationType::~PHY_AccomodationType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_AccomodationType::ReadAccomodation
// Created: JSR 2011-02-17
// -----------------------------------------------------------------------------
void PHY_AccomodationType::ReadAccomodation( xml::xistream& xis )
{
    std::string strRole = xis.attribute< std::string >( "role" );
    const PHY_AccomodationType*& pAccomodation = accomodations_[ strRole ];
    if( pAccomodation )
        throw MASA_EXCEPTION( xis.context() + "Accomodation " + strRole + " already defined" );
    pAccomodation = new PHY_AccomodationType( strRole, xis.attribute< float >( "nominal-capacity" ), xis.attribute< float >( "max-capacity" ) );
}
