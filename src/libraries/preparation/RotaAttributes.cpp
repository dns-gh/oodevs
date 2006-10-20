// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "RotaAttributes.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/NBCAgent.h"
#include "clients_gui/Tools.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: RotaAttributes constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
RotaAttributes::RotaAttributes()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RotaAttributes constructor
// Created: SBO 2006-10-20
// -----------------------------------------------------------------------------
RotaAttributes::RotaAttributes( xml::xistream& xis, const kernel::Resolver_ABC< kernel::NBCAgent, QString >& nbcAgents )
{
    xis >> start( "specific-attributes" )
            >> content( "danger", (int&)danger_ )
            >> start( "nbc-agents" )
                >> list( "nbc-agent", *this, &RotaAttributes::ReadNbcAgent, nbcAgents )
            >> end()
        >> end();
}

// -----------------------------------------------------------------------------
// Name: RotaAttributes destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
RotaAttributes::~RotaAttributes()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RotaAttributes::ReadNbcAgent
// Created: SBO 2006-10-20
// -----------------------------------------------------------------------------
void RotaAttributes::ReadNbcAgent( xml::xistream& xis, const kernel::Resolver_ABC< kernel::NBCAgent, QString >& nbcAgents )
{
    std::string type;
    xis >> attribute( "type", type );
    AddAgent( nbcAgents.Get( type.c_str() ) );
}

// -----------------------------------------------------------------------------
// Name: RotaAttributes::Display
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void RotaAttributes::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "NBC", "ROTA" ) )
                .Display( tools::translate( "NBC", "Danger:" ), danger_ )
                .Display( tools::translate( "NBC", "Agents NBC:" ), agents_ );
}

// -----------------------------------------------------------------------------
// Name: RotaAttributes::SetDanger
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
void RotaAttributes::SetDanger( unsigned int level )
{
    danger_ = level;
}

// -----------------------------------------------------------------------------
// Name: RotaAttributes::AddAgent
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
void RotaAttributes::AddAgent( const kernel::NBCAgent& agent )
{
    if( std::find( agents_.begin(), agents_.end(), &agent ) == agents_.end() )
        agents_.push_back( &agent );
}

// -----------------------------------------------------------------------------
// Name: RotaAttributes::DoSerialize
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
void RotaAttributes::DoSerialize( xml::xostream& xos ) const
{
    xos << start( "specific-attributes" )
            << content( "danger", int( danger_ ) )
            << start( "nbc-agents" );
    for( T_Nbcs::const_iterator it = agents_.begin(); it != agents_.end(); ++it )
    {
        xos << start( "nbc-agent" )
                << attribute( "type", (*it)->GetName() )
            << end();
    }
    xos << end()
        << end();
}
