//*****************************************************************************
//
// $Created: JVT 03-04-28 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_RawVisionDataIterator.inl $
// $Author: Nld $
// $Modtime: 27/09/04 18:16 $
// $Revision: 4 $
// $Workfile: MOS_RawVisionDataIterator.inl $
//
//*****************************************************************************


//-----------------------------------------------------------------------------
// Name: MOS_RawVisionData::Iterator::GetMeteo
// Created: JVT 03-08-08
//-----------------------------------------------------------------------------
inline
const MOS_Meteo& MOS_RawVisionData::Iterator::GetMeteo() const
{
    assert( pCurrentMeteo_ );
    return *pCurrentMeteo_;
}



//-----------------------------------------------------------------------------
// Name: MOS_RawVisionData::Iterator::GetCurrentEnv
// Created: JVT 03-04-02
//-----------------------------------------------------------------------------
inline
MOS_RawVisionData::envBits MOS_RawVisionData::Iterator::GetCurrentEnv() const
{
    if ( bIsInGround_ )
        return nCurrentEnv_ | MOS_RawVisionData::eVisionGround;
    return nCurrentEnv_;
}


//-----------------------------------------------------------------------------
// Name: MOS_RawVisionData::Iterator
// Created: JVT 03-04-02
//-----------------------------------------------------------------------------
inline
MT_Float MOS_RawVisionData::Iterator::Length() const
{
    return rLenght_;
}


//-----------------------------------------------------------------------------
// Name: MOS_RawVisionData::Iterator::End
// Created: JVT 03-04-02
//-----------------------------------------------------------------------------
extern MT_Float rRemainingLength_;
inline
bool MOS_RawVisionData::Iterator::End() const
{
    return eIteratorState_ == eEnd;
}

//-----------------------------------------------------------------------------
// Name: MOS_RawVisionData::Iterator::ToRealSpace
// Created: JVT 03-03-29
//-----------------------------------------------------------------------------
inline
void MOS_RawVisionData::Iterator::ToRealSpace( MT_Float& rX, MT_Float& rY ) const
{
    if ( bNegX_ ) rX = -rX;
    if ( bNegY_ ) rY = -rY;
    if ( bSwap_ ) std::swap( rX, rY );
}

//-----------------------------------------------------------------------------
// Name: MOS_RawVisionData::Iterator::ToRealSpace
// Created: JVT 03-03-30
//-----------------------------------------------------------------------------
inline
void MOS_RawVisionData::Iterator::ToRealSpace( int& nX, int& nY ) const
{
    if ( bNegX_ ) nX = -nX;
    if ( bNegY_ ) nY = -nY;
    if ( bSwap_ ) std::swap( nX, nY );
}


//-----------------------------------------------------------------------------
// Name: MOS_RawVisionData::Iterator::ToRealSpace
// Created: JVT 03-06-23
// Last modified: JVT 03-06-24
//-----------------------------------------------------------------------------
inline
void MOS_RawVisionData::Iterator::OffsetToRealSpace( int& dX, int& dY ) const
{
    bSwapOffset_ ? ToRealSpace( dY, dX ) : ToRealSpace( dX, dY );
}



//-----------------------------------------------------------------------------
// Name: MOS_RawVisionData::Iterator::ToAlgorithmSpace
// Created: JVT 03-03-29
//-----------------------------------------------------------------------------
inline
void MOS_RawVisionData::Iterator::ToAlgorithmSpace( MT_Float& rX, MT_Float& rY ) const
{
    if ( bSwap_ ) std::swap( rX, rY );
    if ( bNegX_ ) rX = -rX;
    if ( bNegY_ ) rY = -rY;
}

//-----------------------------------------------------------------------------
// Name: MOS_RawVisionData::Iterator::ToAlgorithmSpace
// Created: JVT 03-03-29
//-----------------------------------------------------------------------------
inline
void MOS_RawVisionData::Iterator::ToAlgorithmSpace( int& nX, int& nY ) const
{
    if ( bSwap_ ) std::swap( nX, nY );
    if ( bNegX_ ) nX = -nX;
    if ( bNegY_ ) nY = -nY;
}


//-----------------------------------------------------------------------------
// Name: MOS_RawVisionData::Iterator::GetPos
// Created: JVT 03-06-24
//-----------------------------------------------------------------------------
inline
const MT_Vector2D MOS_RawVisionData::Iterator::GetPos() const
{
    MT_Vector2D res( vOutPoint_.rX_ + nCellColOffset_, vOutPoint_.rY_ + nCellRowOffset_ );
    ToRealSpace( res.rX_, res.rY_ );
    res *= data_.GetCellSize();
    return res;
}


//-----------------------------------------------------------------------------
// Name: MOS_RawVisionData::Iterator::GetPos
// Created: JVT 03-06-24
//-----------------------------------------------------------------------------
inline
void MOS_RawVisionData::Iterator::GetPos( MT_Vector2D& res ) const
{
    res.rX_ = vOutPoint_.rX_ + nCellColOffset_;
    res.rY_ = vOutPoint_.rY_ + nCellRowOffset_;
    ToRealSpace( res.rX_, res.rY_ );
    res *= data_.GetCellSize();
}
