// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "Lighting.h"
#include "graphics/FixedLighting.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: Lighting constructor
// Created: AGE 2007-01-02
// -----------------------------------------------------------------------------
Lighting::Lighting()
    : fixed_( new FixedLighting() )
{
    fixed_->SetAmbient( 0.2f, 0.2f, 0.2f );
    fixed_->SetDiffuse( 0.8f, 0.8f, 0.8f );
    fixed_->SetLightDirection( geometry::Vector3f( 0.1f, -0.2f, 0.9f ) );
    current_ = fixed_.get();
}

// -----------------------------------------------------------------------------
// Name: Lighting destructor
// Created: AGE 2007-01-02
// -----------------------------------------------------------------------------
Lighting::~Lighting()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Lighting::SetAmbient
// Created: AGE 2007-01-02
// -----------------------------------------------------------------------------
void Lighting::SetAmbient( float r, float g, float b )
{
    fixed_->SetAmbient( r, g, b );
}

// -----------------------------------------------------------------------------
// Name: Lighting::SetDiffuse
// Created: AGE 2007-01-02
// -----------------------------------------------------------------------------
void Lighting::SetDiffuse( float r, float g, float b )
{
    fixed_->SetDiffuse( r, g, b );
}

// -----------------------------------------------------------------------------
// Name: Lighting::SetFixedLighting
// Created: AGE 2007-01-02
// -----------------------------------------------------------------------------
void Lighting::SetFixedLighting( const geometry::Vector3f& direction )
{
    direction_ = direction;
    fixed_->SetLightDirection( direction );
    current_ = fixed_.get();
}

// -----------------------------------------------------------------------------
// Name: Lighting::Set
// Created: AGE 2007-01-02
// -----------------------------------------------------------------------------
void Lighting::Set()
{
    glColor3f( 1, 0, 0 );
    glBegin( GL_LINES );
        glVertex3f( 0, 0, 0 );
        glVertex3f( direction_.X() * 1000, direction_.Y() * 1000, direction_.Z() * 1000 );
    glEnd();
    current_->Set();
}
