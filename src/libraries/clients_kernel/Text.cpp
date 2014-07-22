// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "Text.h"
#include "LocationVisitor_ABC.h"
#include "Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Text constructor
// Created: LGY 2014-06-11
// -----------------------------------------------------------------------------
Text::Text()
    : pointSet_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Text destructor
// Created: LGY 2014-06-11
// -----------------------------------------------------------------------------
Text::~Text()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Text::PopPoint
// Created: LGY 2014-06-11
// -----------------------------------------------------------------------------
void Text::PopPoint()
{
    pointSet_ = false;
}

// -----------------------------------------------------------------------------
// Name: Text::AddPoint
// Created: LGY 2014-06-11
// -----------------------------------------------------------------------------
void Text::AddPoint( const geometry::Point2f& point )
{
    pointSet_ = true;
    point_ = point;
}

// -----------------------------------------------------------------------------
// Name: Text::Translate
// Created: LGY 2014-06-11
// -----------------------------------------------------------------------------
void Text::Translate( const geometry::Point2f& from, const geometry::Vector2f& translation, float precision )
{
    if( IsValid() && point_.SquareDistance( from ) < precision * precision )
        point_ += translation;
}

// -----------------------------------------------------------------------------
// Name: Text::IsValid
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
bool Text::IsValid() const
{
    return pointSet_;
}

// -----------------------------------------------------------------------------
// Name: Text::IsDone
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
bool Text::IsDone() const
{
    return IsValid();
}

// -----------------------------------------------------------------------------
// Name: Text::Accept
// Created: LGY 2014-06-11
// -----------------------------------------------------------------------------
void Text::Accept( LocationVisitor_ABC& visitor ) const
{
    visitor.VisitText( text_, font_, point_ );
}

// -----------------------------------------------------------------------------
// Name: Text::GetName
// Created: LGY 2014-06-11
// -----------------------------------------------------------------------------
QString Text::GetName() const
{
    static const QString name = tools::translate( "Localisation", "Text" );
    return name;
}

// -----------------------------------------------------------------------------
// Name: Text::Clone
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
Location_ABC& Text::Clone() const
{
    return *new Text( *this );
}

// -----------------------------------------------------------------------------
// Name: Text::AddText
// Created: LGY 2014-06-11
// -----------------------------------------------------------------------------
void Text::AddText( const QString& text, const QFont& font )
{
    text_ = text;
    font_ = font;
}

// -----------------------------------------------------------------------------
// Name: Text::GetTypeName
// Created: LGY 2014-06-11
// -----------------------------------------------------------------------------
std::string Text::GetTypeName() const
{
    return "text";
}
