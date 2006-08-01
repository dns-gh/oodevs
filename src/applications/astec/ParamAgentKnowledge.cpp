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
// $Archive: /MVW_v10/Build/SDK/Light2/src/ParamAgentKnowledge.cpp $
// $Author: Ape $
// $Modtime: 21/07/04 11:34 $
// $Revision: 3 $
// $Workfile: ParamAgentKnowledge.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "ParamAgentKnowledge.h"
#include "moc_ParamAgentKnowledge.cpp"

#include "App.h"
#include "Agent_ABC.h"
#include "Gtia.h"
#include "Team.h"
#include "ActionContext.h"
#include "AgentKnowledge.h"
#include "Agent.h"
#include "OptionalParamFunctor_ABC.h"

// -----------------------------------------------------------------------------
// Name: ParamAgentKnowledge constructor
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
ParamAgentKnowledge::ParamAgentKnowledge( ASN1T_KnowledgeAgent& asnKnowledge, Agent_ABC& agent, const std::string strLabel, const std::string strMenuText, QWidget* pParent, OptionalParamFunctor_ABC* pOptional )
    : QHBox             ( pParent )
    , Param_ABC     ( pOptional )
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

    connect( &App::GetApp(), SIGNAL( AgentKnowledgeDeleted( Gtia&, AgentKnowledge& ) ), this, SLOT( OnAgentKnowledgeDeleted( Gtia&, AgentKnowledge& ) ) );
}


// -----------------------------------------------------------------------------
// Name: ParamAgentKnowledge destructor
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
ParamAgentKnowledge::~ParamAgentKnowledge()
{
}


// -----------------------------------------------------------------------------
// Name: ParamAgentKnowledge::FillRemotePopupMenu
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void ParamAgentKnowledge::FillRemotePopupMenu( QPopupMenu& popupMenu, const ActionContext& context )
{
    if(    context.selectedElement_.pAgent_ == 0
        && context.selectedElement_.pAgentKnowledge_ == 0 )
        return;

    if( context.selectedElement_.pAgent_ != 0 )
    {
        // Disallow using knowledges on agents from our own team.
        if( context.selectedElement_.pAgent_->GetTeam().GetID() == dynamic_cast<Agent*>(&agent_)->GetTeam().GetID() )
            return;

        pPopupKnowledge_ = dynamic_cast<Agent*>(&agent_)->GetGtia().FindKnowledgeOnAgent( *(context.selectedElement_.pAgent_) );
        if( pPopupKnowledge_ == 0 )
            return;
    }
    else
        pPopupKnowledge_ = context.selectedElement_.pAgentKnowledge_;

    popupMenu.insertItem( strMenuText_.c_str(), this, SLOT( AcceptPopupMenuKnowledge() ) );
}


// -----------------------------------------------------------------------------
// Name: ParamAgentKnowledge::CheckValidity
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
bool ParamAgentKnowledge::CheckValidity()
{
    if( pKnowledge_ != 0 || IsOptional() )
        return true;

    pLabel_->TurnRed( 3000 );
    return false;
}
    

// -----------------------------------------------------------------------------
// Name: ParamAgentKnowledge::WriteMsg
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void ParamAgentKnowledge::WriteMsg( std::stringstream& strMsg )
{
    if( pKnowledge_ == 0 )
        return;

    if( pOptional_ )
        pOptional_->SetOptionalPresent();

    strMsg << pLabel_->text().latin1() << ": agent knowledge #" << pKnowledge_->GetID();
    asnKnowledge_ = pKnowledge_->GetID();
}


// -----------------------------------------------------------------------------
// Name: ParamAgentKnowledge::AcceptPopupMenuKnowledge
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void ParamAgentKnowledge::AcceptPopupMenuKnowledge()
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
// Name: ParamAgentKnowledge::OnAgentKnowledgeDeleted
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void ParamAgentKnowledge::OnAgentKnowledgeDeleted( Gtia& /*gtia*/, AgentKnowledge& knowledge )
{
    if( pPopupKnowledge_ == &knowledge )
        pPopupKnowledge_ = 0;

    if( pKnowledge_ == &knowledge )
    {
        pKnowledge_ = 0;
        pKnowledgeLabel_->setText( "---" );
    }
}
