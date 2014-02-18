// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "DrawerTesselator.h"

// -----------------------------------------------------------------------------
// Name: DrawerTesselator constructor
// Created: AGE 2005-01-10
// -----------------------------------------------------------------------------
DrawerTesselator::DrawerTesselator( unsigned int windingRule )
    : TesselatorBase( windingRule )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: DrawerTesselator destructor
// Created: AGE 2005-01-10
// -----------------------------------------------------------------------------
DrawerTesselator::~DrawerTesselator()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DrawerTesselator::Begin
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
void DrawerTesselator::Begin( unsigned int type )
{
    glBegin( type );
}

// -----------------------------------------------------------------------------
// Name: DrawerTesselator::Vertex
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
void DrawerTesselator::Vertex( const geometry::Point& point, unsigned int )
{
    glVertex2fv( (const float*)&point );
}
    
// -----------------------------------------------------------------------------
// Name: DrawerTesselator::End
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
void DrawerTesselator::End()
{
    glEnd();
}
