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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ParamObjectKnowledge.cpp $
// $Author: Ape $
// $Modtime: 20/09/04 17:07 $
// $Revision: 5 $
// $Workfile: MOS_ParamObjectKnowledge.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_ParamObjectKnowledge.h"
#include "moc_MOS_ParamObjectKnowledge.cpp"

#include "MOS_App.h"
#include "MOS_Agent.h"
#include "MOS_Team.h"
#include "MOS_ActionContext.h"
#include "MOS_ObjectKnowledge.h"

#ifndef MOS_USE_INLINE
#   include "MOS_ParamObjectKnowledge.inl"
#endif


// -----------------------------------------------------------------------------
// Name: MOS_ParamObjectKnowledge constructor
/** @param  asnKnowledge 
    @param  strLabel 
    @param  strMenuText 
    @param  pParent 
*/
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
MOS_ParamObjectKnowledge::MOS_ParamObjectKnowledge( ASN1T_KnowledgeObject& asnKnowledge, MOS_Agent& agent, const std::string strLabel, const std::string strMenuText, QWidget* pParent )
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

    connect( &MOS_App::GetApp(), SIGNAL( ObjectKnowledgeDeleted( MOS_Team&, MOS_ObjectKnowledge& ) ), this, SLOT( OnObjectKnowledgeDeleted( MOS_Team&, MOS_ObjectKnowledge& ) ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamObjectKnowledge destructor
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
MOS_ParamObjectKnowledge::~MOS_ParamObjectKnowledge()
{
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamObjectKnowledge::FillRemotePopupMenu
/** @param  popupMenu 
    @param  context 
*/
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void MOS_ParamObjectKnowledge::FillRemotePopupMenu( QPopupMenu& popupMenu, const MOS_ActionContext& context )
{
    if( context.selectedElement_.pObjectKnowledge_ == 0 && context.selectedElement_.pObject_ == 0 )
        return;

    if( context.selectedElement_.pObject_ != 0 )
    {
        pPopupKnowledge_ = agent_.GetTeam().FindKnowledgeOnObject( *(context.selectedElement_.pObject_) );
        if( pPopupKnowledge_ == 0 )
            return;
    }
    else
        pPopupKnowledge_ = context.selectedElement_.pObjectKnowledge_;

    popupMenu.insertItem( strMenuText_.c_str(), this, SLOT( AcceptPopupMenuKnowledge() ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamObjectKnowledge::CheckValidity
/** @return 
*/
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
bool MOS_ParamObjectKnowledge::CheckValidity()
{
    if( pKnowledge_ != 0 )
        return true;

    pLabel_->TurnRed( 3000 );
    return false;
}
    

// -----------------------------------------------------------------------------
// Name: MOS_ParamObjectKnowledge::WriteMsg
/** @param  strMsg 
*/
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void MOS_ParamObjectKnowledge::WriteMsg( std::stringstream& strMsg )
{
    assert( pKnowledge_ != 0 );
    strMsg << pLabel_->text().latin1() << ": object knowledge #" << pKnowledge_->GetID();
    asnKnowledge_ = pKnowledge_->GetID();
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamObjectKnowledge::AcceptPopupMenuKnowledge
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void MOS_ParamObjectKnowledge::AcceptPopupMenuKnowledge()
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
// Name: MOS_ParamObjectKnowledge::OnObjectKnowledgeDeleted
/** @param  gtia 
    @param  knowledge 
*/
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void MOS_ParamObjectKnowledge::OnObjectKnowledgeDeleted( MOS_Team& /*team*/, MOS_ObjectKnowledge& knowledge )
{
    if( pPopupKnowledge_ == &knowledge )
        pPopupKnowledge_ = 0;

    if( pKnowledge_ == &knowledge )
    {
        pKnowledge_ = 0;
        pKnowledgeLabel_->setText( "---" );
    }
}
