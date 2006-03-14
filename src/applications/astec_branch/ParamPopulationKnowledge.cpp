// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "ParamPopulationKnowledge.h"
#include "PopulationKnowledge.h"
#include "Population.h"

// -----------------------------------------------------------------------------
// Name: ParamPopulationKnowledge constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
ParamPopulationKnowledge::ParamPopulationKnowledge( QWidget* pParent, ASN1T_OID& id, const std::string& label, const std::string& menu )
    : EntityParameter< PopulationKnowledge >( pParent, id, label, menu )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamPopulationKnowledge destructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
ParamPopulationKnowledge::~ParamPopulationKnowledge()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamPopulationKnowledge::NotifyContextMenu
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void ParamPopulationKnowledge::NotifyContextMenu( const Population& entity, QPopupMenu& menu )
{
    // Disallow using knowledges on Populations from our own team.
//    if( context.selectedElement_.pPopulation_->GetTeam().GetID() == dynamic_cast<Agent*>(&agent_)->GetTeam().GetID() )
//        return;
//
//    pPopupKnowledge_ = dynamic_cast<Agent*>(&agent_)->GetGtia().FindKnowledgeOnPopulation( *(context.selectedElement_.pPopulation_) );
//    if( pPopupKnowledge_ == 0 )
//        return;
}
