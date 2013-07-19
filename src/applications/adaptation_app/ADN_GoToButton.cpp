// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_GoToButton.h"
#include "moc_ADN_GoToButton.cpp"
#include "ADN_ComboBox.h"
#include "ADN_Workspace.h"

// -----------------------------------------------------------------------------
// Name: ADN_GoToButton constructor
// Created: ABR 2012-03-05
// -----------------------------------------------------------------------------
ADN_GoToButton::ADN_GoToButton( E_WorkspaceElements targetTab, int subTargetTab /* = -1 */, QWidget* parent /* = 0 */ )
    : QPushButton( qApp->style()->standardIcon( QStyle::SP_ArrowRight ), "", parent )
    , linkedCombo_( 0 )
{
    QToolTip::add( this, tr( "Go to" ) );
    setIconSize( QSize( 10, 10 ) );

    goToInfo_.targetName_ = "";
    goToInfo_.targetTab_ = targetTab;
    goToInfo_.subTargetTab_ = subTargetTab;

    connect( this, SIGNAL( clicked() ), SLOT( OnClicked() ) );
    connect( this, SIGNAL( GoToRequested( const ADN_NavigationInfos::GoTo& ) ), &ADN_Workspace::GetWorkspace(), SLOT( OnGoToRequested( const ADN_NavigationInfos::GoTo& ) ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_GoToButton destructor
// Created: ABR 2012-03-05
// -----------------------------------------------------------------------------
ADN_GoToButton::~ADN_GoToButton()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_GoToButton::SetLinkedCombo
// Created: ABR 2012-03-05
// -----------------------------------------------------------------------------
void ADN_GoToButton::SetLinkedCombo( ADN_ComboBox* linkedCombo )
{
    linkedCombo_ = linkedCombo;
}

// -----------------------------------------------------------------------------
// Name: ADN_GoToButton::OnClicked
// Created: ABR 2012-03-05
// -----------------------------------------------------------------------------
void ADN_GoToButton::OnClicked()
{
    if( !linkedCombo_ || linkedCombo_->currentText().isEmpty() )
        return;
    goToInfo_.targetName_ = linkedCombo_->currentText();
    assert( goToInfo_.targetTab_ != eNbrWorkspaceElements );
    emit( GoToRequested( goToInfo_ ) );
}
