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
#include "xeumeuleu/xml.h"

using namespace xml;

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC constructor
// Created: APE 2004-04-14
// -----------------------------------------------------------------------------
TacticalLine_ABC::TacticalLine_ABC( const QString& baseName, IdManager& idManager )
    : id_( idManager.GetNextId() )
{
    RegisterSelf( *this );
    strName_ = ( baseName + " %1" ).arg( id_ );
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC constructor
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
TacticalLine_ABC::TacticalLine_ABC( xml::xistream& xis, IdManager& idManager )
    : id_( idManager.GetNextId() )
{
    RegisterSelf( *this );
    std::string name;
    xis >> attribute( "name", name );
    strName_ = name.c_str();
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
// Name: TacticalLine_ABC::GetId
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
unsigned long TacticalLine_ABC::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::GetName
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
QString TacticalLine_ABC::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::Draw
// Created: SBO 2006-11-07
// -----------------------------------------------------------------------------
void TacticalLine_ABC::Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const kernel::GlTools_ABC& tools ) const
{
//    if( ! pointList_.empty() ) // $$$$ SBO 2006-11-07: Get< kernel::Positions >().IsSet()
    glPushAttrib( GL_CURRENT_BIT | GL_LINE_BIT );
        glColor3f( 0.f, 0.f, 0.f );
        tools.Print( strName_.ascii(), Get< kernel::Positions >().GetPosition() );
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::SerializeAttributes
// Created: SBO 2006-11-07
// -----------------------------------------------------------------------------
void TacticalLine_ABC::SerializeAttributes( xml::xostream& xos ) const
{
    xos << attribute( "name", std::string( strName_.ascii() ) );
}
