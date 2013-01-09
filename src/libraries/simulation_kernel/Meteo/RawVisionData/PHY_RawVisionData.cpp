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
#include "MIL_AgentServer.h"
#include "meteo/PHY_MeteoDataManager.h"
#include "MT_Tools/MT_FormatString.h"
#include "MT_Tools/MT_Ellipse.h"
#include "MT_Tools/MT_Logger.h"
#include "MT_Tools/MT_InterpolatedFunction.h"
#include "tools/InputBinaryStream.h"

PHY_RawVisionData::sCell PHY_RawVisionData::emptyCell_;
const weather::Meteo* PHY_RawVisionData::sCell::pGlobalMeteo_ = 0;

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::sCell::GetPrecipitation
// Created: JVT 04-03-24
//-----------------------------------------------------------------------------
const weather::PHY_Precipitation& PHY_RawVisionData::sCell::GetPrecipitation() const
{
    const weather::PHY_Precipitation& mainPrecipitation = pMeteo ? pMeteo->GetPrecipitation() : pGlobalMeteo_->GetPrecipitation();
    return pEffects ? pEffects->GetPrecipitation( mainPrecipitation ) : mainPrecipitation;
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::GetLighting
// Created: JVT 04-03-24
//-----------------------------------------------------------------------------
const weather::PHY_Lighting& PHY_RawVisionData::sCell::GetLighting() const
{
    const weather::PHY_Lighting& mainLighting = pMeteo ? pMeteo->GetLighting() : pGlobalMeteo_->GetLighting();
    return pEffects ? pEffects->GetLighting( mainLighting ) : mainLighting;
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData constructor
// Created: JVT 02-11-05
// Last modified: JVT 03-08-08
//-----------------------------------------------------------------------------
PHY_RawVisionData::PHY_RawVisionData( weather::Meteo& globalMeteo, const std::string& detection )
    : ppCells_( 0 )
    , nNbrCol_( 0 )
    , nNbrRow_( 0 )
{
    MT_LOG_INFO_MSG( "Initializing vision data" );
    MIL_AgentServer::GetWorkspace().GetConfig().AddFileToCRC( detection );
    Read( detection );
    sCell::pGlobalMeteo_ = &globalMeteo;
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData destructor
// Created: JVT 02-11-05
//-----------------------------------------------------------------------------
PHY_RawVisionData::~PHY_RawVisionData()
{
    for( unsigned int i = nNbrCol_; i; )
        delete [] ppCells_[ --i ];
    delete [] ppCells_;
    ppCells_ = 0;
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::RegisterMeteoPatch
// Created: JVT 03-08-06
// Last modified: JVT 03-08-18
//-----------------------------------------------------------------------------
void PHY_RawVisionData::RegisterMeteoPatch( const geometry::Point2d& upLeft, const geometry::Point2d& downRight, boost::shared_ptr< weather::Meteo > pMeteo )
{
    assert( ppCells_ );
    unsigned int nXEnd = std::min( GetCol( downRight.X() ), nNbrCol_ - 1 );
    unsigned int nYEnd = std::min( GetRow( upLeft.Y() ),    nNbrRow_ - 1 );
    unsigned int nXBeg = std::min( GetCol( upLeft.X() ),    nNbrCol_ - 1 );
    unsigned int nYBeg = std::min( GetRow( downRight.Y() ), nNbrRow_ - 1 );

    // On remet éventuellement dans le bon sens
    if( nXEnd < nXBeg )
        std::swap( nXEnd, nXBeg );
    if( nYEnd < nYBeg )
        std::swap( nYEnd, nYBeg );

    while( nXBeg <= nXEnd )
    {
        for( unsigned int y = nYBeg; y <= nYEnd; ++y )
        {
            sCell& cell = ppCells_[ nXBeg ][ y ];
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
    assert( ppCells_ );
    unsigned int nXEnd = std::min( GetCol( downRight.X() ), nNbrCol_ - 1 );
    unsigned int nYEnd = std::min( GetRow( upLeft.Y() ),    nNbrRow_ - 1 );
    unsigned int nXBeg = std::min( GetCol( upLeft.X() ),    nNbrCol_ - 1 );
    unsigned int nYBeg = std::min( GetRow( downRight.Y() ), nNbrRow_ - 1 );

    // On remet éventuellement dans le bon sens
    if( nXEnd < nXBeg )
        std::swap( nXEnd, nXBeg );
    if( nYEnd < nYBeg )
        std::swap( nYEnd, nYBeg );

    assert( MIL_AgentServer::IsInitialized() );
    PHY_MeteoDataManager& meteoManager = MIL_AgentServer::GetWorkspace().GetMeteoDataManager();

    while( nXBeg <= nXEnd )
    {
        for( unsigned int y = nYBeg; y <= nYEnd; ++y )
        {
            sCell& cell = ppCells_[ nXBeg ][ y ];
            boost::shared_ptr< weather::Meteo > meteo = meteoManager.GetLocalWeather( geometry::Point2f( static_cast< float >( nXBeg * rCellSize_ ), static_cast< float >( y * rCellSize_ ) ), pMeteo );
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
                sCell& cell = operator () ( x, y );
                if( &cell == &emptyCell_ )
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
    double y          = floor( bb.GetBottom() / rCellSize_ ) * rCellSize_;
    const double yMax = ceil ( bb.GetTop()    / rCellSize_ ) * rCellSize_;

    for( ; x < xMax; x += rCellSize_ )
        for( ; y < yMax; y += rCellSize_ )
            if( surface.IsInside( MT_Vector2D( x, y ) ) )
            {
                sCell& cell = operator () ( x, y );
                if( &cell == &emptyCell_ )
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
bool PHY_RawVisionData::Read( const std::string& strFile )
{
    tools::InputBinaryStream archive( strFile.c_str() );
    if( !archive )
        throw MASA_EXCEPTION( MT_FormatString( "Cannot open file %s", strFile.c_str() ) );

    if( !( archive >> rCellSize_ >> nNbrRow_ >> nNbrCol_ ) )
       throw MASA_EXCEPTION( MT_FormatString( "Error reading file %s", strFile.c_str() ) );

    assert( !ppCells_ );
    ppCells_ = new sCell*[ nNbrCol_ ];

    for( unsigned int x = 0; x < nNbrCol_; ++x )
    {
        sCell* pTmp = new sCell[ nNbrRow_ ];
        ppCells_[ x ] = pTmp;

        for( unsigned int i = 0; i < nNbrRow_; ++i )
        {
            pTmp->pMeteo.reset();
            pTmp->pEffects = 0;
            archive.Read( reinterpret_cast< char* >( pTmp++ ), 4 );
            if( !archive )
                throw MASA_EXCEPTION( MT_FormatString( "Error reading file %s", strFile.c_str() ) );
        }
    }
    CalcMinMaxAltitude();
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RawVisionData::ModifyAltitude
// Created: JSR 2011-05-19
// -----------------------------------------------------------------------------
void PHY_RawVisionData::ModifyAltitude( const TER_Localisation& localisation, short heightOffset, unsigned int objectId )
{
    if( heightOffset == 0 )
        elevationOffsets_.erase( objectId );
    else
    {
        elevationOffsets_[ objectId ].localisation_ = localisation;
        elevationOffsets_[ objectId ].offset_ = heightOffset;
    }
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
            short nAltitude = ppCells_[ nX ][ nY ].h;
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
const PHY_RawVisionData::sCell& PHY_RawVisionData::operator () ( unsigned int col, unsigned int row ) const
{
    return ( col < nNbrCol_ && row < nNbrRow_ ) ? ppCells_[ col ][ row ] : emptyCell_;
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::operator
// Created: JVT 02-11-15
//-----------------------------------------------------------------------------
const PHY_RawVisionData::sCell& PHY_RawVisionData::operator() ( const MT_Vector2D& pos ) const
{
    return operator () ( GetCol( pos.rX_ ), GetRow( pos.rY_ ) );
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::operator
// Created: JVT 02-11-15
//-----------------------------------------------------------------------------
const PHY_RawVisionData::sCell& PHY_RawVisionData::operator() ( double x, double y ) const
{
    return operator () ( GetCol( x ), GetRow( y ) );
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::operator
// Created: JVT 04-03-24
//-----------------------------------------------------------------------------
PHY_RawVisionData::sCell& PHY_RawVisionData::operator() ( double rCol, double rRow )
{
    unsigned int nCol = GetCol( rCol );
    unsigned int nRow = GetRow( rRow );
    return ( nCol < nNbrCol_ && nRow < nNbrRow_ ) ? ppCells_[ nCol ][ nRow ] : emptyCell_;
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
    unsigned int nCol = GetCol( rX );
    unsigned int nRow = GetRow( rY );

    double rScaledX = rX / rCellSize_;

    const double halfCellSize_ = 0.5 * rCellSize_;
    const MT_Vector2D offsets[] =
    {
        MT_Vector2D(  halfCellSize_,  halfCellSize_ ),
        MT_Vector2D( -halfCellSize_,  halfCellSize_ ),
        MT_Vector2D( -halfCellSize_, -halfCellSize_ ),
        MT_Vector2D(  halfCellSize_, -halfCellSize_ ),
    };
    const size_t offsetsLen = sizeof( offsets )/sizeof( *offsets );
    T_PointVector cellVector;

    short maxOffset = 0;
    for( auto it = elevationOffsets_.begin(); it != elevationOffsets_.end(); ++it )
    {
        if( it->second.offset_ > maxOffset )
        {
            MT_Vector2D point( rX, rY );
            if( applyOnCell )
            {
                if( it->second.localisation_.GetType() == TER_Localisation::ePolygon )
                {
                    if( it->second.localisation_.Intersect2DWithCircle( point, 1.414f * halfCellSize_ ) )
                        maxOffset = it->second.offset_;
                }
                else
                {
                    if( cellVector.empty() )
                        cellVector.resize( offsetsLen );
                    for( size_t i = 0; i != offsetsLen; ++i )
                        cellVector[i] = point + offsets[i];
                    TER_Localisation cell( TER_Localisation::ePolygon, cellVector );
                    const T_PointVector linePoints = it->second.localisation_.GetPoints();
                    for( std::size_t i = 0; i < linePoints.size() - 1; ++i )
                    {
                        if( cell.Intersect2D( MT_Line( linePoints[ i ], linePoints[ i + 1 ] ) ) )
                        {
                            maxOffset = it->second.offset_;
                            break;
                        }
                    }
                }
            }
            else
            {
                if( it->second.localisation_.GetType() == TER_Localisation::ePolygon && it->second.localisation_.IsInside( point ) )
                    maxOffset = it->second.offset_;
            }
        }
    }

    return Interpolate(
                 nRow,
                 Interpolate(
                    nCol,
                    operator()( nCol, nRow ).GetAltitude(),
                    nCol + 1,
                    operator()( nCol + 1, nRow ).GetAltitude(),
                    rScaledX ),
                 nRow + 1,
                 Interpolate(
                    nCol,
                    operator()( nCol, nRow + 1 ).GetAltitude(),
                    nCol + 1,
                    operator()( nCol + 1, nRow + 1 ).GetAltitude(),
                    rScaledX ),
                 rY / rCellSize_ ) + maxOffset;
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
PHY_RawVisionData::envBits PHY_RawVisionData::GetVisionObject( const MT_Vector2D& pos ) const
{
    return operator()( pos ).GetEnv();
}

// -----------------------------------------------------------------------------
// Name: PHY_RawVisionData::GetWind
// Created: JVT 2004-10-29
// -----------------------------------------------------------------------------
const weather::Meteo::sWindData& PHY_RawVisionData::GetWind( const MT_Vector2D& vPos ) const
{
    return operator()( vPos ).GetWind();
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::GetVisionObject
// Created: JVT 03-07-04
//-----------------------------------------------------------------------------
PHY_RawVisionData::envBits PHY_RawVisionData::GetVisionObject( double rX_, double rY_ ) const
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
const weather::Meteo::sWindData& PHY_RawVisionData::sCell::GetWind() const
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
