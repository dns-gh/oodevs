//*****************************************************************************
//
// $Created: FBD 02-11-07 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_Rect.inl $
// $Author: Nld $
// $Modtime: 1/09/04 16:44 $
// $Revision: 10 $
// $Workfile: MT_Rect.inl $
//
//*****************************************************************************


//-----------------------------------------------------------------------------
// Name: MT_Rect::GetPointUpLeft
// Created: NLD 2002-12-17
//-----------------------------------------------------------------------------
inline
const MT_Vector2D& MT_Rect::GetPointUpLeft() const
{
    return pointUpLeft_;
}

//-----------------------------------------------------------------------------
// Name: MT_Rect::GetPointUpRight
// Created: NLD 2002-12-17
//-----------------------------------------------------------------------------
inline
const MT_Vector2D& MT_Rect::GetPointUpRight() const
{
    return pointUpRight_;
}

//-----------------------------------------------------------------------------
// Name: MT_Rect::GetPointDwnRight
// Created: NLD 2002-12-17
//-----------------------------------------------------------------------------
inline
const MT_Vector2D& MT_Rect::GetPointDownRight() const
{
    return pointDownRight_;
}

//-----------------------------------------------------------------------------
// Name: MT_Rect::GetPointDownLeft
// Created: NLD 2002-12-17
//-----------------------------------------------------------------------------
inline
const MT_Vector2D& MT_Rect::GetPointDownLeft() const
{
    return pointDownLeft_;
}

//-----------------------------------------------------------------------------
//  Name  :  MT_Rect::GetTop
// Created: FBD 02-07-05
//-----------------------------------------------------------------------------
inline
MT_Float MT_Rect::GetTop() const
{
    return rPosY2_;
}

//-----------------------------------------------------------------------------
//  Name  :  MT_Rect::GetBottom
// Created: FBD 02-07-05
//-----------------------------------------------------------------------------
inline
MT_Float MT_Rect::GetBottom() const
{
    return rPosY1_;
}


//-----------------------------------------------------------------------------
//  Name  :  MT_Rect::GetLeft
// Created: FBD 02-07-05
//-----------------------------------------------------------------------------
inline
MT_Float MT_Rect::GetLeft() const
{
    return rPosX1_;
}


//-----------------------------------------------------------------------------
//  Name  :  MT_Rect::GetRight
// Created: FBD 02-07-05
//-----------------------------------------------------------------------------
inline
MT_Float MT_Rect::GetRight() const
{
    return rPosX2_;
}


//-----------------------------------------------------------------------------
// Name: MT_Rect::GetWidth
// Created: FBD 02-11-07
//-----------------------------------------------------------------------------
inline
MT_Float MT_Rect::GetWidth() const
{
    return rWidth_;
}


//-----------------------------------------------------------------------------
// Name: MT_Rect::GetHeight
// Created: FBD 02-11-07
//-----------------------------------------------------------------------------
inline
MT_Float MT_Rect::GetHeight() const
{
    return rHeight_;
}


//-----------------------------------------------------------------------------
// Name: MT_Rect::GetCenter
// Created: FBD 02-11-14
//-----------------------------------------------------------------------------
inline
const MT_Vector2D& MT_Rect::GetCenter() const
{
    return vCenter_;
}

//-----------------------------------------------------------------------------
// Name: MT_Rect::operator=
// Created: FBD 02-11-07
//-----------------------------------------------------------------------------
inline
MT_Rect& MT_Rect::operator=( const MT_Rect& rhs )
{
    rPosX1_         = rhs.rPosX1_;
    rPosY1_         = rhs.rPosY1_;
    rPosX2_         = rhs.rPosX2_;
    rPosY2_         = rhs.rPosY2_;
    rWidth_         = rPosX2_ - rPosX1_;
    rHeight_        = rPosY2_ - rPosY1_;
    vCenter_        = rhs.vCenter_;
    
    pointUpLeft_.rX_ = rPosX1_;
    pointUpLeft_.rY_ = rPosY1_;
    pointUpRight_.rX_ = rPosX2_;
    pointUpRight_.rY_ = rPosY1_;
    pointDownRight_.rX_ = rPosX2_;
    pointDownRight_.rY_ = rPosY2_;
    pointDownLeft_.rX_ = rPosX1_;
    pointDownLeft_.rY_ = rPosY2_;

    return *this;
}

//-----------------------------------------------------------------------------
// Name: MT_Rect::IsInside
// Created: FBD 02-11-07
//-----------------------------------------------------------------------------
inline
bool MT_Rect::IsInside( const MT_Vector2D& point ) const
{
    return( point.rX_ >= rPosX1_ && point.rX_ <= rPosX2_ && point.rY_ >= rPosY1_ && point.rY_ <= rPosY2_  );
}

//-----------------------------------------------------------------------------
// Name: MT_Rect::IsInside
// Created: NLD 2003-01-27
//-----------------------------------------------------------------------------
inline
bool MT_Rect::IsInside( const MT_Vector2D& point, MT_Float rPrecision ) const
{
    MT_Float rTmpX = point.rX_ - rPosX1_;
    if( rTmpX < 0 && MT_flabs( rTmpX ) >= rPrecision )
        return false;

    rTmpX = rPosX2_ - point.rX_;
    if( rTmpX < 0 && MT_flabs( rTmpX ) >= rPrecision )
        return false;

    MT_Float rTmpY = point.rY_ - rPosY1_;
    if( rTmpY < 0 && MT_flabs( rTmpY ) >= rPrecision )
        return false;

    rTmpY = rPosY2_ - point.rY_;
    if( rTmpY < 0 && MT_flabs( rTmpY ) >= rPrecision )
        return false;

    return true;
}


//-----------------------------------------------------------------------------
// Name: MT_Rect::GetLineLeft
// Created: FBD 03-03-07
//-----------------------------------------------------------------------------
inline
const MT_Line& MT_Rect::GetLineLeft() const
{
    return lineLeft_;
}


//-----------------------------------------------------------------------------
// Name: MT_Rect::GetLineRight
// Created: FBD 03-03-07
//-----------------------------------------------------------------------------
inline
const MT_Line& MT_Rect::GetLineRight() const
{
    return lineRight_;
}


//-----------------------------------------------------------------------------
// Name: MT_Rect::GetLineUp
// Created: FBD 03-03-07
//-----------------------------------------------------------------------------
inline
const MT_Line& MT_Rect::GetLineUp() const
{
    return lineUp_;
}


//-----------------------------------------------------------------------------
// Name: MT_Rect::GetLineDown
// Created: FBD 03-03-07
//-----------------------------------------------------------------------------
inline
const MT_Line& MT_Rect::GetLineDown() const
{
    return lineDown_;
}


//-----------------------------------------------------------------------------
// Name: MT_Rect::Intersect2D
// Created: JDY 03-05-20
//-----------------------------------------------------------------------------
inline
bool MT_Rect::Intersect2D( const MT_Rect& rect ) const
{
    return Intersect2D(MT_Line(rect.pointUpLeft_,rect.pointUpRight_) )
        || Intersect2D(MT_Line(rect.pointUpRight_,rect.pointDownRight_) )
        || Intersect2D(MT_Line(rect.pointDownRight_,rect.pointDownLeft_) )
        || Intersect2D(MT_Line(rect.pointDownLeft_,rect.pointUpLeft_) );
}