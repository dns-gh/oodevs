// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2002 MASA Group
//
// *****************************************************************************

#include "MT_Tools/MT_LinearInterpolation.h"

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::operator ()
// Created: JVT 02-11-05
//-----------------------------------------------------------------------------
inline
const PHY_RawVisionData::sCell& PHY_RawVisionData::operator () ( unsigned int col, unsigned int row ) const
{
    return ( col < nNbrCol_ && row < nNbrRow_ ) ? ppCells_[ col ][ row ] : emptyCell_;
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::operator
// Created: JVT 02-11-15
//-----------------------------------------------------------------------------
inline
const PHY_RawVisionData::sCell& PHY_RawVisionData::operator() ( const MT_Vector2D& pos ) const
{
    return operator () ( GetCol( pos.rX_ ), GetRow( pos.rY_ ) );
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::operator
// Created: JVT 02-11-15
//-----------------------------------------------------------------------------
inline
const PHY_RawVisionData::sCell& PHY_RawVisionData::operator() ( double x, double y ) const
{
    return operator () ( GetCol( x ), GetRow( y ) );
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::operator
// Created: JVT 04-03-24
//-----------------------------------------------------------------------------
inline
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
inline
const weather::PHY_Precipitation& PHY_RawVisionData::GetPrecipitation( const MT_Vector2D& vPos ) const
{
    return operator() ( vPos ).GetPrecipitation();    
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::GetAltitude
// Created: JVT 03-02-24
//-----------------------------------------------------------------------------
inline
double PHY_RawVisionData::GetAltitude( double rX, double rY ) const
{
    unsigned int nCol = GetCol( rX );
    unsigned int nRow = GetRow( rY );

    double rScaledX = rX / rCellSize_;

    return MT_LinearInterpolation< double >() ( 
                 nRow, 
                 MT_LinearInterpolation< double >()( 
                    nCol, 
                    operator () ( nCol, nRow ).GetAltitude(),
                    nCol + 1,
                    operator () ( nCol + 1, nRow ).GetAltitude(),
                    rScaledX ),
                 nRow + 1,
                 MT_LinearInterpolation< double >()(
                    nCol,
                    operator () ( nCol, nRow + 1 ).GetAltitude(),
                    nCol + 1,
                    operator () ( nCol + 1, nRow + 1 ).GetAltitude(),
                    rScaledX ),
                 rY / rCellSize_ );
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::GetAltitude
// Created: JVT 02-11-28
// Last modified: JVT 03-02-24
//-----------------------------------------------------------------------------
inline
double PHY_RawVisionData::GetAltitude( const MT_Vector2D& pos ) const
{
    return GetAltitude( pos.rX_, pos.rY_ );
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::GetVisionObject
// Created: JVT 03-07-04
//-----------------------------------------------------------------------------
inline
PHY_RawVisionData::envBits PHY_RawVisionData::GetVisionObject( const MT_Vector2D& pos ) const
{
    return operator() ( pos ).GetEnv();
}

// -----------------------------------------------------------------------------
// Name: PHY_RawVisionData::GetWind
// Created: JVT 2004-10-29
// -----------------------------------------------------------------------------
inline
const weather::PHY_Meteo::sWindData& PHY_RawVisionData::GetWind( const MT_Vector2D& vPos ) const
{
    return operator() ( vPos ).GetWind();    
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::GetVisionObject
// Created: JVT 03-07-04
//-----------------------------------------------------------------------------
inline
PHY_RawVisionData::envBits PHY_RawVisionData::GetVisionObject( double rX_, double rY_ ) const
{
    return operator () ( rX_, rY_ ).GetEnv();
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::Getcol
// Created: JVT 02-11-05
//-----------------------------------------------------------------------------
inline
unsigned int PHY_RawVisionData::GetCol( double x ) const
{
    return static_cast< unsigned int >( x / rCellSize_ );
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::Getrow
// Created: JVT 02-11-05
//-----------------------------------------------------------------------------
inline
unsigned int PHY_RawVisionData::GetRow( double y ) const
{
    return static_cast< unsigned int >( y / rCellSize_);
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::GetCellSize
// Created: JVT 02-11-21
//-----------------------------------------------------------------------------
inline
double PHY_RawVisionData::GetCellSize() const
{
    return rCellSize_;
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::GetMinAltitude
// Created: FBD 03-02-13
//-----------------------------------------------------------------------------
inline
double PHY_RawVisionData::GetMinAltitude() const
{
    return rMinAltitude_;
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::GetMaxAltitude
// Created: FBD 03-02-13
//-----------------------------------------------------------------------------
inline
double PHY_RawVisionData::GetMaxAltitude() const
{
    return rMaxAltitude_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RawVisionData::GetWind
// Created: JVT 2004-10-29
// -----------------------------------------------------------------------------
inline
const weather::PHY_Meteo::sWindData& PHY_RawVisionData::sCell::GetWind() const
{
    return pMeteo ? pMeteo->GetWind() : pGlobalMeteo_->GetWind();
}

// -----------------------------------------------------------------------------
// Name: PHY_RawVisionData::GetVisionObjectsInSurface
// Created: NLD 2004-11-17
// -----------------------------------------------------------------------------
template< typename T > inline
void PHY_RawVisionData::GetVisionObjectsInSurface( const T& localisation, unsigned int& nEmptySurface, unsigned int& nForestSurface, unsigned int& nUrbanSurface ) const
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
