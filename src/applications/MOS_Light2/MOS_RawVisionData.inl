//*****************************************************************************
//
// $Created: JVT 02-11-05 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_RawVisionData.inl $
// $Author: Age $
// $Modtime: 8/02/05 15:52 $
// $Revision: 6 $
// $Workfile: MOS_RawVisionData.inl $
//
//*****************************************************************************

#include "MT_Tools/MT_ScipioException.h"
#include "MT_Tools/MT_LinearInterpolation.h"
#include "MT/MT_IO/MT_FormatString.h"


//-----------------------------------------------------------------------------
// Name: MOS_Meteo::sEphemeride
// Created: JVT 03-08-08
//-----------------------------------------------------------------------------
inline
const MOS_Meteo::sEphemeride& MOS_RawVisionData::GetEphemeride() const
{
    return ephemeride_;
}

//-----------------------------------------------------------------------------
// Name: MOS_RawVisionData::ConvertObjectIdxToEnvironnement
// Created: JVT 03-08-07
//-----------------------------------------------------------------------------
inline
MOS_RawVisionData::E_VisionObject MOS_RawVisionData::ConvertObjectIdxToEnvironnement( uint val )
{
    return (E_VisionObject)( val ? 1 << ( val - 1 ) : 0 );
}

//-----------------------------------------------------------------------------
// Name: MOS_RawVisionData::Initialize
// Created: JVT 02-11-05
// Last modified: JVT 02-11-20
//-----------------------------------------------------------------------------
inline
void MOS_RawVisionData::InitializeHeight( const std::string& strFile )
{
    assert( !ppCells_ );

    if ( !Read( strFile ) )
        throw MT_ScipioException( "MOS_RawVisionData::Initialize", __FILE__, __LINE__, MT_FormatString( "Can't read binary file '%s'", strFile.c_str() ) );
   
    CalcMinMaxAltitude();
}


//-----------------------------------------------------------------------------
// Name: MOS_RawVisionData::InitializeMeteo
// Created: JVT 03-08-08
//-----------------------------------------------------------------------------
inline
void MOS_RawVisionData::InitializeMeteo( const std::string& strFile )
{
    MOS_InputArchive archive;
    if ( !archive.Open( strFile ) || !archive.Section( "Meteo" ) )
        throw MT_ScipioException( "MOS_RawVisionData::Initialize", __FILE__, __LINE__, MT_FormatString( "Can't read xml file '%s'", strFile.c_str() ) );
    InitializeMeteo( archive );
    archive.EndSection();
}



//-----------------------------------------------------------------------------
// Name: MOS_RawVisionData::GetHeight
// Created: JVT 03-02-24
//-----------------------------------------------------------------------------
inline
MT_Float MOS_RawVisionData::GetHeight( MT_Float rX, MT_Float rY ) const
{
    uint nCol = GetCol( rX );
    uint nRow = GetRow( rY );

    MT_Float rScaledX = rX / rCellSize_;
    return MT_LinearInterpolation< MT_Float >() ( 
                 nRow, 
                 MT_LinearInterpolation< MT_Float >()( 
                    nCol, 
                    operator () ( nCol, nRow ).h,
                    nCol + 1,
                    operator () ( nCol + 1, nRow ).h,
                    rScaledX ),
                 nRow + 1,
                 MT_LinearInterpolation< MT_Float >()(
                    nCol,
                    operator () ( nCol, nRow + 1 ).h,
                    nCol + 1,
                    operator () ( nCol + 1, nRow + 1 ).h,
                    rScaledX ),
                 rY / rCellSize_ );
}

//-----------------------------------------------------------------------------
// Name: MOS_RawVisionData::GetHeight
// Created: JVT 02-11-28
// Last modified: JVT 03-02-24
//-----------------------------------------------------------------------------
inline
MT_Float MOS_RawVisionData::GetHeight( const MT_Vector2D& pos ) const
{
    return GetHeight( pos.rX_, pos.rY_ );
}



//-----------------------------------------------------------------------------
// Name: MOS_RawVisionData::GetVisionObjectHeight
// Created: JVT 04-05-27
//-----------------------------------------------------------------------------
inline
MT_Float MOS_RawVisionData::GetVisionObjectHeight( const MT_Vector2D& vPos ) const
{
    return operator () ( GetCol( vPos.rX_ ), GetRow( vPos.rY_ ) ).dh;
}



//-----------------------------------------------------------------------------
// Name: MOS_RawVisionData::sCell::operator ==
// Created: JVT 02-11-22
//-----------------------------------------------------------------------------
inline
bool MOS_RawVisionData::sCell::operator == ( const sCell& rhs ) const
{
    return h == rhs.h && dh == rhs.dh && e == rhs.e; 
}


//-----------------------------------------------------------------------------
// Name: MOS_RawVisionData::operator ()
// Created: JVT 02-11-05
//-----------------------------------------------------------------------------
inline
const MOS_RawVisionData::sCell& MOS_RawVisionData::operator () ( uint col, uint row ) const
{
    return ( col < nNbrCol_ && row < nNbrRow_ ) ? ppCells_[ col ][ row ] : emptyCell_;
}


//-----------------------------------------------------------------------------
// Name: MOS_RawVisionData::operator
// Created: JVT 02-11-15
//-----------------------------------------------------------------------------
inline
const MOS_RawVisionData::sCell& MOS_RawVisionData::operator() ( const MT_Vector2D& pos ) const
{
    return operator () ( GetCol( pos.rX_ ), GetRow( pos.rY_ ) );
}


//-----------------------------------------------------------------------------
// Name: MOS_RawVisionData::operator
// Created: JVT 02-11-15
//-----------------------------------------------------------------------------
inline
const MOS_RawVisionData::sCell& MOS_RawVisionData::operator() ( double x, double y ) const
{
    return operator () ( GetCol( x ), GetRow( y ) );
}


//-----------------------------------------------------------------------------
// Name: MOS_RawVisionData::GetCellSize
// Created: JVT 02-11-21
//-----------------------------------------------------------------------------
inline
double MOS_RawVisionData::GetCellSize() const
{
    return rCellSize_;
}

//-----------------------------------------------------------------------------
// Name: MOS_RawVisionData::GetMinAltitude
// Created: FBD 03-02-13
//-----------------------------------------------------------------------------
inline
MT_Float MOS_RawVisionData::GetMinAltitude() const
{
    return rMinAltitude_;
}


//-----------------------------------------------------------------------------
// Name: MOS_RawVisionData::GetMaxAltitude
// Created: FBD 03-02-13
//-----------------------------------------------------------------------------
inline
MT_Float MOS_RawVisionData::GetMaxAltitude() const
{
    return rMaxAltitude_;
}

