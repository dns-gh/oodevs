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
#include "MT_Tools/MT_ScipioException.h"
#include "MT_Tools/MT_FormatString.h"
#include "MT_Tools/MT_Ellipse.h"
#include "MT_Tools/MT_Logger.h"
#include "simulation_terrain/TER_Localisation.h"
#include "tools/InputBinaryStream.h"
#include "tools/WorldParameters.h"

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
PHY_RawVisionData::PHY_RawVisionData( weather::Meteo& globalMeteo, tools::WorldParameters& config )
    : ppCells_( 0 )
    , nNbrCol_( 0 )
    , nNbrRow_( 0 )
{
    MT_LOG_INFO_MSG( "Initializing vision data" );
    MIL_AgentServer::GetWorkspace().GetConfig().AddFileToCRC( config.detection_ );
    Read( config.detection_ );
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
void PHY_RawVisionData::UnregisterMeteoPatch( const geometry::Point2d& upLeft, const geometry::Point2d& downRight )
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
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Cannot open file %s", strFile.c_str() ) );

    if( !( archive >> rCellSize_ >> nNbrRow_ >> nNbrCol_ ) )
       throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Error reading file %s", strFile.c_str() ) );

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
                throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Error reading file %s", strFile.c_str() ) );
        }
    }
    CalcMinMaxAltitude();
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RawVisionData::ModifyAltitude
// Created: JSR 2011-05-19
// -----------------------------------------------------------------------------
void PHY_RawVisionData::ModifyAltitude( const TER_Localisation& localisation, short heightOffset )
{
    const MT_Rect boundingBox = localisation.GetBoundingBox();
    unsigned int startCol = GetCol( boundingBox.GetLeft() );
    unsigned int endCol = GetCol( boundingBox.GetRight() );
    unsigned int startRow = GetCol( boundingBox.GetBottom() );
    unsigned int endRow = GetCol( boundingBox.GetTop() );
    for( unsigned int y = startRow; y <= endRow + 1 && y < nNbrRow_; ++y )
        for( unsigned int x = startCol; x <= endCol + 1 && x < nNbrCol_; ++x )
            if( localisation.IsInside( MT_Vector2D( x * rCellSize_, y * rCellSize_ ) ) )
            {
                int newHeight = static_cast< int >( ppCells_[ x ][ y ].h ) + heightOffset;
                static const int maxShort = static_cast< int >( std::numeric_limits< short >::max() );
                ppCells_[ x ][ y ].h = static_cast< short >( std::min( newHeight, maxShort ) );
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
