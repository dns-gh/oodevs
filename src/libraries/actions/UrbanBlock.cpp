// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "UrbanBlock.h"
#include "ParameterVisitor_ABC.h"
#include "clients_gui/GLOptions.h"
#include "clients_gui/GLView_ABC.h"
#include "clients_kernel/EntityResolver_ABC.h"
#include "clients_kernel/UrbanPositions_ABC.h"
#include "protocol/Protocol.h"
#include <windows.h>
#include <gl/gl.h>
#undef GetObject

using namespace kernel;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: UrbanBlock constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
UrbanBlock::UrbanBlock( const OrderParameter& parameter, kernel::Controller& controller )
    : Entity< UrbanObject_ABC >( parameter, controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanBlock constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
UrbanBlock::UrbanBlock( const OrderParameter& parameter, unsigned long id, const kernel::EntityResolver_ABC& resolver, kernel::Controller& controller )
    : Entity< UrbanObject_ABC >( parameter, resolver.FindUrbanObject( id ), controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanBlock destructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
UrbanBlock::~UrbanBlock()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanBlock::Accept
// Created: JSR 2010-05-20
// -----------------------------------------------------------------------------
void UrbanBlock::Accept( ParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

// -----------------------------------------------------------------------------
// Name: UrbanBlock::Draw
// Created: LGY 2011-02-04
// -----------------------------------------------------------------------------
void UrbanBlock::Draw( const geometry::Point2f& /*where*/, const gui::Viewport_ABC& /*viewport*/, gui::GLView_ABC& tools ) const
{
    const bool selected = tools.GetOptions().IsDrawingSelection();
    GLfloat color[4];
    glGetFloatv( GL_CURRENT_COLOR, color );
    glPushAttrib( GL_LINE_BIT );
    if( selected )
    {
        glPushAttrib( GL_CURRENT_BIT );
        glColor4f( 0, 0, 0, color[3] * 0.5f );
        glLineWidth( 6.f );
        tools.DrawCross( GetPosition(), GL_CROSSSIZE );
        glPopAttrib();
    }
    glLineWidth( 2.f );
    tools.DrawCross( GetPosition(), GL_CROSSSIZE );
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: UrbanBlock::CommitTo
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void UrbanBlock::CommitTo( sword::MissionParameter& message ) const
{
    message.set_null_value( !IsSet() );
    if( IsSet() )
        Entity< UrbanObject_ABC >::CommitTo( *message.add_value()->mutable_urbanknowledge() );
}
// -----------------------------------------------------------------------------
// Name: UrbanBlock::CommitTo
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void UrbanBlock::CommitTo( sword::MissionParameter_Value& message ) const
{
    if( IsSet() )
        Entity< UrbanObject_ABC >::CommitTo( *message.mutable_urbanknowledge() );
}

// -----------------------------------------------------------------------------
// Name: UrbanBlock::GetPosition
// Created: LGY 2011-02-04
// -----------------------------------------------------------------------------
geometry::Point2f UrbanBlock::GetPosition() const
{
    if( IsSet() )
        if( const kernel::UrbanPositions_ABC* positions = GetValue()->Retrieve< kernel::UrbanPositions_ABC >() )
            return positions->Barycenter();
    return Entity< UrbanObject_ABC >::GetPosition();
}

// -----------------------------------------------------------------------------
// Name: UrbanBlock::DisplayInToolTip
// Created: LGY 2011-02-04
// -----------------------------------------------------------------------------
void UrbanBlock::DisplayInToolTip( kernel::Displayer_ABC& displayer ) const
{
    displayer.Display( "", GetName() );
}

// -----------------------------------------------------------------------------
// Name: UrbanBlock::SerializeType
// Created: LDC 2011-05-13
// -----------------------------------------------------------------------------
std::string UrbanBlock::SerializeType() const
{
    return "urbanknowledge";
}
