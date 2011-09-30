// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "mission_tester_pch.h"
#include "AgentKnowledgeConverter.h"

using namespace mission_tester;


// -----------------------------------------------------------------------------
// Name: AgentKnowledge::SerializeType
// Created: LDC 2011-05-13
// -----------------------------------------------------------------------------
actions::parameters::AgentKnowledge* AgentKnowledgeConverter::FindAgent( const kernel::Agent_ABC* agent )
{
    IT_Knowledges it;
    return it->second.second;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::SerializeType
// Created: LDC 2011-05-13
// -----------------------------------------------------------------------------
void AgentKnowledgeConverter::AddAgent( const kernel::Agent_ABC* agent, actions::parameters::AgentKnowledge* knowledge, unsigned int id )
{
    //std::pair< kernel::Agent_ABC*, actions::parameters::AgentKnowledge* >(agent,knowledge) newKnowledge;
  //  std::map <kernel::Agent_ABC*, actions::parameters::AgentKnowledge*> newKnowledge;
//    knowledges_[id]= newKnowledge;
    //knowledges_.insert( std::pair< int, std::pair< kernel::Agent_ABC*, actions::parameters::AgentKnowledge* > >( id, newKnowledge ) );
}
