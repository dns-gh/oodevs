// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "LogisticAttribute.h"
#include "Tools.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: LogisticAttribute constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
LogisticAttribute::LogisticAttribute( kernel::PropertiesDictionary& dico, kernel::Controllers& controllers )
    : controllers_( controllers )
    , tc2_( 0 )
{
    CreateDictionary( dico );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute constructor
// Created: SBO 2006-10-20
// -----------------------------------------------------------------------------
LogisticAttribute::LogisticAttribute( xml::xistream& xis, const tools::Resolver_ABC< kernel::Automat_ABC >& automats, kernel::PropertiesDictionary& dico, kernel::Controllers& controllers )
    : controllers_( controllers )
    , tc2_( automats.Find( xis.attribute< unsigned long >( "id" ) ) )
{
    CreateDictionary( dico );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
LogisticAttribute::~LogisticAttribute()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute::SetTC2
// Created: SBO 2006-09-11
// -----------------------------------------------------------------------------
void LogisticAttribute::SetTC2( const kernel::Automat_ABC& tc2 )
{
    tc2_ = &tc2;
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute::NotifyDeleted
// Created: SBO 2010-05-20
// -----------------------------------------------------------------------------
void LogisticAttribute::NotifyDeleted( const kernel::Automat_ABC& entity )
{
    if( &entity == tc2_ )
        tc2_ = 0;
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute::Display
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void LogisticAttribute::Display( kernel::Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Object", "Camp" ) )
                .Display( tools::translate( "Object", "TC2:" ), tc2_ );
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute::SerializeAttributes
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
void LogisticAttribute::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::start( "tc2" )
            << xml::attribute( "id", tc2_ ? tc2_->GetId() : 0 )
        << xml::end();
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute::CreateDictionary
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
void LogisticAttribute::CreateDictionary( kernel::PropertiesDictionary& dico )
{
    dico.Register( *this, tools::translate( "LogisticAttribute", "Info/Camp attributes/Superior" ), tc2_ );
}
