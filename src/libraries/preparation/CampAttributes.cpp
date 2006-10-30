// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "CampAttributes.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_gui/Tools.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: CampAttributes constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
CampAttributes::CampAttributes( Controllers& controllers, kernel::PropertiesDictionary& dico )
    : tc2_( 0 )
{
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: CampAttributes constructor
// Created: SBO 2006-10-20
// -----------------------------------------------------------------------------
CampAttributes::CampAttributes( xml::xistream& xis, kernel::Controllers& controllers, const Resolver_ABC< Automat_ABC >& automats, kernel::PropertiesDictionary& dico )
    : tc2_( 0 )
{
    int id;
    xis >> start( "specific-attributes" )
            >> start( "tc2" )
                >> attribute( "id", id )
            >> end()
        >> end();
    tc2_ = automats.Find( id );
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: CampAttributes destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
CampAttributes::~CampAttributes()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CampAttributes::SetTC2
// Created: SBO 2006-09-11
// -----------------------------------------------------------------------------
void CampAttributes::SetTC2( const Automat_ABC& tc2 )
{
    tc2_ = &tc2;
}

// -----------------------------------------------------------------------------
// Name: CampAttributes::Display
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void CampAttributes::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Object", "Camp" ) )
                .Display( tools::translate( "Object", "TC2:" ), tc2_ );
}

// -----------------------------------------------------------------------------
// Name: CampAttributes::SerializeAttributes
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
void CampAttributes::SerializeAttributes( xml::xostream& xos ) const
{
    xos << start( "specific-attributes" )
            << start( "tc2" )
            << attribute( "id", long( ((const kernel::Automat_ABC*)tc2_)->GetId() ) )
            << end()
        << end();
}

// -----------------------------------------------------------------------------
// Name: CampAttributes::CreateDictionary
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
void CampAttributes::CreateDictionary( kernel::PropertiesDictionary& dico )
{
    dico.Register( *this, tools::translate( "CampAttributes", "Info/Camp attributes/Superior" ), tc2_ );
}
