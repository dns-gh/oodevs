// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-04-19 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ParamLimits.cpp $
// $Author: Ape $
// $Modtime: 21/07/04 11:34 $
// $Revision: 6 $
// $Workfile: ParamLimits.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "ParamLimits.h"
#include "moc_ParamLimits.cpp"

#include "App.h"
#include "ActionContext.h"
#include "TacticalLine_ABC.h"
#include "LineManager.h"

// -----------------------------------------------------------------------------
// Name: ParamLimits constructor
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
ParamLimits::ParamLimits( ASN1T_OID& asnLimit1ID, ASN1T_OID& asnLimit2ID, const std::string strLabel, const std::string strMenuText1, const std::string strMenuText2, QWidget* pParent, bool bOptional )
    : QHBox          ( pParent)
    , Param_ABC  ( bOptional )
    , asnLimit1ID_   ( asnLimit1ID )
    , asnLimit2ID_   ( asnLimit2ID )
    , pLimit1_       ( 0 )
    , pLimit2_       ( 0 )
    , strMenuText1_  ( strMenuText1 )
    , strMenuText2_  ( strMenuText2 )
{
    asnLimit1ID = MIL_NULL_LINE_ID;
    asnLimit2ID = MIL_NULL_LINE_ID;

    pLabel_ = new MT_ParameterLabel( strLabel.c_str(), false, this, "" );

    pLimit1Label_ = new QLabel( "---", this );
    pLimit1Label_->setMinimumWidth( 50 );
    pLimit1Label_->setAlignment( Qt::AlignCenter );
    pLimit1Label_->setFrameStyle( QFrame::Box | QFrame::Sunken );

    pLimit2Label_ = new QLabel( "---", this );
    pLimit2Label_->setMinimumWidth( 50 );
    pLimit2Label_->setAlignment( Qt::AlignCenter );
    pLimit2Label_->setFrameStyle( QFrame::Box | QFrame::Sunken );

    QPushButton* pClearButton = new QPushButton( "X", this );
    pClearButton->setMaximumWidth( 15 );

    connect( &App::GetApp(), SIGNAL( TacticalLineDeleted( TacticalLine_ABC& ) ), this, SLOT( OnTacticalLineDeleted( TacticalLine_ABC& ) ) );
    connect( pClearButton, SIGNAL( clicked() ), this, SLOT( ClearSelectedLimits() ) );
}


// -----------------------------------------------------------------------------
// Name: ParamLimits destructor
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
ParamLimits::~ParamLimits()
{
}


// -----------------------------------------------------------------------------
// Name: ParamLimits::FillRemotePopupMenu
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
void ParamLimits::FillRemotePopupMenu( QPopupMenu& popupMenu, const ActionContext& context )
{
    if( context.selectedElement_.pLine_ == 0 || context.selectedElement_.pLine_->GetLineType() != TacticalLine_ABC::eLimit )
        return;

    pPopupMenuLimit_ = context.selectedElement_.pLine_;

    int nId;
    nId = popupMenu.insertItem( strMenuText1_.c_str(), this, SLOT( AcceptPopupMenuLimit( int ) ) );
    popupMenu.setItemParameter( nId, 1 );
    nId = popupMenu.insertItem( strMenuText2_.c_str(), this, SLOT( AcceptPopupMenuLimit( int ) ) );
    popupMenu.setItemParameter( nId, 2 );
}


// -----------------------------------------------------------------------------
// Name: ParamLimits::CheckValidity
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
bool ParamLimits::CheckValidity()
{
    if(   ( pLimit1_ == 0 && pLimit2_ == 0 )
       || ( pLimit1_ != 0 && pLimit2_ != 0 ) )
       return true;

    pLabel_->TurnRed( 3000 );
    return false;
}

// -----------------------------------------------------------------------------
// Name: ParamLimits::CheckValidityWhenRequired
// Created: SBO 2005-11-08
// -----------------------------------------------------------------------------
bool ParamLimits::CheckValidityWhenRequired()
{
    if( pLimit1_ != 0 && pLimit2_ != 0 )
        return true;
    pLabel_->TurnRed( 3000 );
    return false;
}

// -----------------------------------------------------------------------------
// Name: ParamLimits::WriteMsg
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
void ParamLimits::WriteMsg( std::stringstream& strMsg )
{
    if( pLimit1_ != 0 && pLimit2_ != 0 )
    {
        asnLimit1ID_ = pLimit1_->GetID();
        asnLimit2ID_ = pLimit2_->GetID();
        strMsg << pLabel_->text().latin1() << ": " << "#" << asnLimit1ID_ << " & #" << asnLimit2ID_;
    }
    else
    {
        asnLimit1ID_ = MIL_NULL_LINE_ID;
        asnLimit2ID_ = MIL_NULL_LINE_ID;
        strMsg << pLabel_->text().latin1() << ": ---";
    }
}


// -----------------------------------------------------------------------------
// Name: ParamLimits::UpdateLabels
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
void ParamLimits::UpdateLabels()
{
    if( pLimit1_ == 0 )
        pLimit1Label_->setText( "---" );
    else
        pLimit1Label_->setText( pLimit1_->GetName().c_str() );

    if( pLimit2_ == 0 )
        pLimit2Label_->setText( "---" );
    else
        pLimit2Label_->setText( pLimit2_->GetName().c_str() );
}


// -----------------------------------------------------------------------------
// Name: ParamLimits::AcceptPopupMenuLimit
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
void ParamLimits::AcceptPopupMenuLimit( int n )
{
    if( n == 1 )
        pLimit1_ = pPopupMenuLimit_;
    else
        pLimit2_ = pPopupMenuLimit_;

    this->UpdateLabels();
}


// -----------------------------------------------------------------------------
// Name: ParamLimits::OnTacticalLineDeleted
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void ParamLimits::OnTacticalLineDeleted( TacticalLine_ABC& line )
{
    if( pLimit1_ == &line )
        pLimit1_ = 0;
    if( pLimit2_ == &line )
        pLimit2_ = 0;

    this->UpdateLabels();
}


// -----------------------------------------------------------------------------
// Name: ParamLimits::ClearSelectedLimits
// Created: APE 2004-05-18
// -----------------------------------------------------------------------------
void ParamLimits::ClearSelectedLimits()
{
    pLimit1_ = 0;
    pLimit2_ = 0;
    this->UpdateLabels();
}
