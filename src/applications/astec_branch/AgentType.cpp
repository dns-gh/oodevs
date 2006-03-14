// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "AgentType.h"
#include "xeumeuleu/xml.h"
#include "Nature.h"

using namespace xml;

// -----------------------------------------------------------------------------
// Name: AgentType constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
AgentType::AgentType( xml::xistream& xis, const Resolver_ABC< ComponentType, std::string >& componentResolver, const Resolver_ABC< DecisionalModel, std::string >& modelResolver )
    : nature_( 0 )
{
    std::string modelName;
    int id;
    xis >> attribute( "nom", name_ )
        >> content( "MosID", id )
        >> content(  "ModeleDecisionnel", modelName );
    id_ = id;
    model_ = & modelResolver.Get( modelName );

    std::auto_ptr< Nature > nature( new Nature( xis ) );
    xis >> start( "Equipements" )
            >> list( "Equipement", *this, ReadEquipment, componentResolver )
        >> end();
    nature_ = nature.release();
}

// -----------------------------------------------------------------------------
// Name: AgentType destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
AgentType::~AgentType()
{
    delete nature_;
}

// -----------------------------------------------------------------------------
// Name: AgentType::ReadEquipment
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentType::ReadEquipment( xml::xistream& xis, const Resolver_ABC< ComponentType, std::string >& resolver )
{
    std::string name;
    xis >> attribute( "nom", name );
    equipments_.push_back( & resolver.Get( name ) );
    // $$$$ AGE 2006-02-14: 
//            bHasMaintenance_ |= pTypeComposante->HasMaintenance();
//            bHasMedical_     |= pTypeComposante->HasMedical();
//            bHasSupply_      |= pTypeComposante->HasSupply();
}

// -----------------------------------------------------------------------------
// Name: AgentType::GetId
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
unsigned long AgentType::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: AgentType::GetName
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
const std::string& AgentType::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: AgentType::GetDecisionalModel
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
const DecisionalModel& AgentType::GetDecisionalModel() const
{
    return *model_;
}
