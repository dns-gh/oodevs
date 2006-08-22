// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "PreferencesDialog.h"
#include "OptionsPanel.h"
#include "GraphicsPanel.h"
#include "GraphicPreferences.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: PreferencesDialog constructor
// Created: SBO 2006-05-03
// -----------------------------------------------------------------------------
PreferencesDialog::PreferencesDialog( QWidget* parent, Controllers& controllers )
    : QDialog( parent )
    , pGraphicPrefPanel_( 0 )
{
    setCaption( tr( "Préférences" ) );
    QVBoxLayout* pMainLayout = new QVBoxLayout( this );

    QTabWidget* tabWidget = new QTabWidget( this );
    tabWidget->addTab( new OptionsPanel( this, controllers ), tr( "Général" ) );
    pGraphicPrefPanel_ = new GraphicsPanel( this );
    tabWidget->addTab( pGraphicPrefPanel_, tr( "Terrain" ) );
    pMainLayout->addWidget( tabWidget );

    hide();
}
    
// -----------------------------------------------------------------------------
// Name: PreferencesDialog destructor
// Created: SBO 2006-05-03
// -----------------------------------------------------------------------------
PreferencesDialog::~PreferencesDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PreferencesDialog::sizeHint
// Created: SBO 2006-05-03
// -----------------------------------------------------------------------------
QSize PreferencesDialog::sizeHint() const
{
    return QSize( 500, 300 );
}

// -----------------------------------------------------------------------------
// Name: PreferencesDialog::GetPreferences
// Created: SBO 2006-05-04
// -----------------------------------------------------------------------------
GraphicPreferences& PreferencesDialog::GetPreferences() const
{
    if( !pGraphicPrefPanel_ )
        throw std::runtime_error( "Graphic preference panel not initialized" );
    return pGraphicPrefPanel_->GetPreferences();
}
