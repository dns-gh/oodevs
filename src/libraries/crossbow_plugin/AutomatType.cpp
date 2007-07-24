//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "AutomatType.h"
#include "AutomatComposition.h"
#include "AgentType.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: AutomatType constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
AutomatType::AutomatType( xml::xistream& xis, const Resolver_ABC< AgentType, std::string >& agentResolver )
{
    int id;
    std::string modelName;

    xis >> attribute( "nom", name_ )
        >> attribute( "type", type_ )
        >> content( "MosID", id_ )
        >> start( "Automate" )
            >> content( "ModeleDecisionnel", modelName )
            >> start( "Constitution" )
                >> list( "Pion", *this, &AutomatType::ReadAgent, agentResolver )
            >> end();    
    std::string pcType;
    xis >> end()
        >> start( "PionPC" )
            >> attribute( "type", pcType );
    pcType_ = & agentResolver.Get( pcType.c_str() );
    symbol_ = pcType_->GetSymbol();
}

// -----------------------------------------------------------------------------
// Name: AutomatType destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
AutomatType::~AutomatType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatType::ReadAgent
// Created: SBO 2006-08-28
// -----------------------------------------------------------------------------
void AutomatType::ReadAgent( xml::xistream& xis, const Resolver_ABC< AgentType, std::string >& agentResolver )
{
    units_.push_back( new AutomatComposition( xis, agentResolver ) );
}


// -----------------------------------------------------------------------------
// Name: AutomatType::GetTypePC
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
AgentType* AutomatType::GetTypePC() const
{   
    return pcType_;
}

// -----------------------------------------------------------------------------
// Name: AutomatType::GetId
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
unsigned long AutomatType::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: AutomatType::GetName
// Created: SBO 2006-08-28
// -----------------------------------------------------------------------------
const std::string& AutomatType::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: AutomatType::GetSymbol
// Created: SBO 2007-02-22
// -----------------------------------------------------------------------------
const std::string& AutomatType::GetSymbol() const
{
    return symbol_;
}


