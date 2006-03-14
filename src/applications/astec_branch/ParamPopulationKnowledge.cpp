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
// $Archive: /MVW_v10/Build/SDK/Light2/src/ParamPopulationKnowledge.cpp $
// $Author: Ape $
// $Modtime: 21/07/04 11:34 $
// $Revision: 3 $
// $Workfile: ParamPopulationKnowledge.cpp $
//
// *****************************************************************************

#include "astec_pch.h"
#include "ParamPopulationKnowledge.h"
#include "moc_ParamPopulationKnowledge.cpp"

#include "App.h"
#include "Agent_ABC.h"
#include "Gtia.h"
#include "Team.h"
#include "ActionContext.h"
#include "PopulationKnowledge.h"
#include "Population.h"
#include "Agent.h"

// -----------------------------------------------------------------------------
// Name: ParamPopulationKnowledge constructor
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
ParamPopulationKnowledge::ParamPopulationKnowledge( ASN1T_KnowledgePopulation& asnKnowledge, Agent_ABC& agent, const std::string strLabel, const std::string strMenuText, QWidget* pParent, bool bOptional )
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

    connect( &App::GetApp(), SIGNAL( PopulationKnowledgeDeleted( Gtia&, PopulationKnowledge& ) ), this, SLOT( OnPopulationKnowledgeDeleted( Gtia&, PopulationKnowledge& ) ) );
}


// -----------------------------------------------------------------------------
// Name: ParamPopulationKnowledge destructor
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
ParamPopulationKnowledge::~ParamPopulationKnowledge()
{
}


// -----------------------------------------------------------------------------
// Name: ParamPopulationKnowledge::FillRemotePopupMenu
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void ParamPopulationKnowledge::FillRemotePopupMenu( QPopupMenu& popupMenu, const ActionContext& context )
{
    if(    context.selectedElement_.pPopulation_ == 0
        && context.selectedElement_.pPopulationKnowledge_ == 0 )
        return;

    if( context.selectedElement_.pPopulation_ != 0 )
    {
        // Disallow using knowledges on Populations from our own team.
        if( context.selectedElement_.pPopulation_->GetTeam().GetID() == dynamic_cast<Agent*>(&agent_)->GetTeam().GetID() )
            return;

        pPopupKnowledge_ = dynamic_cast<Agent*>(&agent_)->GetGtia().FindKnowledgeOnPopulation( *(context.selectedElement_.pPopulation_) );
        if( pPopupKnowledge_ == 0 )
            return;
    }
    else
        pPopupKnowledge_ = context.selectedElement_.pPopulationKnowledge_;

    popupMenu.insertItem( strMenuText_.c_str(), this, SLOT( AcceptPopupMenuKnowledge() ) );
}


// -----------------------------------------------------------------------------
// Name: ParamPopulationKnowledge::CheckValidity
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
bool ParamPopulationKnowledge::CheckValidity()
{
    if( pKnowledge_ != 0 )
        return true;

    pLabel_->TurnRed( 3000 );
    return false;
}
    

// -----------------------------------------------------------------------------
// Name: ParamPopulationKnowledge::WriteMsg
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void ParamPopulationKnowledge::WriteMsg( std::stringstream& strMsg )
{
    assert( pKnowledge_ != 0 && !IsOptional() );
    if( pKnowledge_ == 0 && IsOptional() )
        return;

    strMsg << pLabel_->text().latin1() << ": Population knowledge #" << pKnowledge_->GetID();
    asnKnowledge_ = pKnowledge_->GetID();
}


// -----------------------------------------------------------------------------
// Name: ParamPopulationKnowledge::AcceptPopupMenuKnowledge
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void ParamPopulationKnowledge::AcceptPopupMenuKnowledge()
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
// Name: ParamPopulationKnowledge::OnPopulationKnowledgeDeleted
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void ParamPopulationKnowledge::OnPopulationKnowledgeDeleted( Gtia& /*gtia*/, PopulationKnowledge& knowledge )
{
    if( pPopupKnowledge_ == &knowledge )
        pPopupKnowledge_ = 0;

    if( pKnowledge_ == &knowledge )
    {
        pKnowledge_ = 0;
        pKnowledgeLabel_->setText( "---" );
    }
}
