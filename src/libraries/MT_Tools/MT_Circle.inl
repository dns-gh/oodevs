//*****************************************************************************
//
// $Created: JDY 03-03-06 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_Circle.inl $
// $Author: Nld $
// $Modtime: 19/07/04 16:50 $
// $Revision: 3 $
// $Workfile: MT_Circle.inl $
//
//*****************************************************************************


//-----------------------------------------------------------------------------
// Name: MT_Circle::Center
// Created: JDY 03-03-06
//-----------------------------------------------------------------------------
inline
MT_Vector2D MT_Circle::Center() const
{
    return c_;
}



//-----------------------------------------------------------------------------
// Name: MT_Circle::Radius
// Created: JDY 03-03-06
//-----------------------------------------------------------------------------
inline
MT_Float MT_Circle::Radius() const
{
    return r_;
}


//-----------------------------------------------------------------------------
// Name: MT_Circle::IsTrivial
// Created: JDY 03-03-06
//-----------------------------------------------------------------------------
inline
bool MT_Circle::IsTrivial() const
{
    return (r_==0);
}


//-----------------------------------------------------------------------------
// Name: MT_Circle::Inside
// Created: JDY 03-03-06
//-----------------------------------------------------------------------------
inline
bool MT_Circle::Inside(const MT_Vector2D& p) const
{
    return c_.SquareDistance(p)<=(r_*r_);
}


//-----------------------------------------------------------------------------
// Name: MT_Circle::Outside
// Created: JDY 03-03-06
//-----------------------------------------------------------------------------
inline
bool MT_Circle::Outside(const MT_Vector2D& p) const
{
    return c_.SquareDistance(p)>=(r_*r_);
}


//-----------------------------------------------------------------------------
// Name: MT_Circle::Contains
// Created: JDY 03-03-06
//-----------------------------------------------------------------------------
inline
bool MT_Circle::Contains(const MT_Vector2D& p) const
{
    return c_.SquareDistance(p)==(r_*r_);
}


//-----------------------------------------------------------------------------
// Name: MT_Circle::operator =
// Created: JDY 03-03-06
//-----------------------------------------------------------------------------
inline
MT_Circle&  MT_Circle::operator =( const MT_Circle& c)
{
    c_=c.c_;
    r_=c.r_;
    return *this;
}

//-----------------------------------------------------------------------------
// Name: MT_Circle::operator =
// Created: JDY 03-03-06
//-----------------------------------------------------------------------------
inline
MT_Circle   MT_Circle::operator -( const MT_Vector2D& v )
{
    return MT_Circle(c_-v,r_);
}

//-----------------------------------------------------------------------------
// Name: MT_Circle::operator =
// Created: JDY 03-03-06
//-----------------------------------------------------------------------------
inline
MT_Circle   MT_Circle::operator +( const MT_Vector2D& v )
{
    return MT_Circle(c_+v,r_);
}

//-----------------------------------------------------------------------------
// Name: MT_Circle::operator =
// Created: JDY 03-03-06
//-----------------------------------------------------------------------------
inline
MT_Circle&  MT_Circle::operator +=( const MT_Vector2D& v )
{
    c_+=v;
    return *this;
}

//-----------------------------------------------------------------------------
// Name: MT_Circle::operator =
// Created: JDY 03-03-06
//-----------------------------------------------------------------------------
inline
MT_Circle&  MT_Circle::operator -=( const MT_Vector2D& v )
{
    c_-=v;
    return *this;
}