// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "NBCAttribute.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/NBCAgent.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "Tools.h"
#include <xeumeuleu/xml.h>

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: NBCAttribute constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
NBCAttribute::NBCAttribute( kernel::PropertiesDictionary& dico )
{
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: NBCAttribute constructor
// Created: SBO 2006-10-20
// -----------------------------------------------------------------------------
NBCAttribute::NBCAttribute( xml::xistream& xis, const tools::Resolver_ABC< kernel::NBCAgent, std::string >& nbcAgents, kernel::PropertiesDictionary& dico )
    : state_ ( "gaseous" )
{
    xis >> xml::optional() >> xml::attribute( "state", state_ );
    xis >> list( "nbc-agent", *this, &NBCAttribute::ReadNbcAgent, nbcAgents );        
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: NBCAttribute destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
NBCAttribute::~NBCAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NBCAttribute::ReadNbcAgent
// Created: SBO 2006-10-20
// -----------------------------------------------------------------------------
void NBCAttribute::ReadNbcAgent( xml::xistream& xis, const tools::Resolver_ABC< kernel::NBCAgent, std::string >& nbcAgents )
{
    std::string type;
    xis >> attribute( "type", type );
    AddAgent( nbcAgents.Get( type ) );
}

// -----------------------------------------------------------------------------
// Name: NBCAttribute::Display
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void NBCAttribute::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "NBC", "NBC" ) )
             .Display( tools::translate( "NBC", "NBC state:" ), state_ )
             .Display( tools::translate( "NBC", "Danger:" ), danger_ )
             .Display( tools::translate( "NBC", "NBC agent(s):" ), agents_ );
}

// -----------------------------------------------------------------------------
// Name: NBCAttribute::AddAgent
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
void NBCAttribute::AddAgent( const kernel::NBCAgent& agent )
{
    if( std::find( agents_.begin(), agents_.end(), &agent ) == agents_.end() )
        agents_.push_back( const_cast< kernel::NBCAgent* >( &agent ) );
}

// -----------------------------------------------------------------------------
// Name: NBCAttribute::SetState
// Created: JCR 2008-08-29
// -----------------------------------------------------------------------------
void NBCAttribute::SetState( const std::string& state )
{
    state_ = state;
}

// -----------------------------------------------------------------------------
// Name: NBCAttribute::SetDanger
// Created: LDC 2009-03-19
// -----------------------------------------------------------------------------
void NBCAttribute::SetDanger( const unsigned int value )
{
    danger_ = value;
}

// -----------------------------------------------------------------------------
// Name: NBCAttribute::SerializeAttributes
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
void NBCAttribute::SerializeAttributes( xml::xostream& xos ) const
{    
    xos << start( "nbc-agents" );
    if( state_ != "" )
        xos << xml::attribute( "state", state_ );
    for( T_NBCAgents::const_iterator it = agents_.begin(); it != agents_.end(); ++it )
    {
        xos << start( "nbc-agent" )
                << attribute( "type", (*it)->GetName() )
            << end();
    }
    xos << end();
}

// -----------------------------------------------------------------------------
// Name: NBCAttribute::CreateDictionary
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
void NBCAttribute::CreateDictionary( kernel::PropertiesDictionary& dico )
{
    dico.Register( *this, tools::translate( "NBCAttribute", "Info/NBC attributes/NBC state" ), state_ );
	dico.Register( *this, tools::translate( "NBCAttribute", "Info/NBC attributes/NBC agents" ), agents_ );    
}
