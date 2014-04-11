// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2002 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RawVisionData.h"
#include "PHY_AmmoEffect.h"
#include "Meteo/PHY_LocalMeteo.h"
#include "Meteo/RawVisionData/ElevationGrid.h"
#include "MT_Tools/MT_Ellipse.h"
#include "MT_Tools/MT_Logger.h"
#include "meteo/Meteo.h"
#include "simulation_terrain/TER_Localisation.h"
#include <tools/InputBinaryStream.h>
#include <tools/Path.h>
#include <boost/lexical_cast.hpp>

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData constructor
// Created: JVT 02-11-05
// Last modified: JVT 03-08-08
//-----------------------------------------------------------------------------
PHY_RawVisionData::PHY_RawVisionData( const weather::Meteo& globalMeteo,
        const tools::Path& detection )
    : nNbrCol_( 0 )
    , nNbrRow_( 0 )
    , pElevationGrid_( 0 )
    , globalMeteo_( globalMeteo )
{
    MT_LOG_INFO_MSG( "Initializing vision data" );
    Read( detection );
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData destructor
// Created: JVT 02-11-05
//-----------------------------------------------------------------------------
PHY_RawVisionData::~PHY_RawVisionData()
{
    // NOTHING
}

namespace
{

uint32_t GetActiveLocalWeatherOnLocation(
        const std::set< boost::shared_ptr< const PHY_LocalMeteo > >& meteos,
        const geometry::Point2f& position )
{
    const PHY_LocalMeteo* result = 0;
    for( auto it = meteos.begin(); it != meteos.end(); ++it )
    {
        const auto& meteo = *it;
        if( meteo->IsInside( position )
            && ( !result 
                || meteo->GetStartTime() > result->GetStartTime()
                || meteo->GetStartTime() == result->GetStartTime()
                    && meteo->GetId() > result->GetId() ))
            result = meteo.get();
    }
    return result ? result->GetId() : 0;
}

}  // namespace

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::RegisterMeteoPatch
// Created: JVT 03-08-06
// Last modified: JVT 03-08-18
//-----------------------------------------------------------------------------
void PHY_RawVisionData::RegisterMeteoPatch(
        const boost::shared_ptr< const PHY_LocalMeteo >& pMeteo )
{
    if( meteoIds_.count( pMeteo->GetId() ) > 0 || !meteos_.insert( pMeteo ).second )
        throw MASA_EXCEPTION( "weather instance "
            + boost::lexical_cast< std::string >( pMeteo->GetId() )
            + " is already registered in vision data" );
    meteoIds_[ pMeteo->GetId() ] = pMeteo.get();

    const auto upLeft = pMeteo->GetTopLeft();
    const auto downRight = pMeteo->GetBottomRight();

    unsigned int nXEnd = std::min( GetCol( downRight.GetX() ), nNbrCol_ - 1 );
    unsigned int nYEnd = std::min( GetRow( upLeft.GetY() ),    nNbrRow_ - 1 );
    unsigned int nXBeg = std::min( GetCol( upLeft.GetX() ),    nNbrCol_ - 1 );
    unsigned int nYBeg = std::min( GetRow( downRight.GetY() ), nNbrRow_ - 1 );

    // On remet éventuellement dans le bon sens
    if( nXEnd < nXBeg )
        std::swap( nXEnd, nXBeg );
    if( nYEnd < nYBeg )
        std::swap( nYEnd, nYBeg );

    while( nXBeg <= nXEnd )
    {
        for( unsigned int y = nYBeg; y <= nYEnd; ++y )
        {
            ElevationCell& cell = pElevationGrid_->GetCell( nXBeg, y );
            cell.weatherId = pMeteo->GetId();
        }
        ++nXBeg;
    }
}

//----------------------------------------------------------------------------
// Name: PHY_RawVisionData::UnregisterLocalMeteoPatch
// Created: SLG 2010-03-19
//-----------------------------------------------------------------------------
void PHY_RawVisionData::UnregisterMeteoPatch(
        const boost::shared_ptr< const PHY_LocalMeteo >& pMeteo )
{
    if( meteoIds_.count( pMeteo->GetId() ) == 0 || !meteos_.erase( pMeteo ) )
        throw MASA_EXCEPTION( "weather instance "
            + boost::lexical_cast< std::string >( pMeteo->GetId() )
            + " is already unregistered from vision data" );
    meteoIds_.erase( pMeteo->GetId() );

    const auto upLeft = pMeteo->GetTopLeft();
    const auto downRight = pMeteo->GetBottomRight();

    unsigned int nXEnd = std::min( GetCol( downRight.GetX() ), nNbrCol_ - 1 );
    unsigned int nYEnd = std::min( GetRow( upLeft.GetY() ),    nNbrRow_ - 1 );
    unsigned int nXBeg = std::min( GetCol( upLeft.GetX() ),    nNbrCol_ - 1 );
    unsigned int nYBeg = std::min( GetRow( downRight.GetY() ), nNbrRow_ - 1 );

    // On remet éventuellement dans le bon sens
    if( nXEnd < nXBeg )
        std::swap( nXEnd, nXBeg );
    if( nYEnd < nYBeg )
        std::swap( nYEnd, nYBeg );

    while( nXBeg <= nXEnd )
    {
        for( unsigned int y = nYBeg; y <= nYEnd; ++y )
        {
            ElevationCell& cell = pElevationGrid_->GetCell( nXBeg, y );
            const auto pos = geometry::Point2f(
                    static_cast< float >( nXBeg * rCellSize_ ),
                    static_cast< float >( y * rCellSize_ ) );
            cell.weatherId = GetActiveLocalWeatherOnLocation( meteos_, pos );
        }
        ++nXBeg;
    }
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::RegisterWeatherEffect
// Created: JVT 04-03-24
//-----------------------------------------------------------------------------
void PHY_RawVisionData::RegisterWeatherEffect( const MT_Ellipse& surface, const PHY_IndirectFireDotationClass& weaponClass )
{
    const MT_Rect bb = surface.GetBoundingBox();
    double x = floor( bb.GetLeft() / rCellSize_ ) * rCellSize_;
    const double xMax = ceil ( bb.GetRight()  / rCellSize_ ) * rCellSize_;
    const double yMin = floor( bb.GetBottom() / rCellSize_ ) * rCellSize_;
    const double yMax = ceil ( bb.GetTop()    / rCellSize_ ) * rCellSize_;
    for( ; x < xMax; x += rCellSize_ )
        for( double y = yMin; y < yMax; y += rCellSize_ )
            if( surface.IsInside( MT_Vector2D( x, y ) ) )
            {
                ElevationCell& cell = pElevationGrid_->GetCell( GetCol( x ), GetRow( y ) );
                if( &cell == &pElevationGrid_->GetEmptyCell() )
                    continue;
                PHY_AmmoEffect* pEffect = new PHY_AmmoEffect( weaponClass, cell.pEffects );
                cell.pEffects = pEffect;
            }
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::UnregisterWeatherEffect
// Created: JVT 04-03-24
//-----------------------------------------------------------------------------
void PHY_RawVisionData::UnregisterWeatherEffect( const MT_Ellipse& surface, const PHY_IndirectFireDotationClass& weaponClass )
{
    const MT_Rect bb = surface.GetBoundingBox();
    double x          = floor( bb.GetLeft()   / rCellSize_ ) * rCellSize_;
    const double xMax = ceil ( bb.GetRight()  / rCellSize_ ) * rCellSize_;
    const double yMin = floor( bb.GetBottom() / rCellSize_ ) * rCellSize_;
    const double yMax = ceil ( bb.GetTop()    / rCellSize_ ) * rCellSize_;

    for( ; x < xMax; x += rCellSize_ )
        for( double y = yMin; y < yMax; y += rCellSize_ )
            if( surface.IsInside( MT_Vector2D( x, y ) ) )
            {
                 ElevationCell& cell = pElevationGrid_->GetCell( GetCol( x ), GetRow( y ) );
                if( &cell == &pElevationGrid_->GetEmptyCell() )
                    continue;

                PHY_AmmoEffect* pPrevEffect = 0;
                PHY_AmmoEffect* pEffect = cell.pEffects;

                while( pEffect && !pEffect->HandleAmmo( weaponClass ) )
                {
                    pPrevEffect = pEffect;
                    pEffect     = pEffect->GetNextEffect();
                }

                if( !pEffect || !pEffect->HandleAmmo( weaponClass ) )
                {
                    assert( false );
                    return;
                }

                if( pPrevEffect )
                    pPrevEffect->SetNextEffect( pEffect->GetNextEffect() );
                else
                    cell.pEffects = pEffect->GetNextEffect();

                delete pEffect;
            }
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::Read
// Created: JVT 02-12-11
// Last modified: JVT 04-03-24
//-----------------------------------------------------------------------------
bool PHY_RawVisionData::Read( const tools::Path& path )
{
    tools::InputBinaryStream archive( path );
    if( !archive )
        throw MASA_EXCEPTION( "Cannot open file " + path.ToUTF8() );
    if( !( archive >> rCellSize_ >> nNbrRow_ >> nNbrCol_ ) )
       throw MASA_EXCEPTION( "Error reading file " + path.ToUTF8() );

    ElevationCell** ppCells = new ElevationCell*[ nNbrCol_ ];

    uint16_t elevation;
    uint8_t env, delta;
    for( unsigned int x = 0; x < nNbrCol_; ++x )
    {
        ElevationCell* pTmp = new ElevationCell[ nNbrRow_ ];
        ppCells[ x ] = pTmp;

        for( unsigned int i = 0; i < nNbrRow_; ++i )
        {
            archive >> elevation >> delta >> env;
            if( !archive )
                throw MASA_EXCEPTION( "Error reading file " + path.ToUTF8() );
            pTmp->h = elevation;
            pTmp->dh = delta;
            pTmp->e = env;
            pTmp->weatherId = 0;
            pTmp->pEffects = 0;
            pTmp++;
        }
    }

    pElevationGrid_.reset( new ElevationGrid( rCellSize_, nNbrCol_, nNbrRow_, ppCells ) );

    CalcMinMaxAltitude();
    return true;
}

namespace
{
    std::vector< geometry::Point2d > Convert( const T_PointVector& vector )
    {
        std::vector< geometry::Point2d > result;
        for( auto it = vector.begin(); it != vector.end(); ++it )
            result.push_back( geometry::Point2d( it->rX_, it->rY_ ) );
        return result;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RawVisionData::ModifyAltitude
// Created: JSR 2011-05-19
// -----------------------------------------------------------------------------
void PHY_RawVisionData::ModifyAltitude( const TER_Localisation& localisation, short heightOffset, unsigned int objectId )
{
    pElevationGrid_->SetAltitude(
        objectId, Convert( localisation.GetPoints() ), localisation.GetType() == TER_Localisation::ePolygon, heightOffset );
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::CalcMinMaxAltitude
// Created: FBD 03-02-13
// Last modified: JVT 03-06-05
//-----------------------------------------------------------------------------
void PHY_RawVisionData::CalcMinMaxAltitude()
{
    short nMaxAltitude = std::numeric_limits< short >::min();
    short nMinAltitude = std::numeric_limits< short >::max();
    for( unsigned int nX = 0; nX < nNbrCol_; ++nX  )
    {
        for( unsigned int nY = 0; nY < nNbrRow_; ++nY )
        {
            short nAltitude = pElevationGrid_->GetCell( nX, nY ).h;
            if( nAltitude < nMinAltitude )
                nMinAltitude = nAltitude;
            if( nAltitude > nMaxAltitude )
                nMaxAltitude = nAltitude;
        }
    }
    rMaxAltitude_ = static_cast< double >( nMaxAltitude );
    rMinAltitude_ = static_cast< double >( nMinAltitude );
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::operator ()
// Created: JVT 02-11-05
//-----------------------------------------------------------------------------
const ElevationCell& PHY_RawVisionData::operator () ( unsigned int col, unsigned int row ) const
{
    return pElevationGrid_->GetCell( col, row );
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::operator
// Created: JVT 02-11-15
//-----------------------------------------------------------------------------
const ElevationCell& PHY_RawVisionData::operator() ( const MT_Vector2D& pos ) const
{
    return pElevationGrid_->GetCell( GetCol( pos.rX_ ), GetRow( pos.rY_ ) );
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::operator
// Created: JVT 02-11-15
//-----------------------------------------------------------------------------
const ElevationCell& PHY_RawVisionData::operator() ( double x, double y ) const
{
    return operator () ( GetCol( x ), GetRow( y ) );
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::operator
// Created: JVT 04-03-24
//-----------------------------------------------------------------------------
ElevationCell& PHY_RawVisionData::operator() ( double rCol, double rRow )
{
    return pElevationGrid_->GetCell( GetCol( rCol ), GetRow( rRow ) );
}

const weather::Meteo& PHY_RawVisionData::GetWeather( const MT_Vector2D& pos ) const
{
    return GetWeather( operator()( pos ) );
}

const weather::Meteo& PHY_RawVisionData::GetWeather( const ElevationCell& cell ) const
{
    if( !cell.weatherId )
        return globalMeteo_;
    return *meteoIds_.at( cell.weatherId );
}

// -----------------------------------------------------------------------------
// Name: PHY_RawVisionData::GetPrecipitation
// Created: BCI 2010-12-13
// -----------------------------------------------------------------------------
const weather::PHY_Precipitation& PHY_RawVisionData::GetPrecipitation( const MT_Vector2D& vPos ) const
{
    return GetPrecipitation( operator()( vPos ) );
}

const weather::PHY_Precipitation& PHY_RawVisionData::GetPrecipitation( const ElevationCell& cell ) const
{
    const auto& meteo = GetWeather( cell );
    if( cell.pEffects )
        return cell.pEffects->GetPrecipitation( meteo.GetPrecipitation() );
    return meteo.GetPrecipitation();
}

const weather::PHY_Lighting& PHY_RawVisionData::GetLighting( const ElevationCell& cell ) const
{
    const auto& meteo = GetWeather( cell );
    if( cell.pEffects )
        return cell.pEffects->GetLighting( meteo.GetLighting() );
    return meteo.GetLighting();
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::GetAltitude
// Created: JVT 03-02-24
//-----------------------------------------------------------------------------
double PHY_RawVisionData::GetAltitude( double rX, double rY, bool applyOnCell /*= false*/ ) const
{
    return pElevationGrid_->ElevationAt( rX, rY, applyOnCell );
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::GetAltitude
// Created: JVT 02-11-28
// Last modified: JVT 03-02-24
//-----------------------------------------------------------------------------
double PHY_RawVisionData::GetAltitude( const MT_Vector2D& pos, bool applyOnCell /*= false*/ ) const
{
    return GetAltitude( pos.rX_, pos.rY_, applyOnCell );
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::GetVisionObject
// Created: JVT 03-07-04
//-----------------------------------------------------------------------------
envBits PHY_RawVisionData::GetVisionObject( const MT_Vector2D& pos ) const
{
    return operator()( pos ).GetEnv();
}

// -----------------------------------------------------------------------------
// Name: PHY_RawVisionData::GetWind
// Created: JVT 2004-10-29
// -----------------------------------------------------------------------------
const weather::WindData& PHY_RawVisionData::GetWind( const MT_Vector2D& vPos ) const
{
    const auto& meteo = GetWeather( vPos );
    return meteo.GetWind();
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::GetVisionObject
// Created: JVT 03-07-04
//-----------------------------------------------------------------------------
envBits PHY_RawVisionData::GetVisionObject( double rX_, double rY_ ) const
{
    return operator()( rX_, rY_ ).GetEnv();
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::Getcol
// Created: JVT 02-11-05
//-----------------------------------------------------------------------------
unsigned int PHY_RawVisionData::GetCol( double x ) const
{
    return static_cast< unsigned int >( x / rCellSize_ );
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::Getrow
// Created: JVT 02-11-05
//-----------------------------------------------------------------------------
unsigned int PHY_RawVisionData::GetRow( double y ) const
{
    return static_cast< unsigned int >( y / rCellSize_ );
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::GetCellSize
// Created: JVT 02-11-21
//-----------------------------------------------------------------------------
double PHY_RawVisionData::GetCellSize() const
{
    return rCellSize_;
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::GetMinAltitude
// Created: FBD 03-02-13
//-----------------------------------------------------------------------------
double PHY_RawVisionData::GetMinAltitude() const
{
    return rMinAltitude_;
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::GetMaxAltitude
// Created: FBD 03-02-13
//-----------------------------------------------------------------------------
double PHY_RawVisionData::GetMaxAltitude() const
{
    return rMaxAltitude_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RawVisionData::GetVisionObjectsInSurface
// Created: NLD 2004-11-17
// -----------------------------------------------------------------------------
void PHY_RawVisionData::GetVisionObjectsInSurface( const TER_Localisation_ABC& localisation, unsigned int& nEmptySurface, unsigned int& nForestSurface, unsigned int& nUrbanSurface ) const
{
    nEmptySurface  = 0;
    nForestSurface = 0;
    nUrbanSurface  = 0;

    const unsigned int nXEnd   = GetCol( localisation.GetBoundingBox().GetRight() );
    const unsigned int nYBegin = GetRow( localisation.GetBoundingBox().GetBottom() );
    const unsigned int nYEnd   = GetRow( localisation.GetBoundingBox().GetTop() );

    for( unsigned int nX = GetCol( localisation.GetBoundingBox().GetLeft() ); nX <= nXEnd; ++nX )
        for( unsigned int nY = nYBegin; nY <= nYEnd; ++nY )
        {
            if( !localisation.IsInside( MT_Vector2D( nX * rCellSize_, nY * rCellSize_ ) ) )
                continue;

            const envBits env = operator () ( nX, nY ).GetEnv();

            if( env == eVisionEmpty )
                ++nEmptySurface;
            else
            {
                if( env & eVisionForest )
                    ++nForestSurface;
                if( env & eVisionUrban )
                    ++nUrbanSurface;
            }
        }

    unsigned int cellSizeSquare = static_cast< unsigned int >( rCellSize_ * rCellSize_ );
    nEmptySurface  *= cellSizeSquare;
    nForestSurface *= cellSizeSquare;
    nUrbanSurface  *= cellSizeSquare;
}

bool PHY_RawVisionData::IsWeatherPatched(
        const boost::shared_ptr< const PHY_LocalMeteo >& weather ) const
{
    return meteos_.count( weather ) > 0;
}
