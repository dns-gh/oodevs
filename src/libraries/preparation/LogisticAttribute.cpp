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
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "Tools.h"
#include <xeumeuleu/xml.h>

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: LogisticAttribute constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
LogisticAttribute::LogisticAttribute( kernel::PropertiesDictionary& dico )
    : tc2_( 0 )
{
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute constructor
// Created: SBO 2006-10-20
// -----------------------------------------------------------------------------
LogisticAttribute::LogisticAttribute( xml::xistream& xis, const tools::Resolver_ABC< Automat_ABC >& automats, kernel::PropertiesDictionary& dico )
    : tc2_( 0 )
{
    int id;
    xis >> attribute( "id", id );    
    tc2_ = automats.Find( static_cast< unsigned long >( id ) );
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
LogisticAttribute::~LogisticAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute::SetTC2
// Created: SBO 2006-09-11
// -----------------------------------------------------------------------------
void LogisticAttribute::SetTC2( const Automat_ABC& tc2 )
{
    tc2_ = &tc2;
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute::Display
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void LogisticAttribute::Display( Displayer_ABC& displayer ) const
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
    xos << start( "tc2" )
            << attribute( "id", long( ((const kernel::Automat_ABC*)tc2_)->GetId() ) )            
        << end();
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute::CreateDictionary
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
void LogisticAttribute::CreateDictionary( kernel::PropertiesDictionary& dico )
{
    dico.Register( *this, tools::translate( "LogisticAttribute", "Info/Camp attributes/Superior" ), tc2_ );
}
