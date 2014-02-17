// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "PathContext.h"

using namespace svg;
             
// -----------------------------------------------------------------------------
// Name: PathContext constructor
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
PathContext::PathContext()
    : relative_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PathContext destructor
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
PathContext::~PathContext()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PathContext::SetRelative
// Created: AGE 2006-08-15
// -----------------------------------------------------------------------------
void PathContext::SetRelative( bool relative )
{
    relative_ = relative;
}

// -----------------------------------------------------------------------------
// Name: PathContext::ResetControlPoint
// Created: AGE 2006-08-15
// -----------------------------------------------------------------------------
void PathContext::ResetControlPoint()
{
    controlPoint_ = geometry::Point();
}

// -----------------------------------------------------------------------------
// Name: PathContext::Start
// Created: AGE 2006-08-15
// -----------------------------------------------------------------------------
void PathContext::Start( const geometry::Point& p )
{
    startPoint_ = p;
}

// -----------------------------------------------------------------------------
// Name: PathContext::MoveTo
// Created: AGE 2006-08-15
// -----------------------------------------------------------------------------
void PathContext::MoveTo( const geometry::Point& p )
{
    currentPoint_ = p;
}

// -----------------------------------------------------------------------------
// Name: PathContext::SetControlPoint
// Created: AGE 2006-08-15
// -----------------------------------------------------------------------------
void PathContext::SetControlPoint( const geometry::Point c )
{
    controlPoint_ = c;
}

// -----------------------------------------------------------------------------
// Name: PathContext::Translate
// Created: AGE 2006-08-15
// -----------------------------------------------------------------------------
geometry::Point PathContext::Translate( const geometry::Point& p )
{
    if( relative_ )
        return currentPoint_ + p;
    return p;
}

// -----------------------------------------------------------------------------
// Name: PathContext::StartPoint
// Created: AGE 2006-08-15
// -----------------------------------------------------------------------------
const geometry::Point& PathContext::StartPoint() const
{
    return startPoint_;
}
    
// -----------------------------------------------------------------------------
// Name: PathContext::CurrentPoint
// Created: AGE 2006-08-15
// -----------------------------------------------------------------------------
const geometry::Point& PathContext::CurrentPoint() const
{
    return currentPoint_;
}

// -----------------------------------------------------------------------------
// Name: PathContext::ControlPoint
// Created: AGE 2006-08-15
// -----------------------------------------------------------------------------
const geometry::Point& PathContext::ControlPoint() const
{
    return controlPoint_;
}

// -----------------------------------------------------------------------------
// Name: PathContext::IsRelative
// Created: AGE 2006-08-15
// -----------------------------------------------------------------------------
bool PathContext::IsRelative() const
{
    return relative_;
}

// -----------------------------------------------------------------------------
// Name: PathContext::InferedControlPoint
// Created: AGE 2006-08-15
// -----------------------------------------------------------------------------
geometry::Point PathContext::InferedControlPoint()
{
    return 2.f * currentPoint_ - controlPoint_;
}
