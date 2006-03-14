// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "AutomatType.h"
#include "xeumeuleu/xml.h"

using namespace xml;

// -----------------------------------------------------------------------------
// Name: AutomatType constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
AutomatType::AutomatType( xml::xistream& xis, const Resolver_ABC< AgentType, std::string >& agentResolver
                                            , const Resolver_ABC< DecisionalModel, std::string >& modelResolver )
{
    int id;
    std::string modelName;

    xis >> attribute( "nom", name_ )
        >> attribute( "type", type_ )
        >> content( "MosID", id )
        >> start( "Automate" )
            >> content( "ModeleDecisionnel", modelName );
    id_ = id;
    model_ = & modelResolver.Get( modelName );
    std::string pcType;
    xis >> end()
        >> start( "PionPC" )
            >> attribute( "type", pcType );
    pcType_ = & agentResolver.Get( pcType );
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
// Name: AutomatType::GetTypePC
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
AgentType* AutomatType::GetTypePC()
{   
    return pcType_;
}

// -----------------------------------------------------------------------------
// Name: AutomatType::GetId
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
unsigned long AutomatType::GetId()
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: AutomatType::GetDecisionalModel
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
const DecisionalModel& AutomatType::GetDecisionalModel() const
{
    return *model_;
}
