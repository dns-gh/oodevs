//*****************************************************************************
//
// $Created: JVT 04-02-26 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_Ellipse.inl $
// $Author: Age $
// $Modtime: 2/03/05 17:31 $
// $Revision: 6 $
// $Workfile: MT_Ellipse.inl $
//
//*****************************************************************************



//-----------------------------------------------------------------------------
// Name: MT_Ellipse::GetCenter
// Created: JVT 04-03-16
//-----------------------------------------------------------------------------
inline
const MT_Vector2D& MT_Ellipse::GetCenter() const
{
    return vCenter_;
}

// -----------------------------------------------------------------------------
// Name: MT_Ellipse::GetMajorAxeLength
// Created: NLD 2004-10-12
// -----------------------------------------------------------------------------
inline
MT_Float MT_Ellipse::GetMajorAxeLength() const
{
    return rMajorAxeLength_;
}

//-----------------------------------------------------------------------------
// Name: MT_Ellipse::GetMajorAxisHighPoint
// Created: JVT 04-03-25
//-----------------------------------------------------------------------------
inline
const MT_Vector2D MT_Ellipse::GetMajorAxisHighPoint()  const
{
    MT_Vector2D axis( cos( rTheta_ ), sin( rTheta_ ) );
    axis *= rMajorAxeLength_;
    return axis += vCenter_;
}


//-----------------------------------------------------------------------------
// Name: MT_Ellipse::GetMinorAxisHighPoint
// Created: JVT 04-03-25
//-----------------------------------------------------------------------------
inline
const MT_Vector2D MT_Ellipse::GetMinorAxisHighPoint()  const
{
    MT_Vector2D axis( -sin( rTheta_ ), cos( rTheta_ ) );
    axis *= rMinorAxeLength_;
    return axis += vCenter_;
}
