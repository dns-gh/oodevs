//*****************************************************************************
//
// $Created: JVT 03-04-28 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/RawVisionDataIterator.inl $
// $Author: Ape $
// $Modtime: 4/11/04 11:42 $
// $Revision: 3 $
// $Workfile: RawVisionDataIterator.inl $
//
//*****************************************************************************


//-----------------------------------------------------------------------------
// Name: RawVisionData::Iterator::GetMeteo
// Created: JVT 03-08-08
//-----------------------------------------------------------------------------
inline
const Meteo& RawVisionData::Iterator::GetMeteo() const
{
    assert( pCurrentMeteo_ );
    return *pCurrentMeteo_;
}



//-----------------------------------------------------------------------------
// Name: RawVisionData::Iterator::GetCurrentEnv
// Created: JVT 03-04-02
//-----------------------------------------------------------------------------
inline
RawVisionData::envBits RawVisionData::Iterator::GetCurrentEnv() const
{
    if ( bIsInGround_ )
        return nCurrentEnv_ | RawVisionData::eVisionGround;
    return nCurrentEnv_;
}


//-----------------------------------------------------------------------------
// Name: RawVisionData::Iterator
// Created: JVT 03-04-02
//-----------------------------------------------------------------------------
inline
MT_Float RawVisionData::Iterator::Length() const
{
    return rLenght_;
}


//-----------------------------------------------------------------------------
// Name: RawVisionData::Iterator::End
// Created: JVT 03-04-02
//-----------------------------------------------------------------------------
extern MT_Float rRemainingLength_;
inline
bool RawVisionData::Iterator::End() const
{
    return eIteratorState_ == eEnd;
}

//-----------------------------------------------------------------------------
// Name: RawVisionData::Iterator::ToRealSpace
// Created: JVT 03-03-29
//-----------------------------------------------------------------------------
inline
void RawVisionData::Iterator::ToRealSpace( MT_Float& rX, MT_Float& rY ) const
{
    if ( bNegX_ ) rX = -rX;
    if ( bNegY_ ) rY = -rY;
    if ( bSwap_ ) std::swap( rX, rY );
}

//-----------------------------------------------------------------------------
// Name: RawVisionData::Iterator::ToRealSpace
// Created: JVT 03-03-30
//-----------------------------------------------------------------------------
inline
void RawVisionData::Iterator::ToRealSpace( int& nX, int& nY ) const
{
    if ( bNegX_ ) nX = -nX;
    if ( bNegY_ ) nY = -nY;
    if ( bSwap_ ) std::swap( nX, nY );
}


//-----------------------------------------------------------------------------
// Name: RawVisionData::Iterator::ToRealSpace
// Created: JVT 03-06-23
// Last modified: JVT 03-06-24
//-----------------------------------------------------------------------------
inline
void RawVisionData::Iterator::OffsetToRealSpace( int& dX, int& dY ) const
{
    bSwapOffset_ ? ToRealSpace( dY, dX ) : ToRealSpace( dX, dY );
}



//-----------------------------------------------------------------------------
// Name: RawVisionData::Iterator::ToAlgorithmSpace
// Created: JVT 03-03-29
//-----------------------------------------------------------------------------
inline
void RawVisionData::Iterator::ToAlgorithmSpace( MT_Float& rX, MT_Float& rY ) const
{
    if ( bSwap_ ) std::swap( rX, rY );
    if ( bNegX_ ) rX = -rX;
    if ( bNegY_ ) rY = -rY;
}

//-----------------------------------------------------------------------------
// Name: RawVisionData::Iterator::ToAlgorithmSpace
// Created: JVT 03-03-29
//-----------------------------------------------------------------------------
inline
void RawVisionData::Iterator::ToAlgorithmSpace( int& nX, int& nY ) const
{
    if ( bSwap_ ) std::swap( nX, nY );
    if ( bNegX_ ) nX = -nX;
    if ( bNegY_ ) nY = -nY;
}


//-----------------------------------------------------------------------------
// Name: RawVisionData::Iterator::GetPos
// Created: JVT 03-06-24
//-----------------------------------------------------------------------------
inline
const MT_Vector2D RawVisionData::Iterator::GetPos() const
{
    MT_Vector2D res( vOutPoint_.rX_ + nCellColOffset_, vOutPoint_.rY_ + nCellRowOffset_ );
    ToRealSpace( res.rX_, res.rY_ );
    res *= data_.GetCellSize();
    return res;
}


//-----------------------------------------------------------------------------
// Name: RawVisionData::Iterator::GetPos
// Created: JVT 03-06-24
//-----------------------------------------------------------------------------
inline
void RawVisionData::Iterator::GetPos( MT_Vector2D& res ) const
{
    res.rX_ = vOutPoint_.rX_ + nCellColOffset_;
    res.rY_ = vOutPoint_.rY_ + nCellRowOffset_;
    ToRealSpace( res.rX_, res.rY_ );
    res *= data_.GetCellSize();
}
