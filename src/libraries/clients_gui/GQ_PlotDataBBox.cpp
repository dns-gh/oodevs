// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "GQ_PlotDataBBox.h"

#include <cmath>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: GQ_PlotDataBBox constructor
// Created: CBX 2003-08-08
// -----------------------------------------------------------------------------
GQ_PlotDataBBox::GQ_PlotDataBBox()
: bEmpty_( true )
, rXMin_ ( 0.0 )
, rXMax_ ( 0.0 )
, rYMin_ ( 0.0 )
, rYMax_ ( 0.0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotDataBBox::operator=
/** @param  rhs
    @return
*/
// Created: CBX 2003-08-08
// -----------------------------------------------------------------------------
GQ_PlotDataBBox& GQ_PlotDataBBox::operator=( const GQ_PlotDataBBox& bbox )
{
    if( &bbox == this )
        return *this;

    rXMin_ = bbox.rXMin_;
    rXMax_ = bbox.rXMax_;

    rYMin_ = bbox.rYMin_;
    rYMax_ = bbox.rYMax_;

    bEmpty_ = bbox.bEmpty_;

    return *this;
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotDataBBox::operator==
/** @param  GQ_PlotDataBBox&
    @return
*/
// Created: CBX 2003-08-08
// -----------------------------------------------------------------------------
bool GQ_PlotDataBBox::operator==( const GQ_PlotDataBBox& bbox ) const
{
    if( &bbox == this )
        return true;

    if( bbox.bEmpty_ != bEmpty_ )
        return false;

    if( bbox.rXMin_  != rXMin_ )
        return false;

    if( bbox.rXMax_  != rXMax_ )
        return false;

    if( bbox.rYMin_  != rYMin_ )
        return false;

    if( bbox.rYMax_  != rYMax_ )
        return false;

    return true;
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotDataBBox::Reset
// Created: CBX 2003-08-08
// -----------------------------------------------------------------------------
bool GQ_PlotDataBBox::Reset()
{
    if( bEmpty_ )
        return false;

    bEmpty_ = true;
    rXMin_  = 0.0;
    rXMax_  = 0.0;
    rYMin_  = 0.0;
    rYMax_  = 0.0;

    return true;
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotDataBBox::SetBBox
/** @param  GQ_PlotDataBBox&
    @return
*/
// Created: CBX 2003-08-08
// -----------------------------------------------------------------------------
bool GQ_PlotDataBBox::SetBBox( const GQ_PlotDataBBox& bbox )
{
    if( bbox == *this )
        return false;

    *this = bbox;
    return true;
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotDataBBox::UpdateWithBBox
/** @param  GQ_PlotDataBBox&
    @return
*/
// Created: CBX 2003-08-08
// -----------------------------------------------------------------------------
bool GQ_PlotDataBBox::UpdateWithBBox( const GQ_PlotDataBBox& bbox )
{
    if( bbox.IsEmpty() )
        return false;

    if( bEmpty_ )
    {
        *this = bbox;
        return true;
    }

    bool bTouched = false;
    if( bbox.rXMin_ < rXMin_ )
    {
        rXMin_ = bbox.rXMin_;
        bTouched = true;
    }
    if( bbox.rXMax_ > rXMax_ )
    {
        rXMax_ = bbox.rXMax_;
        bTouched = true;
    }

    if( bbox.rYMin_ < rYMin_ )
    {
        rYMin_ = bbox.rYMin_;
        bTouched = true;
    }
    if( bbox.rYMax_ > rYMax_ )
    {
        rYMax_ = bbox.rYMax_;
        bTouched = true;
    }

    bEmpty_ = false;
    return bTouched;
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotDataBBox::UpdateWithPoint
/** @param  T_Point&
    @return
*/
// Created: CBX 2003-08-08
// -----------------------------------------------------------------------------
bool GQ_PlotDataBBox::UpdateWithPoint( const T_Point& point )
{
    double rX = point.first;
    double rY = point.second;

    if( bEmpty_ )
    {
        rXMin_ = rX;
        rXMax_ = rX;

        rYMin_ = rY;
        rYMax_ = rY;

        bEmpty_ = false;
        return true;
    }

    bool bTouched = false;
    if( rX < rXMin_ )
    {
        rXMin_ = rX;
        bTouched = true;
    }
    else if( rX > rXMax_ )
    {
        rXMax_ = rX;
        bTouched = true;
    }

    if( rY < rYMin_ )
    {
        rYMin_ = rY;
        bTouched = true;
    }
    else if( rY > rYMax_ )
    {
        rYMax_ = rY;
        bTouched = true;
    }

    bEmpty_ = false;
    return bTouched;
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotDataBBox::AddPoint
/** @param  T_Point&
*/
// Created: CBX 2003-08-08
// -----------------------------------------------------------------------------
void GQ_PlotDataBBox::AddPoint( const T_Point& point )
{
    double rX = point.first;
    double rY = point.second;

    if( bEmpty_ )
    {
        rXMin_ = rX;
        rXMax_ = rX;

        rYMin_ = rY;
        rYMax_ = rY;

        bEmpty_ = false;
        return;
    }

    if( rX < rXMin_ )
        rXMin_ = rX;
    else if( rX > rXMax_ )
        rXMax_ = rX;

    if( rY < rYMin_ )
        rYMin_ = rY;
    else if( rY > rYMax_ )
        rYMax_ = rY;

    bEmpty_ = false;
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotDataBBox::Includes
/** @param  T_Point&
    @return
*/
// Created: CBX 2003-08-18
// -----------------------------------------------------------------------------
bool GQ_PlotDataBBox::Includes( const T_Point& point ) const
{
    if( bEmpty_ )
        return false;

    double rX = point.first;
    double rY = point.second;

    return ( rXMin_ <= rX && rX <= rXMax_
          && rYMin_ <= rY && rY <= rYMax_ );
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotDataBBox::Includes
/** @param  GQ_PlotDataBBox&
    @return
*/
// Created: CBX 2003-08-18
// -----------------------------------------------------------------------------
bool GQ_PlotDataBBox::Includes( const GQ_PlotDataBBox& bbox ) const
{
    if( bEmpty_ )
        return false;

    return ( rXMin_ <= bbox.XMin() && bbox.XMax() <= rXMax_
          && rYMin_ <= bbox.YMin() && bbox.YMax() <= rYMax_ );
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotDataBBox::Intersects
/** @return
*/
// Created: CBX 2004-03-08
// -----------------------------------------------------------------------------
bool GQ_PlotDataBBox::Intersects( const GQ_PlotDataBBox& bbox ) const
{
    if( bEmpty_ || bbox.IsEmpty() )
        return false;

    return !( bbox.XMin() > XMax() || bbox.XMax() < XMin()
           || bbox.YMin() > YMax() || bbox.YMax() < YMin() );
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotDataBBox::GetDistanceTo
/** @param  T_Point&
    @return
*/
// Created: CBX 2003-08-18
// -----------------------------------------------------------------------------
double GQ_PlotDataBBox::GetDistanceTo( const T_Point& point ) const
{
//    if( Includes( point ) )
//        return 0.0;

    double rX = point.first;
    double rY = point.second;

    double rdx = 0.0;
    double rdy = 0.0;

    if( rX < rXMin_ )
        rdx = rXMin_ - rX;
    else if( rX > rXMax_ )
        rdx = rX - rXMax_;

    if( rY < rYMin_ )
        rdy = rYMin_ - rY;
    else if( rY > rYMax_ )
        rdy = rY - rYMax_;
    else
        return rdx;

    return sqrt( rdx * rdx + rdy * rdy );
}
