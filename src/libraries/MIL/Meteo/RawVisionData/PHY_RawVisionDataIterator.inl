//*****************************************************************************
//
// $Created: JVT 03-03-29 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Meteo/RawVisionData/PHY_RawVisionDataIterator.inl $
// $Author: Nld $
// $Modtime: 19/10/04 13:54 $
// $Revision: 1 $
// $Workfile: PHY_RawVisionDataIterator.inl $
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionDataIterator::GetCurrentEnv
// Created: JVT 03-04-02
//-----------------------------------------------------------------------------
inline
PHY_RawVisionData::envBits PHY_RawVisionDataIterator::GetCurrentEnv() const
{
    assert( pCurrentCell_ );
    if( bIsInGround_ )
        return (PHY_RawVisionData::envBits)( pCurrentCell_->GetEnv() | PHY_RawVisionData::eVisionGround );
    else if( bIsInEnv_ )
        return pCurrentCell_->GetEnv();
    else 
        return (PHY_RawVisionData::envBits)PHY_RawVisionData::eVisionEmpty;
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionDataIterator::GetLighting
// Created: JVT 04-03-24
//-----------------------------------------------------------------------------
inline
const PHY_Lighting& PHY_RawVisionDataIterator::GetLighting() const
{
    return pCurrentCell_->GetLighting();
}


//-----------------------------------------------------------------------------
// Name: PHY_RawVisionDataIterator::GetPrecipitation
// Created: JVT 04-03-24
//-----------------------------------------------------------------------------
inline
const PHY_Precipitation& PHY_RawVisionDataIterator::GetPrecipitation() const
{
    return pCurrentCell_->GetPrecipitation();
}



//-----------------------------------------------------------------------------
// Name: PHY_RawVisionDataIterator
// Created: JVT 03-04-02
//-----------------------------------------------------------------------------
inline
MT_Float PHY_RawVisionDataIterator::Length() const
{
    return rLenght_;
}


//-----------------------------------------------------------------------------
// Name: PHY_RawVisionDataIterator::End
// Created: JVT 03-04-02
//-----------------------------------------------------------------------------
extern MT_Float rRemainingLength_;
inline
bool PHY_RawVisionDataIterator::End() const
{
    return eIteratorState_ == eEnd;
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionDataIterator::ToRealSpace
// Created: JVT 03-03-29
//-----------------------------------------------------------------------------
inline
void PHY_RawVisionDataIterator::ToRealSpace( MT_Float& rX, MT_Float& rY ) const
{
    if ( bNegX_ ) rX = -rX;
    if ( bNegY_ ) rY = -rY;
    if ( bSwap_ ) std::swap( rX, rY );
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionDataIterator::ToRealSpace
// Created: JVT 03-03-30
//-----------------------------------------------------------------------------
inline
void PHY_RawVisionDataIterator::ToRealSpace( int& nX, int& nY ) const
{
    if ( bNegX_ ) nX = -nX;
    if ( bNegY_ ) nY = -nY;
    if ( bSwap_ ) std::swap( nX, nY );
}


//-----------------------------------------------------------------------------
// Name: PHY_RawVisionDataIterator::ToRealSpace
// Created: JVT 03-06-23
// Last modified: JVT 03-06-24
//-----------------------------------------------------------------------------
inline
void PHY_RawVisionDataIterator::OffsetToRealSpace( int& dX, int& dY ) const
{
    bSwapOffset_ ? ToRealSpace( dY, dX ) : ToRealSpace( dX, dY );
}



//-----------------------------------------------------------------------------
// Name: PHY_RawVisionDataIterator::ToAlgorithmSpace
// Created: JVT 03-03-29
//-----------------------------------------------------------------------------
inline
void PHY_RawVisionDataIterator::ToAlgorithmSpace( MT_Float& rX, MT_Float& rY ) const
{
    if ( bSwap_ ) std::swap( rX, rY );
    if ( bNegX_ ) rX = -rX;
    if ( bNegY_ ) rY = -rY;
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionDataIterator::ToAlgorithmSpace
// Created: JVT 03-03-29
//-----------------------------------------------------------------------------
inline
void PHY_RawVisionDataIterator::ToAlgorithmSpace( int& nX, int& nY ) const
{
    if ( bSwap_ ) std::swap( nX, nY );
    if ( bNegX_ ) nX = -nX;
    if ( bNegY_ ) nY = -nY;
}


//-----------------------------------------------------------------------------
// Name: PHY_RawVisionDataIterator::GetPos
// Created: JVT 03-06-24
//-----------------------------------------------------------------------------
inline
const MT_Vector2D PHY_RawVisionDataIterator::GetPos() const
{
    MT_Vector2D res( vOutPoint_.rX_ + nCellColOffset_, vOutPoint_.rY_ + nCellRowOffset_ );
    ToRealSpace( res.rX_, res.rY_ );
    res *= data_.GetCellSize();
    return res;
}


//-----------------------------------------------------------------------------
// Name: PHY_RawVisionDataIterator::GetPos
// Created: JVT 03-06-24
//-----------------------------------------------------------------------------
inline
void PHY_RawVisionDataIterator::GetPos( MT_Vector2D& res ) const
{
    res.rX_ = vOutPoint_.rX_ + nCellColOffset_;
    res.rY_ = vOutPoint_.rY_ + nCellRowOffset_;
    ToRealSpace( res.rX_, res.rY_ );
    res *= data_.GetCellSize();
}

