// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-03-24 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ParamAgent.cpp $
// $Author: Ape $
// $Modtime: 21/07/04 11:34 $
// $Revision: 5 $
// $Workfile: ParamAgent.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "ParamAgent.h"
#include "moc_ParamAgent.cpp"

#include "Agent.h"
#include "ActionContext.h"

// -----------------------------------------------------------------------------
// Name: ParamAgent constructor
// Created: APE 2004-03-24
// -----------------------------------------------------------------------------
ParamAgent::ParamAgent( ASN1T_Agent& asnAgent, const std::string strLabel, const std::string strMenuText, QWidget* pParent, OptionalParamFunctor_ABC* pOptional )
    : QHBox         ( pParent )
    , Param_ABC ( pOptional )
    , strMenuText_  ( strMenuText )
    , asnAgent_     ( asnAgent )
    , pPopupAgent_  ( 0 )
    , pAgent_       ( 0 )
{
    pLabel_ = new MT_ParameterLabel( strLabel.c_str(), false, this, "" );

    pAgentLabel_ = new QLabel( "---", this );
    pAgentLabel_->setMinimumWidth( 100 );
    pAgentLabel_->setAlignment( Qt::AlignCenter );
    pAgentLabel_->setFrameStyle( QFrame::Box | QFrame::Sunken );
}


// -----------------------------------------------------------------------------
// Name: ParamAgent destructor
// Created: APE 2004-03-24
// -----------------------------------------------------------------------------
ParamAgent::~ParamAgent()
{
}


// -----------------------------------------------------------------------------
// Name: ParamAgent::FillRemotePopupMenu
// Created: APE 2004-03-24
// -----------------------------------------------------------------------------
void ParamAgent::FillRemotePopupMenu( QPopupMenu& popupMenu, const ActionContext& context )
{
    if( context.selectedElement_.pAgent_ == 0 )
        return;

    pPopupAgent_ = context.selectedElement_.pAgent_;

    popupMenu.insertItem( strMenuText_.c_str(), this, SLOT( AcceptPopupMenuAgent() ) );
}


// -----------------------------------------------------------------------------
// Name: ParamAgent::CheckValidity
// Created: APE 2004-03-24
// -----------------------------------------------------------------------------
bool ParamAgent::CheckValidity()
{
    if( pAgent_ != 0 || IsOptional() )
        return true;

    pLabel_->TurnRed( 3000 );
    return false;
}


// -----------------------------------------------------------------------------
// Name: ParamAgent::WriteMsg
// Created: APE 2004-03-24
// -----------------------------------------------------------------------------
void ParamAgent::WriteMsg( std::stringstream& strMsg )
{
    if( pAgent_ )
        return;

    if( pOptional_ )
        pOptional_->SetOptionalPresent();

    strMsg << pLabel_->text().latin1() << ": agent #" << pAgent_->GetID();
    asnAgent_ = pAgent_->GetID();
}


// -----------------------------------------------------------------------------
// Name: ParamAgent::AcceptPopupMenuAgent
// Created: APE 2004-03-24
// -----------------------------------------------------------------------------
void ParamAgent::AcceptPopupMenuAgent()
{
    assert( pPopupAgent_ != 0 );
    pAgent_ = pPopupAgent_;
    pAgentLabel_->setText( pAgent_->GetName().c_str() );
}
