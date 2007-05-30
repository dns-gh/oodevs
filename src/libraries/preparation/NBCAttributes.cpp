// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "NBCAttributes.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/NBCAgent.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "Tools.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: NBCAttributes constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
NBCAttributes::NBCAttributes( kernel::PropertiesDictionary& dico )
    : nbc_( 0 )
{
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: NBCAttributes constructor
// Created: SBO 2006-10-20
// -----------------------------------------------------------------------------
NBCAttributes::NBCAttributes( xml::xistream& xis, const kernel::Resolver_ABC< kernel::NBCAgent, QString >& nbcAgents, kernel::PropertiesDictionary& dico )
{
    std::string type;
    xis >> start( "specific-attributes" )
            >> start( "nbc-agent" )
                >> attribute( "type", type )
            >> end()
        >> end();
    nbc_ = nbcAgents.Find( type.c_str() );
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: NBCAttributes destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
NBCAttributes::~NBCAttributes()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NBCAttributes::Display
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void NBCAttributes::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "NBC", "NBC cloud/zone" ) )
        .Display( tools::translate( "NBC", "NBC agent:" ), nbc_ );
}


// -----------------------------------------------------------------------------
// Name: NBCAttributes::SetAgent
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
void NBCAttributes::SetAgent( const kernel::NBCAgent& agent )
{
    nbc_ = const_cast< kernel::NBCAgent* >( &agent );
}

// -----------------------------------------------------------------------------
// Name: NBCAttributes::SerializeAttributes
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
void NBCAttributes::SerializeAttributes( xml::xostream& xos ) const
{
    xos << start( "specific-attributes" )
            << start( "nbc-agent" )
                << attribute( "type", nbc_->GetName() )
            << end()
        << end();
}

// -----------------------------------------------------------------------------
// Name: NBCAttributes::CreateDictionary
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
void NBCAttributes::CreateDictionary( kernel::PropertiesDictionary& dico )
{
    dico.Register( *this, tools::translate( "NBCAttributes", "Info/NBC attributes/NBC agent" ), nbc_ );
}
