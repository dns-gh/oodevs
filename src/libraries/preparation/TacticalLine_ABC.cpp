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
#include "IdManager.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/Tools.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC constructor
// Created: APE 2004-04-14
// -----------------------------------------------------------------------------
TacticalLine_ABC::TacticalLine_ABC( kernel::Controller& controller, const QString& baseName, IdManager& idManager )
    : kernel::EntityImplementation< kernel::TacticalLine_ABC >( controller, idManager.GetNextId(), "" )
{
    RegisterSelf( *this );
    name_ = ( baseName + " %1" ).arg( id_ );
    CreateDictionary( controller );
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC constructor
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
TacticalLine_ABC::TacticalLine_ABC( kernel::Controller& controller, xml::xistream& xis, IdManager& idManager )
    : kernel::EntityImplementation< kernel::TacticalLine_ABC >( controller, idManager.GetNextId(), "" )
{
    RegisterSelf( *this );
    std::string name;
    xis >> xml::attribute( "name", name );
    name_ = name.c_str();
    CreateDictionary( controller );
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
void TacticalLine_ABC::Draw( const geometry::Point2f&, const kernel::Viewport_ABC&, const kernel::GlTools_ABC& tools ) const
{
    glPushAttrib( GL_CURRENT_BIT | GL_LINE_BIT );
        glColor3f( 0.f, 0.f, 0.f );
        if( !name_.isEmpty() )
            tools.Print( name_.ascii(), Get< kernel::Positions >().GetPosition() );
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::SerializeAttributes
// Created: SBO 2006-11-07
// -----------------------------------------------------------------------------
void TacticalLine_ABC::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::attribute( "name", name_.ascii() );
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::CreateDictionary
// Created: SBO 2006-11-17
// -----------------------------------------------------------------------------
void TacticalLine_ABC::CreateDictionary( kernel::Controller& controller )
{
    kernel::PropertiesDictionary& dictionary = *new kernel::PropertiesDictionary( controller );
    Attach( dictionary );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "TacticalLine_ABC", "Info/Identifier" ), (const unsigned long)id_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "TacticalLine_ABC", "Info/Name" ), name_ );
}
