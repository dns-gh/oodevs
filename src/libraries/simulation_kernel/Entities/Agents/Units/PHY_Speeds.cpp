// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_Speeds.h"
#include "Tools/MIL_Tools.h"
#include "Entities/Agents/Actions/Moving/PHY_RoleAction_Moving.h"
#include "MT_Tools/MT_Logger.h"
#include <xeumeuleu/xml.hpp>
#include <algorithm>

// -----------------------------------------------------------------------------
// Name: PHY_Speeds constructor
// Created: AGE 2005-02-03
// -----------------------------------------------------------------------------
PHY_Speeds::PHY_Speeds( xml::xistream& xis, unsigned int timeStepDuration )
    : rMaxSpeed_                ( -1. )
    , rBaseSpeed_               ( -1. )
    , rAreaSpeeds_              ( new double[ 8  ] ) // $$$$ AGE 2005-02-03: Use a vector
    , rBorderSpeeds_            ( new double[ 8  ] )
    , rLinearSpeeds_            ( new double[ 11 ] )
    , nLinearPassabilityMask_   ( 0 )
    , nAreaPassabilityMask_     ( 0 )
    , nAreaImpassabilityMask_   ( 0 )
    , nBorderImpassabilityMask_ ( 0 )
    , nLinearImpassabilityMask_ ( 0 )
{
    std::fill( rAreaSpeeds_,   rAreaSpeeds_   + 8, -1. );
    std::fill( rBorderSpeeds_, rBorderSpeeds_ + 8, -1. );
    std::fill( rLinearSpeeds_, rLinearSpeeds_ + 11, -1. );

    xis >> xml::optional
        >> xml::list( "speeds", *this, &PHY_Speeds::ReadSpeed, timeStepDuration );

    CheckInitialization( xis, timeStepDuration );
    GenerateMasks();
}

// -----------------------------------------------------------------------------
// Name: PHY_Speeds constructor
// Created: AGE 2005-02-03
// -----------------------------------------------------------------------------
PHY_Speeds::PHY_Speeds( const moving::PHY_RoleAction_Moving& role )
    : rMaxSpeed_                ( role.GetMaxSpeedWithReinforcement() )
    , rBaseSpeed_               ( role.GetSpeedWithReinforcement( TerrainData() ) )
    , rAreaSpeeds_              ( new double[ 8  ] )
    , rBorderSpeeds_            ( new double[ 8  ] )
    , rLinearSpeeds_            ( new double[ 11 ] )
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

    for( unsigned int nOffset = 0; nOffset != 8; ++nOffset )
        rAreaSpeeds_[ nOffset ] = role.GetSpeedWithReinforcement( areas[ nOffset ] );
    for( unsigned int nOffset = 0; nOffset != 8; ++nOffset )
        rBorderSpeeds_[ nOffset ] = role.GetSpeedWithReinforcement( borders[ nOffset ] );
    for( unsigned int nOffset = 0; nOffset != 11; ++nOffset )
        rLinearSpeeds_[ nOffset ] = role.GetSpeedWithReinforcement( linears[ nOffset ] );

    GenerateMasks();
}

// -----------------------------------------------------------------------------
// Name: PHY_Speeds constructor
// Created: LMT 2010-05-04
// -----------------------------------------------------------------------------
PHY_Speeds::PHY_Speeds( const moving::PHY_RoleAction_Moving& role, bool loaded )
    : rMaxSpeed_                ( role.GetTheoricMaxSpeed( loaded ) )
    , rBaseSpeed_               ( role.GetSpeedWithReinforcement( TerrainData() ) )
    , rAreaSpeeds_              ( new double[ 8  ] )
    , rBorderSpeeds_            ( new double[ 8  ] )
    , rLinearSpeeds_            ( new double[ 11 ] )
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

    for( unsigned int nOffset = 0; nOffset != 8; ++nOffset )
        rAreaSpeeds_[ nOffset ] = role.GetSpeedWithReinforcement( areas[ nOffset ] );
    for( unsigned int nOffset = 0; nOffset != 8; ++nOffset )
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
// Name: PHY_Speeds::ReadSpeed
// Created: ABL 2007-07-23
// -----------------------------------------------------------------------------
void PHY_Speeds::ReadSpeed( xml::xistream& xis, unsigned int timeStepDuration )
{
    xis >> xml::attribute( "max", rMaxSpeed_ );
    if( rMaxSpeed_ <= 0 )
        xis.error( "speeds: max <= 0" );

    rMaxSpeed_ *= 1000. * timeStepDuration / 3600.;
    xis >> xml::list( "speed", *this, &PHY_Speeds::ReadTerrain );
}

// -----------------------------------------------------------------------------
// Name: PHY_Speeds::ReadTerrain
// Created: ABL 2007-07-23
// -----------------------------------------------------------------------------
void PHY_Speeds::ReadTerrain( xml::xistream& xis )
{
    std::string strTerrainType;

    xis >> xml::attribute( "terrain", strTerrainType );

    const TerrainData data = TerrainData::FromString( strTerrainType );
    if( data.Area() == 0xFF )
        xis.error( "Unknown terrain type '" + strTerrainType + "'" );

    double& speed = SpeedFor( data );

    xis >> xml::attribute( "value", speed );

    if( speed < 0 )
        xis.error( "speed: terrain < 0" );

    speed = MIL_Tools::ConvertSpeedMosToSim( speed );
    rMaxSpeed_ = std::max( rMaxSpeed_, speed );
}

namespace
{
    double ConvertSpeedToMOS( double speed, unsigned int timeStepDuration )
    {
        return speed * 3600. / timeStepDuration / 1000.;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_Speeds::CheckInitialization
// Created: AGE 2005-02-03
// -----------------------------------------------------------------------------
void PHY_Speeds::CheckInitialization( xml::xistream& xis, unsigned int timeStepDuration )
{
    for( unsigned nOffset = 0; nOffset < 8; ++nOffset )
    {
        double& speed = *( rAreaSpeeds_ + nOffset );
        if( speed == -1 )
        {
            speed = 0;
            MT_LOG_WARNING_MSG( "Speed for " << MIL_Tools::GetLandTypeName( TerrainData( (unsigned char)(1<<nOffset), 0, 0, 0 ) ) << " not initialized. Defaulted to 0 km/h." );
        }
    }
    for( unsigned nOffset = 0; nOffset < 8; ++nOffset )
    {
        double& speed = *( rBorderSpeeds_ + nOffset );
        if( speed == -1 )
        {
            speed = *( rAreaSpeeds_ + nOffset );
            MT_LOG_WARNING_MSG( "Speed for " << MIL_Tools::GetLandTypeName( TerrainData( 0, (unsigned char)(1<<nOffset), 0, 0 ) ) << " not initialized. Defaulted to " << ConvertSpeedToMOS( speed, timeStepDuration ) << " km/h." );
        }
    }
    for( unsigned nOffset = 0; nOffset < 10; ++nOffset )
    {
        double& speed = *( rLinearSpeeds_ + nOffset );
        if( speed == -1 )
        {
            speed = 0;
            MT_LOG_WARNING_MSG( "Speed for " << MIL_Tools::GetLandTypeName( TerrainData( 0, 0, 0, (unsigned short)(1<<nOffset) ) ) << " not initialized. Defaulted to 0 km/h." );
        }
    }
    if( *( rLinearSpeeds_ + 10 ) == -1 )
    {
        *( rLinearSpeeds_ + 10 ) = SpeedFor( TerrainData::SmallRoad() );
        MT_LOG_WARNING_MSG( "Speed for " << MIL_Tools::GetLandTypeName( TerrainData( 0, 0, 0, (unsigned short)(1<<10) ) ) << " not initialized. Defaulted to " <<  ConvertSpeedToMOS( *( rLinearSpeeds_ + 10 ), timeStepDuration ) << " km/h." );

    }
    if( rMaxSpeed_ == 0. )
        xis.error( "Composante's max speed is 0 km/h ..." );
}

// -----------------------------------------------------------------------------
// Name: PHY_Speeds::GetMaxSpeed
// Created: AGE 2005-02-03
// -----------------------------------------------------------------------------
double PHY_Speeds::GetMaxSpeed() const
{
    return rMaxSpeed_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Speeds::GetMaxSpeed
// Created: AGE 2005-02-03
// -----------------------------------------------------------------------------
double PHY_Speeds::GetMaxSpeed( const TerrainData& data ) const
{
    if( ! IsPassable( data ) )
        return 0;
    double rMax = data == TerrainData() ? rBaseSpeed_ : 0;
    {
        unsigned int nOffset = 0;
        const unsigned char area = data.Area();
        for( double* it = rAreaSpeeds_; it != rAreaSpeeds_ + 8; ++it, ++nOffset )
            if( area & (1<<nOffset) && *it > rMax )
                rMax = *it;
    }
    {
        unsigned int nOffset = 0;
        const unsigned char border = data.Border();
        for( double* it = rBorderSpeeds_; it != rBorderSpeeds_ + 8; ++it, ++nOffset )
            if( border & (1<<nOffset) && *it > rMax )
                rMax = *it;
    }
    {
        unsigned nOffset = 0;
        const unsigned short linear = data.Linear();
        for( double* it = rLinearSpeeds_; it != rLinearSpeeds_ + 11; ++it, ++nOffset )
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
    if( ( data.Linear() & nLinearPassabilityMask_ )
     || ( data.Area()   & nAreaPassabilityMask_   ) )
        return true;
    if( ( data.Linear() & nLinearImpassabilityMask_ )
     || ( data.Border() & nBorderImpassabilityMask_ ) )
        return false;
    if( ( data.Area() & nAreaImpassabilityMask_ )                   // je suis en surface impassable
      & ~( data.Border() & ~nBorderImpassabilityMask_ ) )           // et je ne suis pas sur un bord passable de meme type
        return false;
    return true;
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
        for( double* it = rAreaSpeeds_; it != rAreaSpeeds_ + 8; ++it, ++nOffset )
            if( *it == 0. )
                nAreaImpassabilityMask_ |= (1<<nOffset);
    }
    {
        nBorderImpassabilityMask_ = 0;
        unsigned int nOffset = 0;
        for( double* it = rBorderSpeeds_; it != rBorderSpeeds_ + 8; ++it, ++nOffset )
            if( *it == 0. )
                nBorderImpassabilityMask_ |= (1<<nOffset);
    }
    {
        nLinearImpassabilityMask_ = 0;
        unsigned int nOffset = 0;
        for( double* it = rLinearSpeeds_; it != rLinearSpeeds_ + 11; ++it, ++nOffset )
            if( *it == 0. )
                nLinearImpassabilityMask_ |= (1<<nOffset);
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_Speeds::SpeedFor
// Created: AGE 2005-02-03
// -----------------------------------------------------------------------------
double& PHY_Speeds::SpeedFor( const TerrainData& data )
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
    MT_LOG_INFO_MSG( "Speed dump not implemented..." ); // $$$$ AGE 2005-02-03:
}
