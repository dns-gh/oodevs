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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ParamPopulationKnowledge.cpp $
// $Author: Ape $
// $Modtime: 21/07/04 11:34 $
// $Revision: 3 $
// $Workfile: MOS_ParamPopulationKnowledge.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_ParamPopulationKnowledge.h"
#include "moc_MOS_ParamPopulationKnowledge.cpp"

#include "MOS_App.h"
#include "MOS_Agent_ABC.h"
#include "MOS_Gtia.h"
#include "MOS_Team.h"
#include "MOS_ActionContext.h"
#include "MOS_PopulationKnowledge.h"
#include "MOS_Population.h"
#include "MOS_Agent.h"

// -----------------------------------------------------------------------------
// Name: MOS_ParamPopulationKnowledge constructor
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
MOS_ParamPopulationKnowledge::MOS_ParamPopulationKnowledge( ASN1T_KnowledgePopulation& asnKnowledge, MOS_Agent_ABC& agent, const std::string strLabel, const std::string strMenuText, QWidget* pParent, bool bOptional )
    : QHBox             ( pParent )
    , MOS_Param_ABC     ( bOptional )
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

    connect( &MOS_App::GetApp(), SIGNAL( PopulationKnowledgeDeleted( MOS_Gtia&, MOS_PopulationKnowledge& ) ), this, SLOT( OnPopulationKnowledgeDeleted( MOS_Gtia&, MOS_PopulationKnowledge& ) ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamPopulationKnowledge destructor
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
MOS_ParamPopulationKnowledge::~MOS_ParamPopulationKnowledge()
{
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamPopulationKnowledge::FillRemotePopupMenu
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void MOS_ParamPopulationKnowledge::FillRemotePopupMenu( QPopupMenu& popupMenu, const MOS_ActionContext& context )
{
    if(    context.selectedElement_.pPopulation_ == 0
        && context.selectedElement_.pPopulationKnowledge_ == 0 )
        return;

    if( context.selectedElement_.pPopulation_ != 0 )
    {
        // Disallow using knowledges on Populations from our own team.
        if( context.selectedElement_.pPopulation_->GetTeam().GetID() == dynamic_cast<MOS_Agent*>(&agent_)->GetTeam().GetID() )
            return;

        pPopupKnowledge_ = dynamic_cast<MOS_Agent*>(&agent_)->GetGtia().FindKnowledgeOnPopulation( *(context.selectedElement_.pPopulation_) );
        if( pPopupKnowledge_ == 0 )
            return;
    }
    else
        pPopupKnowledge_ = context.selectedElement_.pPopulationKnowledge_;

    popupMenu.insertItem( strMenuText_.c_str(), this, SLOT( AcceptPopupMenuKnowledge() ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamPopulationKnowledge::CheckValidity
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
bool MOS_ParamPopulationKnowledge::CheckValidity()
{
    if( pKnowledge_ != 0 )
        return true;

    pLabel_->TurnRed( 3000 );
    return false;
}
    

// -----------------------------------------------------------------------------
// Name: MOS_ParamPopulationKnowledge::WriteMsg
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void MOS_ParamPopulationKnowledge::WriteMsg( std::stringstream& strMsg )
{
    assert( pKnowledge_ != 0 && !IsOptional() );
    if( pKnowledge_ == 0 && IsOptional() )
        return;

    strMsg << pLabel_->text().latin1() << ": Population knowledge #" << pKnowledge_->GetID();
    asnKnowledge_ = pKnowledge_->GetID();
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamPopulationKnowledge::AcceptPopupMenuKnowledge
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void MOS_ParamPopulationKnowledge::AcceptPopupMenuKnowledge()
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
// Name: MOS_ParamPopulationKnowledge::OnPopulationKnowledgeDeleted
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void MOS_ParamPopulationKnowledge::OnPopulationKnowledgeDeleted( MOS_Gtia& /*gtia*/, MOS_PopulationKnowledge& knowledge )
{
    if( pPopupKnowledge_ == &knowledge )
        pPopupKnowledge_ = 0;

    if( pKnowledge_ == &knowledge )
    {
        pKnowledge_ = 0;
        pKnowledgeLabel_->setText( "---" );
    }
}
