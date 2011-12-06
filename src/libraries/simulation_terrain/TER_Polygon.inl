//*****************************************************************************
//
// $Created: JDY 03-05-19 $
// $Archive: /MVW_v10/Build/SDK/TER/src/TER_Polygon.inl $
// $Author: Age $
// $Modtime: 14/06/05 15:22 $
// $Revision: 2 $
// $Workfile: TER_Polygon.inl $
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// Name: TER_Polygon::GetBorderPoints
// Created: JDY 03-05-21
//-----------------------------------------------------------------------------

const T_PointVector& TER_Polygon::GetBorderPoints() const
{
    assert(pData_);
    return pData_->borderVector_;
}

//-----------------------------------------------------------------------------
// Name: TER_Polygon::Reset
// Created: JDY 03-05-19
//-----------------------------------------------------------------------------

void TER_Polygon::Reset(const T_PointPtrVector& points,bool bConvexHull )
{
    *this=TER_Polygon( points, bConvexHull );
}

//-----------------------------------------------------------------------------
// Name: TER_Polygon::Reset
// Created: NLD 2003-07-25
//-----------------------------------------------------------------------------

void TER_Polygon::Reset( const T_PointVector& points, bool bConvexHull )
{
    *this=TER_Polygon( points, bConvexHull ); //$$ c'est de la merde !
}

//-----------------------------------------------------------------------------
// Name: TER_Polygon::Reset
// Created: JDY 03-05-19
//-----------------------------------------------------------------------------
void TER_Polygon::Reset()
{
    *this=*empty_polygon;
}

//-----------------------------------------------------------------------------
// Name: TER_Polygon::IsNull
// Created: JDY 03-05-19
//-----------------------------------------------------------------------------
bool TER_Polygon::IsNull() const
{
    return pData_->bIsNull_;
}

// -----------------------------------------------------------------------------
// Name: TER_Polygon::GetArea
// Created: NLD 2003-10-17
// -----------------------------------------------------------------------------

double TER_Polygon::GetArea() const
{
    if( pData_->rArea_ == -1 )
        pData_->rArea_ = ComputeArea();
    return pData_->rArea_;
}
