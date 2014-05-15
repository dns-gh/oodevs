// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "TacticalLine_ABC.h"
#include "TacticalLinePositions.h"
#include "tools/IdManager.h"
#include "clients_gui/GlTools_ABC.h"
#include "clients_gui/PropertiesDictionary.h"
#include "clients_kernel/Tools.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC constructor
// Created: APE 2004-04-14
// -----------------------------------------------------------------------------
TacticalLine_ABC::TacticalLine_ABC( kernel::Controller& controller, const QString& baseName, tools::IdManager& idManager )
    : gui::EntityImplementation< kernel::TacticalLine_ABC >( controller, idManager.GetNextId(), "" )
{
    AddExtension( *this );
    name_ = ( baseName + " %L1" ).arg( id_ );
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC constructor
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
TacticalLine_ABC::TacticalLine_ABC( kernel::Controller& controller, xml::xistream& xis, tools::IdManager& idManager )
    : gui::EntityImplementation< kernel::TacticalLine_ABC >( controller, idManager.GetNextId(), "" )
{
    AddExtension( *this );
    std::string name;
    xis >> xml::attribute( "name", name );
    name_ = name.c_str();
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC destructor
// Created: APE 2004-04-14
// -----------------------------------------------------------------------------
TacticalLine_ABC::~TacticalLine_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::Draw
// Created: SBO 2006-11-07
// -----------------------------------------------------------------------------
void TacticalLine_ABC::Draw( const geometry::Point2f&, const gui::Viewport_ABC&, gui::GlTools_ABC& tools ) const
{
    glPushAttrib( GL_CURRENT_BIT | GL_LINE_BIT );
        glColor3f( 0.f, 0.f, 0.f );
        if( !name_.isEmpty() )
            tools.Print( name_.toStdString(), Get< kernel::Positions >().GetPosition() );
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::SerializeAttributes
// Created: SBO 2006-11-07
// -----------------------------------------------------------------------------
void TacticalLine_ABC::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::attribute( "name", name_.toStdString() );
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::NotifyDestruction
// Created: LGY 2014-05-15
// -----------------------------------------------------------------------------
void TacticalLine_ABC::NotifyDestruction()
{
    // NOTHING
}
