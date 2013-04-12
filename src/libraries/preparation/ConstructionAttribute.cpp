// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "ConstructionAttribute.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/DotationType.h"
#include "clients_gui/PropertiesDictionary.h"
#include "clients_kernel/Tools.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute constructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
ConstructionAttribute::ConstructionAttribute( gui::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity )
    : completion_   ( 0, kernel::Units::percentageGTzero )
    , dotationType_ ( 0 )
    , dotationCount_( 0 )
{
    CreateDictionary( dictionary, entity );
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute constructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
ConstructionAttribute::ConstructionAttribute( xml::xistream& xis, gui::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity )
    : completion_   ( unsigned int( xis.attribute< float >( "completion", 1.f ) * 100 ), kernel::Units::percentageGTzero )
    , dotationType_ ( 0 )
    , dotationCount_( 0 )
{
    CreateDictionary( dictionary, entity );
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute destructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
ConstructionAttribute::~ConstructionAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::Display
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void ConstructionAttribute::Display( kernel::Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Object", "Information" ) ) // $$$$ ABR 2012-03-20: Used ?
             .Display( tools::translate( "Object", "Construction:" ), completion_ );
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::DisplayInTooltip
// Created: JCR 2008-06-10
// -----------------------------------------------------------------------------
void ConstructionAttribute::DisplayInTooltip( kernel::Displayer_ABC& displayer ) const
{
    displayer.Display( tools::translate( "Object", "Construction:" ), completion_ );
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::SerializeObjectAttributes
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void ConstructionAttribute::SerializeObjectAttributes( xml::xostream& xos ) const
{
    xos << xml::start( "construction" )
            << xml::attribute( "completion", float( completion_.value_ ) / 100.f );
    if( dotationType_ != 0 && dotationCount_ != 0 )
        xos << xml::start( "resources" )
                << xml::start( "dotation" )
                    << xml::attribute( "name", dotationType_->GetName() )
                    << xml::attribute( "count", dotationCount_ )
                << xml::end
            << xml::end;
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::CreateDictionary
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void ConstructionAttribute::CreateDictionary( gui::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity )
{
    dictionary.RegisterExtension( entity, this, tools::translate( "Object", "Info/Construction/Completion" ), completion_ );
    dictionary.RegisterExtension( entity, this, tools::translate( "Object", "Info/Construction/Resource/Type" ), dotationType_ );
    dictionary.RegisterExtension( entity, this, tools::translate( "Object", "Info/Construction/Resource/Count" ), dotationCount_ );
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::SetCompletion
// Created: SBO 2009-06-09
// -----------------------------------------------------------------------------
void ConstructionAttribute::SetCompletion( unsigned int value )
{
    completion_.value_ = value;
}
