// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2005-11-02 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
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
    : attritions_        ( PHY_Protection::GetProtections().size(), PHY_AttritionData() )
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
    : attitudeAttritionData_( MIL_PopulationAttitude::GetAttitudes().size(), sAttritionData() )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationPionAttritionData destructor
// Created: NLD 2005-11-02
// -----------------------------------------------------------------------------
MIL_PopulationPionAttritionData::~MIL_PopulationPionAttritionData()
{

}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationPionAttritionData::Initialize
// Created: NLD 2005-11-02
// -----------------------------------------------------------------------------
void MIL_PopulationPionAttritionData::Initialize( MIL_InputArchive& archive )
{
    if( !archive.BeginList( "Attitudes", MIL_InputArchive::eNothing ) )
        return;

    while( archive.NextListElement() )
    {
        archive.Section( "Attitude" );
        
        std::string strAttitude;
        archive.ReadAttribute( "nom", strAttitude );

        const MIL_PopulationAttitude* pAttitude = MIL_PopulationAttitude::Find( strAttitude );
        if( !pAttitude )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Unknown attitude '%s'", strAttitude.c_str() ), archive.GetContext() );

        ReadAttitudeData( *pAttitude, archive );

        archive.EndSection(); // Attitude
    }
    archive.EndList();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationPionAttritionData::ReadAttitudeData
// Created: NLD 2005-11-02
// -----------------------------------------------------------------------------
void MIL_PopulationPionAttritionData::ReadAttitudeData( const MIL_PopulationAttitude& attitude, MIL_InputArchive& archive )
{
    assert( attitudeAttritionData_.size() > attitude.GetID() );

    sAttritionData& attitudeData = attitudeAttritionData_[ attitude.GetID() ];

    archive.Section( "Intensite" );
    archive.ReadAttribute( "densitePopulation", attitudeData.rPopulationDensity_, CheckValueGreaterOrEqual( 0. ) );
    archive.ReadAttribute( "intensite"        , attitudeData.rIntensity_        , CheckValueBound         ( 0., 1. ) );
    archive.EndSection(); // Intensite

    archive.BeginList( "ProtectionsPions" );
    while( archive.NextListElement() )
    {
        archive.Section( "ProtectionPion" );

        std::string strProtection;
        archive.ReadAttribute( "nom", strProtection );
        const PHY_Protection* pProtection = PHY_Protection::Find( strProtection );
        if( !pProtection )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Unknown protection '%s'", strProtection.c_str() ), archive.GetContext() );

        assert( attitudeData.attritions_.size() > pProtection->GetID() );
        attitudeData.attritions_[ pProtection->GetID() ] = PHY_AttritionData( archive );

        archive.EndSection(); // ProtectionPion            
    }

    archive.EndList(); // ProtectionsPions
}

// =============================================================================
// ACCESSORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_PopulationPionAttritionData::GetAttritionData
// Created: NLD 2005-11-03
// -----------------------------------------------------------------------------
const PHY_AttritionData& MIL_PopulationPionAttritionData::GetAttritionData( const MIL_PopulationAttitude& attitude, const PHY_Protection& protection ) const
{
    assert( attitudeAttritionData_.size() > attitude.GetID() );

    const sAttritionData& attitudeData = attitudeAttritionData_[ attitude.GetID() ];

    assert( attitudeData.attritions_.size() > protection.GetID() );
    return attitudeData.attritions_[ protection.GetID() ];
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationPionAttritionData::GetPH
// Created: NLD 2005-11-04
// -----------------------------------------------------------------------------
MT_Float MIL_PopulationPionAttritionData::GetPH( const MIL_PopulationAttitude& attitude, MT_Float rDensity ) const
{
    assert( attitudeAttritionData_.size() > attitude.GetID() );

    const sAttritionData& attitudeData = attitudeAttritionData_[ attitude.GetID() ];

    if( attitudeData.rPopulationDensity_ == 0. )
        return 0.;

    return std::min( 1., rDensity * attitudeData.rIntensity_ / attitudeData.rPopulationDensity_ );
}
