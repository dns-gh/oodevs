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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ParamAgentKnowledge.cpp $
// $Author: Ape $
// $Modtime: 21/07/04 11:34 $
// $Revision: 3 $
// $Workfile: MOS_ParamAgentKnowledge.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_Pch.h"
#include "MOS_ParamAgentKnowledge.h"
#include "moc_MOS_ParamAgentKnowledge.cpp"

#include "MOS_App.h"
#include "MOS_Agent.h"
#include "MOS_Gtia.h"
#include "MOS_Team.h"
#include "MOS_ActionContext.h"
#include "MOS_AgentKnowledge.h"

#ifndef MOS_USE_INLINE
#   include "MOS_ParamAgentKnowledge.inl"
#endif


// -----------------------------------------------------------------------------
// Name: MOS_ParamAgentKnowledge constructor
/** @param  asnKnowledge 
    @param  strLabel 
    @param  strMenuText 
    @param  pParent 
*/
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
MOS_ParamAgentKnowledge::MOS_ParamAgentKnowledge( ASN1T_KnowledgeAgent& asnKnowledge, MOS_Agent& agent, const std::string strLabel, const std::string strMenuText, QWidget* pParent )
    : QHBox             ( pParent )
    , MOS_Param_ABC     ()
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

    connect( &MOS_App::GetApp(), SIGNAL( AgentKnowledgeDeleted( MOS_Gtia&, MOS_AgentKnowledge& ) ), this, SLOT( OnAgentKnowledgeDeleted( MOS_Gtia&, MOS_AgentKnowledge& ) ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamAgentKnowledge destructor
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
MOS_ParamAgentKnowledge::~MOS_ParamAgentKnowledge()
{
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamAgentKnowledge::FillRemotePopupMenu
/** @param  popupMenu 
    @param  context 
*/
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void MOS_ParamAgentKnowledge::FillRemotePopupMenu( QPopupMenu& popupMenu, const MOS_ActionContext& context )
{
    if(    context.selectedElement_.pAgent_ == 0
        && context.selectedElement_.pAgentKnowledge_ == 0 )
        return;

    if( context.selectedElement_.pAgent_ != 0 )
    {
        // Disallow using knowledges on agents from our own team.
        if( context.selectedElement_.pAgent_->GetTeam().GetID() == agent_.GetTeam().GetID() )
            return;

        pPopupKnowledge_ = agent_.GetGtia().FindKnowledgeOnAgent( *(context.selectedElement_.pAgent_) );
        if( pPopupKnowledge_ == 0 )
            return;
    }
    else
        pPopupKnowledge_ = context.selectedElement_.pAgentKnowledge_;

    popupMenu.insertItem( strMenuText_.c_str(), this, SLOT( AcceptPopupMenuKnowledge() ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamAgentKnowledge::CheckValidity
/** @return 
*/
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
bool MOS_ParamAgentKnowledge::CheckValidity()
{
    if( pKnowledge_ != 0 )
        return true;

    pLabel_->TurnRed( 3000 );
    return false;
}
    

// -----------------------------------------------------------------------------
// Name: MOS_ParamAgentKnowledge::WriteMsg
/** @param  strMsg 
*/
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void MOS_ParamAgentKnowledge::WriteMsg( std::stringstream& strMsg )
{
    assert( pKnowledge_ != 0 );
    strMsg << pLabel_->text().latin1() << ": agent knowledge #" << pKnowledge_->GetID();
    asnKnowledge_ = pKnowledge_->GetID();
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamAgentKnowledge::AcceptPopupMenuKnowledge
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void MOS_ParamAgentKnowledge::AcceptPopupMenuKnowledge()
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
// Name: MOS_ParamAgentKnowledge::OnAgentKnowledgeDeleted
/** @param  gtia 
    @param  knowledge 
*/
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void MOS_ParamAgentKnowledge::OnAgentKnowledgeDeleted( MOS_Gtia& /*gtia*/, MOS_AgentKnowledge& knowledge )
{
    if( pPopupKnowledge_ == &knowledge )
        pPopupKnowledge_ = 0;

    if( pKnowledge_ == &knowledge )
    {
        pKnowledge_ = 0;
        pKnowledgeLabel_->setText( "---" );
    }
}
