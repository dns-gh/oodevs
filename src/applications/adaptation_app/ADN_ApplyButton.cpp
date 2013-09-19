// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_ApplyButton.h"
#include "moc_ADN_ApplyButton.cpp"
#include "clients_kernel/Language.h"
#include "clients_kernel/LocalizedString.h"
#include "ADN_Workspace.h"
#include "ADN_Languages_GUI.h"

// -----------------------------------------------------------------------------
// Name: ADN_ApplyButton constructor
// Created: ABR 2013-07-16
// -----------------------------------------------------------------------------
ADN_ApplyButton::ADN_ApplyButton( QWidget* parent /*= 0*/ )
    : QPushButton( qApp->style()->standardIcon( QStyle::SP_DialogApplyButton ), "", parent )
{
    QToolTip::add( this, tr( "Mark as finished" ) );
    setCheckable( true );
    setIconSize( QSize( 10, 10 ) );
    connect( &ADN_Workspace::GetWorkspace().GetLanguages().GetGuiABC(), SIGNAL( LanguageChanged() ), this, SLOT( OnLanguageChanged() ) );
    connect( this, SIGNAL( clicked( bool ) ), this, SLOT( OnClicked( bool ) ) );
    OnLanguageChanged();
}

// -----------------------------------------------------------------------------
// Name: ADN_ApplyButton destructor
// Created: ABR 2013-07-16
// -----------------------------------------------------------------------------
ADN_ApplyButton::~ADN_ApplyButton()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_ApplyButton::Connect
// Created: ABR 2013-07-16
// -----------------------------------------------------------------------------
void ADN_ApplyButton::Connect( QWidget* widget )
{
    connect( this, SIGNAL( TypeChanged( int ) ), widget, SIGNAL( OnTypeChanged( int ) ) );
    connect( widget, SIGNAL( TypeChanged( int ) ), this, SLOT( OnTypeChanged( int ) ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_ApplyButton::OnClicked
// Created: ABR 2013-07-18
// -----------------------------------------------------------------------------
void ADN_ApplyButton::OnClicked( bool checked )
{
    emit TypeChanged( checked ? kernel::eTranslationType_None : kernel::eTranslationType_Unfinished );
}

// -----------------------------------------------------------------------------
// Name: ADN_ApplyButton::OnTypeChanged
// Created: ABR 2013-07-18
// -----------------------------------------------------------------------------
void ADN_ApplyButton::OnTypeChanged( int newType )
{
    setChecked( newType == kernel::eTranslationType_None );
}

// -----------------------------------------------------------------------------
// Name: ADN_ApplyButton::OnLanguageChanged
// Created: ABR 2013-07-17
// -----------------------------------------------------------------------------
void ADN_ApplyButton::OnLanguageChanged()
{
    setVisible( !kernel::Language::IsCurrentDefault() );
}
