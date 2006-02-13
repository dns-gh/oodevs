// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-05-10 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ParamObjectKnowledge.cpp $
// $Author: Ape $
// $Modtime: 20/09/04 17:07 $
// $Revision: 5 $
// $Workfile: ParamObjectKnowledge.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "ParamObjectKnowledge.h"
#include "moc_ParamObjectKnowledge.cpp"

#include "App.h"
#include "Agent.h"
#include "Agent_ABC.h"
#include "Team.h"
#include "ActionContext.h"
#include "ObjectKnowledge.h"

// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledge constructor
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
ParamObjectKnowledge::ParamObjectKnowledge( ASN1T_KnowledgeObject& asnKnowledge, Agent_ABC& agent, const std::string strLabel, const std::string strMenuText, QWidget* pParent, bool bOptional )
    : QHBox             ( pParent )
    , Param_ABC     ( bOptional )
    , strMenuText_      ( strMenuText )
    , asnKnowledge_     ( asnKnowledge )
    , agent_            ( agent )
    , pPopupKnowledge_  ( 0 )
    , pKnowledge_  ( 0 )
{
    pLabel_ = new MT_ParameterLabel( strLabel.c_str(), false, this, "" );

    pKnowledgeLabel_ = new QLabel( "---", this );
    pKnowledgeLabel_->setMinimumWidth( 100 );
    pKnowledgeLabel_->setAlignment( Qt::AlignCenter );
    pKnowledgeLabel_->setFrameStyle( QFrame::Box | QFrame::Sunken );

    connect( &App::GetApp(), SIGNAL( ObjectKnowledgeDeleted( Team&, ObjectKnowledge& ) ), this, SLOT( OnObjectKnowledgeDeleted( Team&, ObjectKnowledge& ) ) );
}


// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledge destructor
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
ParamObjectKnowledge::~ParamObjectKnowledge()
{
}


// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledge::FillRemotePopupMenu
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void ParamObjectKnowledge::FillRemotePopupMenu( QPopupMenu& popupMenu, const ActionContext& context )
{
    if( context.selectedElement_.pObjectKnowledge_ == 0 && context.selectedElement_.pObject_ == 0 )
        return;

    if( context.selectedElement_.pObject_ != 0 )
    {
        pPopupKnowledge_ = dynamic_cast<Agent*>(&agent_)->GetTeam().FindKnowledgeOnObject( *(context.selectedElement_.pObject_) );
        if( pPopupKnowledge_ == 0 )
            return;
    }
    else
        pPopupKnowledge_ = context.selectedElement_.pObjectKnowledge_;

    popupMenu.insertItem( strMenuText_.c_str(), this, SLOT( AcceptPopupMenuKnowledge() ) );
}


// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledge::CheckValidity
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
bool ParamObjectKnowledge::CheckValidity()
{
    if( pKnowledge_ != 0 )
        return true;

    pLabel_->TurnRed( 3000 );
    return false;
}
    

// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledge::WriteMsg
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void ParamObjectKnowledge::WriteMsg( std::stringstream& strMsg )
{
    assert( pKnowledge_ != 0 && !IsOptional() );
    if( pKnowledge_ == 0 && IsOptional() )
        return;

    strMsg << pLabel_->text().latin1() << ": object knowledge #" << pKnowledge_->GetID();
    asnKnowledge_ = pKnowledge_->GetID();
}


// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledge::AcceptPopupMenuKnowledge
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void ParamObjectKnowledge::AcceptPopupMenuKnowledge()
{
    // The knowledge could have been deleted between the popup's appearance
    // and validation.
    if( pPopupKnowledge_ == 0 )
        return;

    pKnowledge_ = pPopupKnowledge_;
    pKnowledgeLabel_->setText( QString::number( pKnowledge_->GetID() ) );
    //$$$ Display name?
}


// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledge::OnObjectKnowledgeDeleted
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void ParamObjectKnowledge::OnObjectKnowledgeDeleted( Team& /*team*/, ObjectKnowledge& knowledge )
{
    if( pPopupKnowledge_ == &knowledge )
        pPopupKnowledge_ = 0;

    if( pKnowledge_ == &knowledge )
    {
        pKnowledge_ = 0;
        pKnowledgeLabel_->setText( "---" );
    }
}
