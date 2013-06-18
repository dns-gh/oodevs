// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Speeds.h"
#include "wrapper/View.h"
#include "wrapper/Hook.h"
#include <spatialcontainer/TerrainData.h>
#include <module_api/Log.h>
#include <xeumeuleu/xml.hpp>
#include <algorithm>

using namespace sword;
using namespace sword::movement;

DECLARE_HOOK( ConvertSpeedMosToSim, double, ( double speed ) )
DECLARE_HOOK( GetLandTypeName, const char*, ( const TerrainData& terrain ) )
DECLARE_HOOK( GetTheoricMaxSpeedWithReinforcement, double, ( const SWORD_Model* entity ) )
DECLARE_HOOK( GetSpeedWithReinforcement, double, ( const SWORD_Model* entity, const TerrainData& environment ) )

// -----------------------------------------------------------------------------
// Name: Speeds constructor
// Created: AGE 2005-02-03
// -----------------------------------------------------------------------------
Speeds::Speeds( xml::xistream& xis, unsigned int timeStepDuration )
    : rMaxSpeed_               ( -1. )
    , rBaseSpeed_              ( -1. )
    , nLinearPassabilityMask_  ( 0 )
    , nAreaPassabilityMask_    ( 0 )
    , nAreaImpassabilityMask_  ( 0 )
    , nBorderImpassabilityMask_( 0 )
    , nLinearImpassabilityMask_( 0 )
{
    rAreaSpeeds_.resize( TerrainData::nAreaTypes, -1. );
    rBorderSpeeds_.resize( TerrainData::nBorderTypes, -1. );
    rLinearSpeeds_.resize( TerrainData::nLinearTypes, -1. );
    xis >> xml::list( "speeds", *this, &Speeds::ReadSpeed, timeStepDuration );
    CheckInitialization( xis, timeStepDuration );
    GenerateMasks();
}

// -----------------------------------------------------------------------------
// Name: Speeds constructor
// Created: AGE 2005-02-03
// -----------------------------------------------------------------------------
Speeds::Speeds( const wrapper::View& entity )
    : rMaxSpeed_               ( GET_HOOK( GetTheoricMaxSpeedWithReinforcement )( entity ) )
    , rBaseSpeed_              ( GET_HOOK( GetSpeedWithReinforcement )( entity, TerrainData() ) )
    , nLinearPassabilityMask_  ( 0 )
    , nAreaPassabilityMask_    ( 0 )
    , nAreaImpassabilityMask_  ( 0 )
    , nBorderImpassabilityMask_( 0 )
    , nLinearImpassabilityMask_( 0 )
{
    Initialize( entity );
}

// -----------------------------------------------------------------------------
// Name: Speeds destructor
// Created: AGE 2005-02-03
// -----------------------------------------------------------------------------
Speeds::~Speeds()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Speeds::Initialize
// Created: JSR 2012-02-02
// -----------------------------------------------------------------------------
void Speeds::Initialize( const wrapper::View& entity )
{
    std::vector< std::string > allTypes = TerrainData::GetAllTypes();
    const unsigned int size = TerrainData::nAreaTypes + TerrainData::nBorderTypes + TerrainData::nLinearTypes;
    assert( allTypes.size() - 1 == size ); // sans unknown
    for( unsigned int nOffset = 0; nOffset < size; ++nOffset )
    {
        double speed = GET_HOOK( GetSpeedWithReinforcement )( entity, TerrainData::FromString( allTypes[ nOffset ] ) );
        if( nOffset < TerrainData::nAreaTypes )
            rAreaSpeeds_.push_back( speed );
        else if( nOffset < TerrainData::nAreaTypes + TerrainData::nBorderTypes )
            rBorderSpeeds_.push_back( speed );
        else
            rLinearSpeeds_.push_back( speed );
    }
    GenerateMasks();
}

// -----------------------------------------------------------------------------
// Name: Speeds::ReadSpeed
// Created: ABL 2007-07-23
// -----------------------------------------------------------------------------
void Speeds::ReadSpeed( xml::xistream& xis, unsigned int timeStepDuration )
{
    rMaxSpeed_ = xis.attribute< double >( "max" );
    if( rMaxSpeed_ <= 0 )
        xis.error( "speeds: max <= 0" );
    rMaxSpeed_ *= 1000. * timeStepDuration / 3600.;
    xis >> xml::list( "speed", *this, &Speeds::ReadTerrain );
}

// -----------------------------------------------------------------------------
// Name: Speeds::ReadTerrain
// Created: ABL 2007-07-23
// -----------------------------------------------------------------------------
void Speeds::ReadTerrain( xml::xistream& xis )
{
    std::string strTerrainType = xis.attribute< std::string >( "terrain" );

    const TerrainData data = TerrainData::FromString( strTerrainType );
    if( data.Area() == 0xFF )
        xis.error( "Unknown terrain type '" + strTerrainType + "'" );
    double& speed = SpeedFor( data );
    speed = xis.attribute< double >( "value" );
    if( speed < 0 )
        xis.error( "speed: terrain < 0" );
    speed = GET_HOOK( ConvertSpeedMosToSim )( speed );
    rMaxSpeed_ = std::max( rMaxSpeed_, speed );
    if( xis.has_attribute( "construction-speed" ) )
        rConstructionSpeeds_[ strTerrainType ] = xis.attribute< unsigned int >( "construction-speed" ) * 0.01;
}

namespace
{
    double ConvertSpeedToMOS( double speed, unsigned int timeStepDuration )
    {
        return speed * 3600. / timeStepDuration / 1000.;
    }
    void LogWarning( const TerrainData& terrain, double defaultSpeed = 0 )
    {
        std::stringstream s;
        s << "Speed for "
          << GET_HOOK( GetLandTypeName )( terrain )
          << " not initialized. Defaulted to " << defaultSpeed << " km/h.";
        ::SWORD_Log( SWORD_LOG_LEVEL_WARNING, s.str().c_str() );
    }
}

// -----------------------------------------------------------------------------
// Name: Speeds::CheckInitialization
// Created: AGE 2005-02-03
// -----------------------------------------------------------------------------
void Speeds::CheckInitialization( xml::xistream& xis, unsigned int timeStepDuration )
{
    for( unsigned int i = 0; i < rAreaSpeeds_.size(); ++i )
    {
        if( rAreaSpeeds_[ i ] == -1 )
        {
            rAreaSpeeds_[ i ] = 0;
            LogWarning( TerrainData( static_cast< unsigned char >( 1 << i ), 0, 0, 0 ) );
        }
    }
    for( unsigned int i = 0; i < rBorderSpeeds_.size(); ++i )
    {
        if( rBorderSpeeds_[ i ] == -1 )
        {
            assert( i < rAreaSpeeds_.size() );
            rBorderSpeeds_[ i ] = rAreaSpeeds_[ i ];
            LogWarning( TerrainData( 0, static_cast< unsigned char >( 1 << i ), 0, 0 ), ConvertSpeedToMOS( rBorderSpeeds_[ i ], timeStepDuration ) );
        }
    }
    for( unsigned int i = 0; i < rLinearSpeeds_.size() - 1; ++i )
    {
        if( rLinearSpeeds_[ i ] == -1 )
        {
            rLinearSpeeds_[ i ] = 0;
            LogWarning( TerrainData( 0, 0, 0, static_cast< unsigned short >( 1 << i ) ) );
        }
    }
    const std::size_t lastIndex = rLinearSpeeds_.size() - 1;
    if( rLinearSpeeds_[ lastIndex ] == -1 )
    {
        rLinearSpeeds_[ lastIndex ] = SpeedFor( TerrainData::SmallRoad() );
        LogWarning( TerrainData( 0, 0, 0, static_cast< unsigned short >( 1 << lastIndex ) ), ConvertSpeedToMOS( rLinearSpeeds_[ lastIndex ], timeStepDuration ) );
    }
    if( rMaxSpeed_ == 0. )
        xis.error( "Composante's max speed is 0 km/h ..." );
}

// -----------------------------------------------------------------------------
// Name: Speeds::GetMaxSpeed
// Created: AGE 2005-02-03
// -----------------------------------------------------------------------------
double Speeds::GetMaxSpeed() const
{
    return rMaxSpeed_;
}

// -----------------------------------------------------------------------------
// Name: Speeds::GetMaxSpeed
// Created: AGE 2005-02-03
// -----------------------------------------------------------------------------
double Speeds::GetMaxSpeed( const TerrainData& data ) const
{
    if( ! IsPassable( data ) )
        return 0;
    double rMaxArea = data == TerrainData() ? rBaseSpeed_ : 0;
    double rMaxBorder = rMaxArea;
    double rMaxLinear = rMaxArea;
    {
        unsigned int nOffset = 0;
        const unsigned char area = data.Area();
        for( auto it = rAreaSpeeds_.begin(); it != rAreaSpeeds_.end(); ++it, ++nOffset )
            if( area & ( 1 << nOffset ) && *it > rMaxArea )
                rMaxArea = *it;
    }
    {
        unsigned int nOffset = 0;
        const unsigned char border = data.Border();
        for( auto it = rBorderSpeeds_.begin(); it != rBorderSpeeds_.end(); ++it, ++nOffset )
            if( border & ( 1 << nOffset ) && *it > rMaxBorder )
                rMaxBorder = *it;
    }
    {
        unsigned nOffset = 0;
        const unsigned short linear = data.Linear();
        for( auto it = rLinearSpeeds_.begin(); it != rLinearSpeeds_.end(); ++it, ++nOffset )
            if( linear & ( 1 << nOffset ) && *it > rMaxLinear )
                rMaxLinear = *it;
    }
    if( ( data.Area() & TerrainData::Urban().Area() ) && data.IsRoad() )
        return ( rMaxArea + std::max( rMaxBorder, rMaxLinear ) ) / 2;
    return std::max( rMaxArea, std::max( rMaxBorder, rMaxLinear ) );
}

// -----------------------------------------------------------------------------
// Name: Speeds::IsPassable
// Created: AGE 2005-02-03
// -----------------------------------------------------------------------------
bool Speeds::IsPassable( const TerrainData& data ) const
{
    if( data.Linear() & nLinearPassabilityMask_ )
        return true;
    if( ( data.Linear() & nLinearImpassabilityMask_ )
     || ( data.Border() & nBorderImpassabilityMask_ ) )
        return false;
    if( ( data.Area() & nAreaImpassabilityMask_ )                   // je suis en surface impassable
      & ~( data.Border() & nBorderPassabilityMask_ ) )              // et je ne suis pas sur un bord passable de meme type
        return false;
    return true;
}

// -----------------------------------------------------------------------------
// Name: Speeds::HasConstructionSpeeds
// Created: JSR 2012-02-07
// -----------------------------------------------------------------------------
bool Speeds::HasConstructionSpeeds() const
{
    return !rConstructionSpeeds_.empty();
}

// -----------------------------------------------------------------------------
// Name: Speeds::GetConstructionSpeed
// Created: JSR 2012-02-03
// -----------------------------------------------------------------------------
double Speeds::GetConstructionSpeed( const TerrainData& data ) const
{
    double ret = 1.;
    for( auto it = rConstructionSpeeds_.begin(); it != rConstructionSpeeds_.end(); ++it )
        if( data.ContainsOne( TerrainData::FromString( it->first ) ) )
            ret = std::min( ret, it->second );
    return ret;
}

// -----------------------------------------------------------------------------
// Name: Speeds::GenerateMasks
// Created: AGE 2005-02-03
// -----------------------------------------------------------------------------
void Speeds::GenerateMasks()
{
    {
        nLinearPassabilityMask_ = 0;
        nLinearImpassabilityMask_ = 0;
        unsigned int nOffset = 0;
        for( auto it = rLinearSpeeds_.begin(); it != rLinearSpeeds_.end(); ++it, ++nOffset )
        {
            if( *it == 0. )
                nLinearImpassabilityMask_ |= ( 1 << nOffset );
            else
                nLinearPassabilityMask_ |= ( 1 << nOffset );
        }
    }
    {
        nBorderPassabilityMask_ = 0;
        nBorderImpassabilityMask_ = 0;
        unsigned int nOffset = 0;
        for( auto it = rBorderSpeeds_.begin(); it != rBorderSpeeds_.end(); ++it, ++nOffset )
        {
            if( *it == 0. )
                nBorderImpassabilityMask_ |= ( 1 << nOffset );
            else
                nBorderPassabilityMask_ |= ( 1 << nOffset );
        }
    }
    {
        nAreaPassabilityMask_ = 0;
        nAreaImpassabilityMask_ = 0;
        unsigned int nOffset = 0;
        for( auto it = rAreaSpeeds_.begin(); it != rAreaSpeeds_.end(); ++it, ++nOffset )
        {
            if( *it == 0. )
                nAreaImpassabilityMask_ |= ( 1 << nOffset);
            else
                nAreaPassabilityMask_ |= ( 1 << nOffset);
        }
    }
}

// -----------------------------------------------------------------------------
// Name: Speeds::SpeedFor
// Created: AGE 2005-02-03
// -----------------------------------------------------------------------------
double& Speeds::SpeedFor( const TerrainData& data )
{
    if( data.Area() )
        return rAreaSpeeds_[ data.ExtractAreaOffset() ];
    if( data.Border() )
        return rBorderSpeeds_[ data.ExtractBorderOffset() ];
    if( data.Linear() )
        return rLinearSpeeds_[ data.ExtractLinearOffset() ];
    return rBaseSpeed_;
}

// -----------------------------------------------------------------------------
// Name: Speeds::DumpSpeeds
// Created: AGE 2005-02-03
// -----------------------------------------------------------------------------
void Speeds::DumpSpeeds( const TerrainData& ) const
{
    //MT_LOG_INFO_MSG( "Speed dump not implemented..." ); // $$$$ AGE 2005-02-03:
}
