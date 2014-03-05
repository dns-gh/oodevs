// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_PopulationPionAttritionData.h"
#include "Entities/Agents/Units/Categories/PHY_Protection.h"
#include "Entities/Populations/MIL_PopulationAttitude.h"

// -----------------------------------------------------------------------------
// Name: MIL_PopulationPionAttritionData::sAttritionData
// Created: NLD 2005-11-02
// -----------------------------------------------------------------------------
MIL_PopulationPionAttritionData::sAttritionData::sAttritionData()
    : armedAttritions_   ( PHY_Protection::GetProtections().size(), PHY_AttritionData() )
    , unarmedAttritions_ ( PHY_Protection::GetProtections().size(), PHY_AttritionData() )
    , rPopulationDensity_( 0. )
    , rIntensity_        ( 0. )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationPionAttritionData constructor
// Created: NLD 2005-11-02
// -----------------------------------------------------------------------------
MIL_PopulationPionAttritionData::MIL_PopulationPionAttritionData()
    : attitudeAttritionData_( MIL_PopulationAttitude::Size(), sAttritionData() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationPionAttritionData destructor
// Created: NLD 2005-11-02
// -----------------------------------------------------------------------------
MIL_PopulationPionAttritionData::~MIL_PopulationPionAttritionData()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationPionAttritionData::Initialize
// Created: NLD 2005-11-02
// -----------------------------------------------------------------------------
void MIL_PopulationPionAttritionData::Initialize( xml::xistream& xis )
{
    xis >> xml::list( "attrition-effect", *this, &MIL_PopulationPionAttritionData::ReadAttritionEffect );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationPionAttritionData::ReadAttritionEffect
// Created: ABL 2007-07-24
// -----------------------------------------------------------------------------
void MIL_PopulationPionAttritionData::ReadAttritionEffect( xml::xistream& xis )
{
    std::string strAttitude = xis.attribute< std::string >( "population-attitude" );
    const MIL_PopulationAttitude* pAttitude = MIL_PopulationAttitude::Find( strAttitude );
    if( !pAttitude )
        throw MASA_EXCEPTION( xis.context() + "Unknown attitude '" + strAttitude + "'" );
    ReadAttitudeData( *pAttitude, xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationPionAttritionData::ReadAttitudeData
// Created: NLD 2005-11-02
// -----------------------------------------------------------------------------
void MIL_PopulationPionAttritionData::ReadAttitudeData( const MIL_PopulationAttitude& attitude, xml::xistream& xis )
{
    assert( attitudeAttritionData_.size() > attitude.GetID() );
    sAttritionData& attitudeData = attitudeAttritionData_[ attitude.GetID() ];
    xis >> xml::attribute( "population-density", attitudeData.rPopulationDensity_ )
        >> xml::attribute( "intensity", attitudeData.rIntensity_ );
    if( attitudeData.rPopulationDensity_ < 0 )
        throw MASA_EXCEPTION( xis.context() + "attrition-effect: population-density < 0" );
    if( attitudeData.rIntensity_ < 0 || attitudeData.rIntensity_ > 1 )
        throw MASA_EXCEPTION( xis.context() + "attrition-effect: intensity not in [0..1]" );
    xis >> xml::list( "protection", *this, &MIL_PopulationPionAttritionData::ReadAttritionUnitEffect, attitudeData );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationPionAttritionData::ReadAttritionUnitEffect
// Created: ABL 2007-07-24
// -----------------------------------------------------------------------------
void MIL_PopulationPionAttritionData::ReadAttritionUnitEffect( xml::xistream& xis, sAttritionData& attitudeData )
{
    std::string strProtection = xis.attribute< std::string >( "name" );

    const PHY_Protection* pProtection = PHY_Protection::Find( strProtection );
    if( !pProtection )
        throw MASA_EXCEPTION( xis.context() + "Unknown protection '" + strProtection + "'" );

    xis >> xml::start( "armed" );
    assert( attitudeData.armedAttritions_.size() > pProtection->GetID() );
    attitudeData.armedAttritions_[ pProtection->GetID() ] = PHY_AttritionData( xis );
    xis >> xml::end;
    xis >> xml::start( "unarmed" );
    assert( attitudeData.unarmedAttritions_.size() > pProtection->GetID() );
    attitudeData.unarmedAttritions_[ pProtection->GetID() ] = PHY_AttritionData( xis );
    xis >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationPionAttritionData::GetAttritionData
// Created: NLD 2005-11-03
// -----------------------------------------------------------------------------
const PHY_AttritionData MIL_PopulationPionAttritionData::GetAttritionData( const MIL_PopulationAttitude& attitude, const PHY_Protection& protection, double armedIndividuals ) const
{
    assert( attitudeAttritionData_.size() > attitude.GetID() );
    const sAttritionData& attitudeData = attitudeAttritionData_[ attitude.GetID() ];
    assert( attitudeData.armedAttritions_.size() > protection.GetID() );
    assert( attitudeData.unarmedAttritions_.size() > protection.GetID() );
    //return attitudeData.attritions_[ protection.GetID() ];

    return PHY_AttritionData( attitudeData.armedAttritions_[ protection.GetID() ], attitudeData.unarmedAttritions_[ protection.GetID() ], armedIndividuals );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationPionAttritionData::GetPH
// Created: NLD 2005-11-04
// -----------------------------------------------------------------------------
double MIL_PopulationPionAttritionData::GetPH( const MIL_PopulationAttitude& attitude, double rDensity ) const
{
    assert( attitudeAttritionData_.size() > attitude.GetID() );
    const sAttritionData& attitudeData = attitudeAttritionData_[ attitude.GetID() ];
    if( attitudeData.rPopulationDensity_ == 0. )
        return 0.;
    return std::min( 1., rDensity * attitudeData.rIntensity_ / attitudeData.rPopulationDensity_ );
}
