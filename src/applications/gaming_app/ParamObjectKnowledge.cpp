// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamObjectKnowledge.h"
#include "gaming/ObjectKnowledge.h"
#include "clients_kernel/Object_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledge constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
ParamObjectKnowledge::ParamObjectKnowledge( QWidget* pParent, ASN1T_OID& id, const std::string& label, const std::string& menu )
    : EntityParameter< ObjectKnowledge >( pParent, id, label, menu )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledge destructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
ParamObjectKnowledge::~ParamObjectKnowledge()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledge::NotifyContextMenu
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void ParamObjectKnowledge::NotifyContextMenu( const Object_ABC& entity, ContextMenu& menu )
{
    // agent_.GetKnowledgeGroup()->GetTeam().Get< ObjectKnowledges >()....
    // $$$$ AGE 2006-03-14: 
//    if( context.selectedElement_.pObject_ != 0 )
//    {
//        pPopupKnowledge_ = dynamic_cast<Agent_ABC*>(&agent_)->GetTeam().FindKnowledgeOnObject( *(context.selectedElement_.pObject_) );
//        if( pPopupKnowledge_ == 0 )
//            return;
//    }
}
