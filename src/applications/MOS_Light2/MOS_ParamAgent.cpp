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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ParamAgent.cpp $
// $Author: Ape $
// $Modtime: 21/07/04 11:34 $
// $Revision: 5 $
// $Workfile: MOS_ParamAgent.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_ParamAgent.h"
#include "moc_MOS_ParamAgent.cpp"

#include "MOS_Agent.h"
#include "MOS_ActionContext.h"

// -----------------------------------------------------------------------------
// Name: MOS_ParamAgent constructor
/** @param  asnAgent 
    @param  strName 
    @param  pParent 
*/
// Created: APE 2004-03-24
// -----------------------------------------------------------------------------
MOS_ParamAgent::MOS_ParamAgent( ASN1T_Agent& asnAgent, const std::string strLabel, const std::string strMenuText, QWidget* pParent )
    : QHBox         ( pParent )
    , MOS_Param_ABC ()
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
// Name: MOS_ParamAgent destructor
// Created: APE 2004-03-24
// -----------------------------------------------------------------------------
MOS_ParamAgent::~MOS_ParamAgent()
{
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamAgent::FillRemotePopupMenu
/** @param  popupMenu 
    @param  pAgent 
    @param  pPoint 
*/
// Created: APE 2004-03-24
// -----------------------------------------------------------------------------
void MOS_ParamAgent::FillRemotePopupMenu( QPopupMenu& popupMenu, const MOS_ActionContext& context )
{
    if( context.selectedElement_.pAgent_ == 0 )
        return;

    pPopupAgent_ = context.selectedElement_.pAgent_;

    popupMenu.insertItem( strMenuText_.c_str(), this, SLOT( AcceptPopupMenuAgent() ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamAgent::CheckValidity
/** @return 
*/
// Created: APE 2004-03-24
// -----------------------------------------------------------------------------
bool MOS_ParamAgent::CheckValidity()
{
    if( pAgent_ != 0 )
        return true;

    pLabel_->TurnRed( 3000 );
    return false;
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamAgent::WriteMsg
/** @param  sParam 
*/
// Created: APE 2004-03-24
// -----------------------------------------------------------------------------
void MOS_ParamAgent::WriteMsg( std::stringstream& strMsg )
{
    strMsg << pLabel_->text().latin1() << ": agent #" << pAgent_->GetAgentID();
    asnAgent_ = pAgent_->GetAgentID();
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamAgent::AcceptPopupMenuAgent
// Created: APE 2004-03-24
// -----------------------------------------------------------------------------
void MOS_ParamAgent::AcceptPopupMenuAgent()
{
    assert( pPopupAgent_ != 0 );
    pAgent_ = pPopupAgent_;
    pAgentLabel_->setText( pAgent_->GetName().c_str() );
}
