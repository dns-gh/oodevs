//*****************************************************************************
//
// $Created: JVT 02-11-05 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Meteo/RawVisionData/PHY_RawVisionData.inl $
// $Author: Jvt $
// $Modtime: 29/10/04 10:43 $
// $Revision: 2 $
// $Workfile: PHY_RawVisionData.inl $
//
//*****************************************************************************

#include "MT_Tools/MT_LinearInterpolation.h"

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::operator ()
// Created: JVT 02-11-05
//-----------------------------------------------------------------------------
inline
const PHY_RawVisionData::sCell& PHY_RawVisionData::operator () ( uint col, uint row ) const
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
    uint nCol = GetCol( rCol );
    uint nRow = GetRow( rRow );

    return ( nCol < nNbrCol_ && nRow < nNbrRow_ ) ? ppCells_[ nCol ][ nRow ] : emptyCell_;
}


//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::GetAltitude
// Created: JVT 03-02-24
//-----------------------------------------------------------------------------
inline
MT_Float PHY_RawVisionData::GetAltitude( MT_Float rX, MT_Float rY ) const
{
    uint nCol = GetCol( rX );
    uint nRow = GetRow( rY );

    MT_Float rScaledX = rX / rCellSize_;

    return MT_LinearInterpolation< MT_Float >() ( 
                 nRow, 
                 MT_LinearInterpolation< MT_Float >()( 
                    nCol, 
                    operator () ( nCol, nRow ).GetAltitude(),
                    nCol + 1,
                    operator () ( nCol + 1, nRow ).GetAltitude(),
                    rScaledX ),
                 nRow + 1,
                 MT_LinearInterpolation< MT_Float >()(
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
MT_Float PHY_RawVisionData::GetAltitude( const MT_Vector2D& pos ) const
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
const PHY_Meteo::sWindData& PHY_RawVisionData::GetWind( const MT_Vector2D& vPos ) const
{
    return operator() ( vPos ).GetWind();    
}


//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::GetVisionObject
// Created: JVT 03-07-04
//-----------------------------------------------------------------------------
inline
PHY_RawVisionData::envBits PHY_RawVisionData::GetVisionObject( MT_Float rX_, MT_Float rY_ ) const
{
    return operator () ( rX_, rY_ ).GetEnv();
}


//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::Getcol
// Created: JVT 02-11-05
//-----------------------------------------------------------------------------
inline
uint PHY_RawVisionData::GetCol( double x ) const
{
    return (uint)( x / rCellSize_ );
}


//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::Getrow
// Created: JVT 02-11-05
//-----------------------------------------------------------------------------
inline
uint PHY_RawVisionData::GetRow( double y ) const
{
    return (uint)( y / rCellSize_);
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
MT_Float PHY_RawVisionData::GetMinAltitude() const
{
    return rMinAltitude_;
}


//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::GetMaxAltitude
// Created: FBD 03-02-13
//-----------------------------------------------------------------------------
inline
MT_Float PHY_RawVisionData::GetMaxAltitude() const
{
    return rMaxAltitude_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RawVisionData::GetWind
// Created: JVT 2004-10-29
// -----------------------------------------------------------------------------
inline
const PHY_Meteo::sWindData& PHY_RawVisionData::sCell::GetWind() const
{
    return pMeteo ? pMeteo->GetWind() : pGlobalMeteo_->GetWind();
}
