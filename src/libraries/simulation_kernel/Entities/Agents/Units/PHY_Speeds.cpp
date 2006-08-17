// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-02-02 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/PHY_Speeds.cpp $
// $Author: Age $
// $Modtime: 29/03/05 18:23 $
// $Revision: 11 $
// $Workfile: PHY_Speeds.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_Speeds.h"
#include "Tools/MIL_Tools.h"
#include "Entities/Agents/Actions/Moving/PHY_RoleAction_Moving.h"
#include <algorithm>

// -----------------------------------------------------------------------------
// Name: PHY_Speeds constructor
// Created: AGE 2005-02-03
// -----------------------------------------------------------------------------
PHY_Speeds::PHY_Speeds( MIL_InputArchive& archive )
    : rMaxSpeed_                ( -1. )
    , rBaseSpeed_               ( -1. )
    , rAreaSpeeds_              ( new MT_Float[ 7  ] ) // $$$$ AGE 2005-02-03: Use a vector
    , rBorderSpeeds_            ( new MT_Float[ 7  ] )
    , rLinearSpeeds_            ( new MT_Float[ 11 ] )
    , nLinearPassabilityMask_   ( 0 )
    , nAreaPassabilityMask_     ( 0 )
    , nAreaImpassabilityMask_   ( 0 )
    , nBorderImpassabilityMask_ ( 0 )
    , nLinearImpassabilityMask_ ( 0 )
{
    std::fill( rAreaSpeeds_,   rAreaSpeeds_   + 7, -1. );
    std::fill( rBorderSpeeds_, rBorderSpeeds_ + 7, -1. );
    std::fill( rLinearSpeeds_, rLinearSpeeds_ + 11, -1. );

    ReadArchive( archive );
    CheckInitialization( archive );
    GenerateMasks();
}

// -----------------------------------------------------------------------------
// Name: PHY_Speeds constructor
// Created: AGE 2005-02-03
// -----------------------------------------------------------------------------
PHY_Speeds::PHY_Speeds( const PHY_RoleAction_Moving& role )
    : rMaxSpeed_                ( role.GetMaxSpeedWithReinforcement() )
    , rBaseSpeed_               ( role.GetSpeedWithReinforcement( TerrainData() ) )
    , rAreaSpeeds_              ( new MT_Float[ 7  ] )
    , rBorderSpeeds_            ( new MT_Float[ 7  ] )
    , rLinearSpeeds_            ( new MT_Float[ 11 ] )
    , nLinearPassabilityMask_   ( 0 )
    , nAreaPassabilityMask_     ( 0 )
    , nAreaImpassabilityMask_   ( 0 )
    , nBorderImpassabilityMask_ ( 0 )
    , nLinearImpassabilityMask_ ( 0 )
{
    static const TerrainData areas[]   = { 
        TerrainData::Forest(), TerrainData::Plantation(), TerrainData::Swamp(),
        TerrainData::Urban(), TerrainData::Water(), TerrainData::Dune(), TerrainData::Ice() };
    static const TerrainData borders[] = { 
        TerrainData::ForestBorder(), TerrainData::PlantationBorder(), TerrainData::SwampBorder(),
        TerrainData::UrbanBorder(), TerrainData::WaterBorder(), TerrainData::DuneBorder(), TerrainData::IceBorder() };
    static const TerrainData linears[] = { 
        TerrainData::Cliff(), TerrainData::Motorway(), TerrainData::LargeRoad(), TerrainData::MediumRoad(), TerrainData::SmallRoad(), 
        TerrainData::Bridge(), TerrainData::Railroad(), TerrainData::LargeRiver(), TerrainData::MediumRiver(), TerrainData::SmallRiver(), TerrainData::Crossroad() };

    for( unsigned int nOffset = 0; nOffset != 7; ++nOffset )
        rAreaSpeeds_[ nOffset ] = role.GetSpeedWithReinforcement( areas[ nOffset ] );
    for( unsigned int nOffset = 0; nOffset != 7; ++nOffset )
        rBorderSpeeds_[ nOffset ] = role.GetSpeedWithReinforcement( borders[ nOffset ] );
    for( unsigned int nOffset = 0; nOffset != 11; ++nOffset )
        rLinearSpeeds_[ nOffset ] = role.GetSpeedWithReinforcement( linears[ nOffset ] );

    GenerateMasks();
}

// -----------------------------------------------------------------------------
// Name: PHY_Speeds destructor
// Created: AGE 2005-02-03
// -----------------------------------------------------------------------------
PHY_Speeds::~PHY_Speeds()
{
    delete[] rAreaSpeeds_;
    delete[] rBorderSpeeds_;
    delete[] rLinearSpeeds_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Speeds::ReadArchive
// Created: AGE 2005-02-03
// -----------------------------------------------------------------------------
void PHY_Speeds::ReadArchive( MIL_InputArchive& archive )
{
    archive.BeginList( "Vitesses" );
    archive.ReadAttribute( "maximum", rMaxSpeed_, CheckValueGreater( 0. ) );
    rMaxSpeed_ = MIL_Tools::ConvertSpeedMosToSim( rMaxSpeed_ );

    while ( archive.NextListElement() )
    {
        std::string strTerrainType;

        archive.Section( "Terrain" );
        archive.ReadAttribute( "type", strTerrainType );

        const TerrainData data = MIL_Tools::ConvertLandType( strTerrainType );
        if( data.Area() == 0xFF )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown terrain type '" + strTerrainType + "'", archive.GetContext() );
        MT_Float& speed = SpeedFor( data );
        archive.Read( speed, CheckValueGreaterOrEqual( 0. ) );

        speed = MIL_Tools::ConvertSpeedMosToSim( speed );
        rMaxSpeed_ = std::max( rMaxSpeed_, speed );
        archive.EndSection(); // Terrain
    }
    archive.EndList(); // Vitesses
}

// -----------------------------------------------------------------------------
// Name: PHY_Speeds::CheckInitialization
// Created: AGE 2005-02-03
// -----------------------------------------------------------------------------
void PHY_Speeds::CheckInitialization( MIL_InputArchive& archive )
{
    for( unsigned nOffset = 0; nOffset < 7; ++nOffset )
    {
        MT_Float& speed = *( rAreaSpeeds_ + nOffset );
        if( speed == -1 )
        {
            speed = 0;
            MT_LOG_WARNING_MSG( "Speed for " << MIL_Tools::GetLandTypeName( TerrainData( (unsigned char)(1<<nOffset), 0, 0, 0 ) ) << " not initialized. Defaulted to " << MIL_Tools::ConvertSpeedSimToMos( speed ) << " km/h." );
        }
    }
    for( unsigned nOffset = 0; nOffset < 7; ++nOffset )
    {
        MT_Float& speed = *( rBorderSpeeds_ + nOffset );
        if( speed == -1 )
        {
            speed = *( rAreaSpeeds_ + nOffset );
            MT_LOG_WARNING_MSG( "Speed for " << MIL_Tools::GetLandTypeName( TerrainData( 0, (unsigned char)(1<<nOffset), 0, 0 ) ) << " not initialized. Defaulted to " << MIL_Tools::ConvertSpeedSimToMos( speed ) << " km/h." );
        }
    }
    for( unsigned nOffset = 0; nOffset < 10; ++nOffset )
    {
        MT_Float& speed = *( rLinearSpeeds_ + nOffset );
        if( speed == -1 )
        {
            speed = 0;
            MT_LOG_WARNING_MSG( "Speed for " << MIL_Tools::GetLandTypeName( TerrainData( 0, 0, 0, (unsigned short)(1<<nOffset) ) ) << " not initialized. Defaulted to 0 km/h." );
        }
    }
    if( *( rLinearSpeeds_ + 10 ) == -1 )
    {
        *( rLinearSpeeds_ + 10 ) = SpeedFor( TerrainData::SmallRoad() );
        MT_LOG_WARNING_MSG( "Speed for " << MIL_Tools::GetLandTypeName( TerrainData( 0, 0, 0, (unsigned short)(1<<10) ) ) << " not initialized. Defaulted to " <<  MIL_Tools::ConvertSpeedSimToMos( *( rLinearSpeeds_ + 10 ) ) << " km/h." );
        
    }
    if( rMaxSpeed_ == 0. )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Composante's max speed is 0 km/h ...", archive.GetContext() );
}

// -----------------------------------------------------------------------------
// Name: PHY_Speeds::GetMaxSpeed
// Created: AGE 2005-02-03
// -----------------------------------------------------------------------------
MT_Float PHY_Speeds::GetMaxSpeed() const
{
    return rMaxSpeed_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Speeds::GetMaxSpeed
// Created: AGE 2005-02-03
// -----------------------------------------------------------------------------
MT_Float PHY_Speeds::GetMaxSpeed( const TerrainData& data ) const
{
    if( ! IsPassable( data ) )
        return 0;
    MT_Float rMax = data == TerrainData() ? rBaseSpeed_ : 0;
    {
        unsigned int nOffset = 0;
        const unsigned char area = data.Area();
        for( MT_Float* it = rAreaSpeeds_; it != rAreaSpeeds_ + 7; ++it, ++nOffset )
            if( area & (1<<nOffset) && *it > rMax )
                rMax = *it;
    }
    {
        unsigned int nOffset = 0;
        const unsigned char border = data.Border();
        for( MT_Float* it = rBorderSpeeds_; it != rBorderSpeeds_ + 7; ++it, ++nOffset )
            if( border & (1<<nOffset) && *it > rMax )
                rMax = *it;
    }
    {
        unsigned nOffset = 0;
        const unsigned short linear = data.Linear();
        for( MT_Float* it = rLinearSpeeds_; it != rLinearSpeeds_ + 11; ++it, ++nOffset )
            if( linear & (1<<nOffset) && *it > rMax )
                rMax = *it;
    }
    return rMax;
}

// -----------------------------------------------------------------------------
// Name: PHY_Speeds::IsPassable
// Created: AGE 2005-02-03
// -----------------------------------------------------------------------------
bool PHY_Speeds::IsPassable( const TerrainData& data ) const
{
    return  ( ( data.Linear() & nLinearPassabilityMask_ )
           || ( data.Area()   & nAreaPassabilityMask_ ) )
         || ( !( data.Area()   & nAreaImpassabilityMask_ )
           && !( data.Border() & nBorderImpassabilityMask_ )
           && !( data.Linear() & nLinearImpassabilityMask_ ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_Speeds::GenerateMasks
// Created: AGE 2005-02-03
// -----------------------------------------------------------------------------
void PHY_Speeds::GenerateMasks()
{
    {
        // If you can go on a road, you can go on whatever there is under the road
        nLinearPassabilityMask_ = 0;
        unsigned int nOffset = TerrainData::Motorway().ExtractLinearOffset();
        if( rLinearSpeeds_[ nOffset ] > 0 )
            nLinearPassabilityMask_ |= (1 << nOffset);
        nOffset = TerrainData::LargeRoad().ExtractLinearOffset();
        if( rLinearSpeeds_[ nOffset ] > 0 )
            nLinearPassabilityMask_ |= (1 << nOffset);
        nOffset = TerrainData::MediumRoad().ExtractLinearOffset();
        if( rLinearSpeeds_[ nOffset ] > 0 )
            nLinearPassabilityMask_ |= (1 << nOffset);
        nOffset = TerrainData::SmallRoad().ExtractLinearOffset();
        if( rLinearSpeeds_[ nOffset ] > 0 )
            nLinearPassabilityMask_ |= (1 << nOffset);
        nOffset = TerrainData::Bridge().ExtractLinearOffset();
        if( rLinearSpeeds_[ nOffset ] > 0 )
            nLinearPassabilityMask_ |= (1 << nOffset);
    }
    {
        // If you can go on water, you're not stopped by anything that could cross the water
        nAreaPassabilityMask_ = 0;
        const unsigned int nOffset = TerrainData::Water().ExtractAreaOffset();
        if( rAreaSpeeds_[ nOffset ] > 0 )
            nAreaPassabilityMask_ |= (1 << nOffset);
    }
    {
        nAreaImpassabilityMask_ = 0;
        unsigned int nOffset = 0;
        for( MT_Float* it = rAreaSpeeds_; it != rAreaSpeeds_ + 7; ++it, ++nOffset )
            if( *it == 0. )
                nAreaImpassabilityMask_ |= (1<<nOffset);
    }
    {
        nBorderImpassabilityMask_ = 0;
        unsigned int nOffset = 0;
        for( MT_Float* it = rBorderSpeeds_; it != rBorderSpeeds_ + 7; ++it, ++nOffset )
            if( *it == 0. )
                nBorderImpassabilityMask_ |= (1<<nOffset);
    }
    {
        nLinearImpassabilityMask_ = 0;
        unsigned int nOffset = 0;
        for( MT_Float* it = rLinearSpeeds_; it != rLinearSpeeds_ + 11; ++it, ++nOffset )
            if( *it == 0. )
                nLinearImpassabilityMask_ |= (1<<nOffset);
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_Speeds::SpeedFor
// Created: AGE 2005-02-03
// -----------------------------------------------------------------------------
MT_Float& PHY_Speeds::SpeedFor( const TerrainData& data )
{
    if( data.Area() )
        return *( rAreaSpeeds_ + data.ExtractAreaOffset() );
    if( data.Border() )
        return *( rBorderSpeeds_ + data.ExtractBorderOffset() );
    if( data.Linear() )
        return *( rLinearSpeeds_ + data.ExtractLinearOffset() );
    return rBaseSpeed_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Speeds::DumpSpeeds
// Created: AGE 2005-02-03
// -----------------------------------------------------------------------------
void PHY_Speeds::DumpSpeeds( const TerrainData& ) const
{
   MT_LOG_INFO_MSG( "Hahaha do it yourself !" ); // $$$$ AGE 2005-02-03: 
}
