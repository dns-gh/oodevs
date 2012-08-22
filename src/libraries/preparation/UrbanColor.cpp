// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "UrbanColor.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/ModeController_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/Tools.h"
#include "ENT/ENT_Enums_Gen.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: UrbanColor constructor
// Created: ABR 2012-06-04
// -----------------------------------------------------------------------------
UrbanColor::UrbanColor( const kernel::Entity_ABC* parent, kernel::Controllers& controllers,
                        kernel::UrbanObject_ABC& object, kernel::PropertiesDictionary& dictionary )
    : controllers_( controllers )
    , object_     ( object )
    , dictionary_ ( dictionary )
{
    if( parent )
    {
        const kernel::UrbanColor_ABC& parentColor = parent->Get< kernel::UrbanColor_ABC >();
        SetColor( parentColor.Red(), parentColor.Green(), parentColor.Blue(), static_cast< int >( parentColor.Alpha() * 255 + 0.5 ) );
        initial_ = current_;
    }
    assert( controllers_.modes_ );
    controllers_.modes_->Register( *this );
    CreateDictionnary( controllers_.modes_->GetCurrentMode() == ePreparationMode_Exercise );
}

// -----------------------------------------------------------------------------
// Name: UrbanColor constructor
// Created: LGY 2011-04-19
// -----------------------------------------------------------------------------
UrbanColor::UrbanColor( xml::xistream& xis, kernel::Controllers& controllers, kernel::UrbanObject_ABC& object, kernel::PropertiesDictionary& dictionary )
    : controllers_( controllers )
    , object_     ( object )
    , dictionary_ ( dictionary )
{
    if( xis.has_child( "color" ) )
    {
        xis >> xml::start( "color" );
        SetColor( xis.attribute< int >( "red" ), xis.attribute< int >( "green" ),
                  xis.attribute< int >( "blue" ), static_cast< int >( xis.attribute< float >( "alpha" ) * 255 ) );
        initial_ = current_;
        xis >> xml::end;
    }
    CreateDictionnary( true );
    if( controllers_.modes_ )
        controllers_.modes_->Register( *this );
}

// -----------------------------------------------------------------------------
// Name: UrbanColor destructor
// Created: LGY 2011-04-19
// -----------------------------------------------------------------------------
UrbanColor::~UrbanColor()
{
    if( controllers_.modes_ )
        controllers_.modes_->Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: UrbanColor::SerializeAttributes
// Created: ABR 2012-05-22
// -----------------------------------------------------------------------------
void UrbanColor::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::start( "color" )
            << xml::attribute< int >( "red", current_.red_ )
            << xml::attribute< int >( "green", current_.green_ )
            << xml::attribute< int >( "blue", current_.blue_ )
            << xml::attribute< float >( "alpha", Alpha() )
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: UrbanColor::NotifyModeChanged
// Created: ABR 2012-06-04
// -----------------------------------------------------------------------------
void UrbanColor::NotifyModeChanged( int newMode )
{
    if( newMode == ePreparationMode_Exercise )
        CreateDictionnary( true );
    else if( newMode == ePreparationMode_Terrain )
        CreateDictionnary( false );
}

// -----------------------------------------------------------------------------
// Name: UrbanColor::CreateDictionnary
// Created: ABR 2012-06-04
// -----------------------------------------------------------------------------
void UrbanColor::CreateDictionnary( bool readOnly )
{
    dictionary_.Register( object_, tools::translate( "UrbanColor", "Info/Color" ), current_, readOnly, kernel::eUrbanTemplate );
}
