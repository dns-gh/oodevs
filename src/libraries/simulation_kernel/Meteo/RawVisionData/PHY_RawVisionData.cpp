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
#include "Meteo/PHY_MeteoDataManager.h"
#include "MT_Tools/MT_Ellipse.h"
#include "MT_Tools/MT_Logger.h"
#include <tools/InputBinaryStream.h>
#include <tools/Path.h>

const weather::Meteo* ElevationGrid::sCell::pGlobalMeteo_ = 0;

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::sCell::GetPrecipitation
// Created: JVT 04-03-24
//-----------------------------------------------------------------------------
const weather::PHY_Precipitation& ElevationGrid::sCell::GetPrecipitation() const
{
    const weather::PHY_Precipitation& mainPrecipitation = pMeteo ? pMeteo->GetPrecipitation() : pGlobalMeteo_->GetPrecipitation();
    return pEffects ? pEffects->GetPrecipitation( mainPrecipitation ) : mainPrecipitation;
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::GetLighting
// Created: JVT 04-03-24
//-----------------------------------------------------------------------------
const weather::PHY_Lighting& ElevationGrid::sCell::GetLighting() const
{
    const weather::PHY_Lighting& mainLighting = pMeteo ? pMeteo->GetLighting() : pGlobalMeteo_->GetLighting();
    return pEffects ? pEffects->GetLighting( mainLighting ) : mainLighting;
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData constructor
// Created: JVT 02-11-05
// Last modified: JVT 03-08-08
//-----------------------------------------------------------------------------
PHY_RawVisionData::PHY_RawVisionData( weather::Meteo& globalMeteo, const tools::Path& detection, PHY_MeteoDataManager* manager )
    : nNbrCol_( 0 )
    , nNbrRow_( 0 )
    , meteoManager_( manager )
    , pElevationGrid_( 0 )
{
    MT_LOG_INFO_MSG( "Initializing vision data" );
    Read( detection );
    ElevationGrid::sCell::pGlobalMeteo_ = &globalMeteo;
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData destructor
// Created: JVT 02-11-05
//-----------------------------------------------------------------------------
PHY_RawVisionData::~PHY_RawVisionData()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::RegisterMeteoPatch
// Created: JVT 03-08-06
// Last modified: JVT 03-08-18
//-----------------------------------------------------------------------------
void PHY_RawVisionData::RegisterMeteoPatch( const geometry::Point2d& upLeft, const geometry::Point2d& downRight, boost::shared_ptr< weather::Meteo > pMeteo )
{
    unsigned int nXEnd = std::min( GetCol( downRight.X() ), nNbrCol_ - 1 );
    unsigned int nYEnd = std::min( GetRow( upLeft.Y() ),    nNbrRow_ - 1 );
    unsigned int nXBeg = std::min( GetCol( upLeft.X() ),    nNbrCol_ - 1 );
    unsigned int nYBeg = std::min( GetRow( downRight.Y() ), nNbrRow_ - 1 );

    // On remet �ventuellement dans le bon sens
    if( nXEnd < nXBeg )
        std::swap( nXEnd, nXBeg );
    if( nYEnd < nYBeg )
        std::swap( nYEnd, nYBeg );

    while( nXBeg <= nXEnd )
    {
        for( unsigned int y = nYBeg; y <= nYEnd; ++y )
        {
            ElevationGrid::sCell& cell = pElevationGrid_->GetCell( nXBeg, y );
            cell.pMeteo = pMeteo;
        }
        ++nXBeg;
    }
}

//----------------------------------------------------------------------------
// Name: PHY_RawVisionData::UnregisterLocalMeteoPatch
// Created: SLG 2010-03-19
//-----------------------------------------------------------------------------
void PHY_RawVisionData::UnregisterMeteoPatch( const geometry::Point2d& upLeft, const geometry::Point2d& downRight, boost::shared_ptr< weather::Meteo > pMeteo )
{
    unsigned int nXEnd = std::min( GetCol( downRight.X() ), nNbrCol_ - 1 );
    unsigned int nYEnd = std::min( GetRow( upLeft.Y() ),    nNbrRow_ - 1 );
    unsigned int nXBeg = std::min( GetCol( upLeft.X() ),    nNbrCol_ - 1 );
    unsigned int nYBeg = std::min( GetRow( downRight.Y() ), nNbrRow_ - 1 );

    // On remet �ventuellement dans le bon sens
    if( nXEnd < nXBeg )
        std::swap( nXEnd, nXBeg );
    if( nYEnd < nYBeg )
        std::swap( nYEnd, nYBeg );

    while( nXBeg <= nXEnd )
    {
        for( unsigned int y = nYBeg; y <= nYEnd; ++y )
        {
            ElevationGrid::sCell& cell = pElevationGrid_->GetCell( nXBeg, y );
            boost::shared_ptr< weather::Meteo > meteo = meteoManager_->GetLocalWeather( geometry::Point2f( static_cast< float >( nXBeg * rCellSize_ ), static_cast< float >( y * rCellSize_ ) ), pMeteo );
            if( meteo.get() && cell.pMeteo.get() != meteo.get() )
                cell.pMeteo = meteo;
            else if( !meteo )
                cell.pMeteo.reset();
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
                ElevationGrid::sCell& cell = pElevationGrid_->GetCell( GetCol( x ), GetRow( y ) );
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
                 ElevationGrid::sCell& cell = pElevationGrid_->GetCell( GetCol( x ), GetRow( y ) );
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

    ElevationGrid::sCell** ppCells = new ElevationGrid::sCell*[ nNbrCol_ ];

    for( unsigned int x = 0; x < nNbrCol_; ++x )
    {
        ElevationGrid::sCell* pTmp = new ElevationGrid::sCell[ nNbrRow_ ];
        ppCells[ x ] = pTmp;

        for( unsigned int i = 0; i < nNbrRow_; ++i )
        {
            pTmp->pMeteo.reset();
            pTmp->pEffects = 0;
            archive.Read( reinterpret_cast< char* >( pTmp++ ), 4 );
            if( !archive )
                throw MASA_EXCEPTION( "Error reading file " + path.ToUTF8() );
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
const ElevationGrid::sCell& PHY_RawVisionData::operator () ( unsigned int col, unsigned int row ) const
{
    return pElevationGrid_->GetCell( col, row );
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::operator
// Created: JVT 02-11-15
//-----------------------------------------------------------------------------
const ElevationGrid::sCell& PHY_RawVisionData::operator() ( const MT_Vector2D& pos ) const
{
    return pElevationGrid_->GetCell( GetCol( pos.rX_ ), GetRow( pos.rY_ ) );
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::operator
// Created: JVT 02-11-15
//-----------------------------------------------------------------------------
const ElevationGrid::sCell& PHY_RawVisionData::operator() ( double x, double y ) const
{
    return operator () ( GetCol( x ), GetRow( y ) );
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::operator
// Created: JVT 04-03-24
//-----------------------------------------------------------------------------
ElevationGrid::sCell& PHY_RawVisionData::operator() ( double rCol, double rRow )
{
    return pElevationGrid_->GetCell( GetCol( rCol ), GetRow( rRow ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_RawVisionData::GetPrecipitation
// Created: BCI 2010-12-13
// -----------------------------------------------------------------------------
const weather::PHY_Precipitation& PHY_RawVisionData::GetPrecipitation( const MT_Vector2D& vPos ) const
{
    return operator() ( vPos ).GetPrecipitation();
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
ElevationGrid::envBits PHY_RawVisionData::GetVisionObject( const MT_Vector2D& pos ) const
{
    return operator()( pos ).GetEnv();
}

// -----------------------------------------------------------------------------
// Name: PHY_RawVisionData::GetWind
// Created: JVT 2004-10-29
// -----------------------------------------------------------------------------
const weather::WindData& PHY_RawVisionData::GetWind( const MT_Vector2D& vPos ) const
{
    return operator()( vPos ).GetWind();
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::GetVisionObject
// Created: JVT 03-07-04
//-----------------------------------------------------------------------------
ElevationGrid::envBits PHY_RawVisionData::GetVisionObject( double rX_, double rY_ ) const
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
// Name: PHY_RawVisionData::GetWind
// Created: JVT 2004-10-29
// -----------------------------------------------------------------------------
const weather::WindData& ElevationGrid::sCell::GetWind() const
{
    return pMeteo ? pMeteo->GetWind() : pGlobalMeteo_->GetWind();
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

            const ElevationGrid::envBits env = operator () ( nX, nY ).GetEnv();

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
