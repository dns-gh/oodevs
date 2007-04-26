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
#include "moc_PreferencesDialog.cpp"
#include "OptionsPanel.h"
#include "GraphicsPanel.h"
#include "GraphicPreferences.h"
#include "PreferencesList.h"
#include "PreferencePanel_ABC.h"
#include "Graphics3dPanel.h"
#include "LightingPanel.h"
#include "LayersPanel.h"
#include "resources.h"
#include "ElevationPanel.h"
#include "Elevation2dLayer.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: PreferencesDialog constructor
// Created: SBO 2006-05-03
// -----------------------------------------------------------------------------
PreferencesDialog::PreferencesDialog( QWidget* parent, Controllers& controllers, LightingProxy& lighting )
    : QDialog( parent )
    , pGraphicPrefPanel_( 0 )
{
    setCaption( tr( "Preferences" ) );
    QGridLayout* grid = new QGridLayout( this, 3, 2 );
    grid->setColStretch( 0, 1 );
    grid->setColStretch( 1, 3 );
    grid->setRowStretch( 0, 1 );
    grid->setRowStretch( 1, 5 );
    grid->setRowStretch( 2, 1 );

    QHBox* box = new QHBox( this );
    QLabel* title = new QLabel( caption(), box );
    QFont font;
    font.setBold( true );
    font.setPointSize( 16 );
    title->setFont( font );
    title->setMargin( 10 );
    title->setBackgroundColor( Qt::white );
    QLabel* icon = new QLabel( box );
    icon->setPixmap( MAKE_PIXMAP( option_general ) );
    icon->setMaximumWidth( 64 );
    icon->setBackgroundColor( Qt::white );
    grid->addMultiCellWidget( box, 0, 0, 0, 1 );

    QWidgetStack* pages = new QWidgetStack( this );
    pages->setMargin( 5 );
    grid->addWidget( pages, 1, 1 );
    
    box = new QHBox( this );
    box->setMargin( 5 );
    list_ = new PreferencesList( box, *pages );
    grid->addWidget( box, 1, 0 );

    pGraphicPrefPanel_ = new GraphicsPanel( this );
    layersPanel_       = new LayersPanel( this, controllers );
    AddPage( tr( "3D" ), *new Graphics3dPanel( this ) );
    AddPage( tr( "3D/Lighting" ), *new LightingPanel( this, lighting, controllers ) );
    AddPage( tr( "2D" )        , *layersPanel_ ); 
    AddPage( tr( "2D/Terrain" ), *pGraphicPrefPanel_ );
    AddPage( tr( "General" ), *new OptionsPanel( this, controllers ) );    

    box = new QHBox( this );
    box->setMargin( 5 );
    box->setMaximumHeight( 40 );
    QPushButton* okBtn = new QPushButton( tr( "Ok" ), box );
    QButton* applyBtn = new QPushButton( tr( "Apply" ), box );
    QButton* cancelBtn = new QPushButton( tr( "Cancel" ), box );
    okBtn->setDefault( true );
    grid->addWidget( box, 2, 1, Qt::AlignRight );

    connect( okBtn, SIGNAL( clicked() ), SLOT( OnOk() ) );
    connect( applyBtn, SIGNAL( clicked() ), SLOT( OnApply() ) );
    connect( cancelBtn, SIGNAL( clicked() ), SLOT( OnCancel() ) );

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
// Name: PreferencesDialog::AddPage
// Created: SBO 2007-01-03
// -----------------------------------------------------------------------------
void PreferencesDialog::AddPage( const QString& name, PreferencePanel_ABC& page )
{
    list_->AddPage( name, &page );
    pages_.push_back( &page );
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

// -----------------------------------------------------------------------------
// Name: PreferencesDialog::OnOk
// Created: SBO 2007-01-03
// -----------------------------------------------------------------------------
void PreferencesDialog::OnOk()
{
    OnApply();
    hide();
}
    
// -----------------------------------------------------------------------------
// Name: PreferencesDialog::OnApply
// Created: SBO 2007-01-03
// -----------------------------------------------------------------------------
void PreferencesDialog::OnApply()
{
    for( IT_Pages it = pages_.begin(); it != pages_.end(); ++it )
        (*it)->Commit();    
}
    
// -----------------------------------------------------------------------------
// Name: PreferencesDialog::OnCancel
// Created: SBO 2007-01-03
// -----------------------------------------------------------------------------
void PreferencesDialog::OnCancel()
{
    for( IT_Pages it = pages_.begin(); it != pages_.end(); ++it )
        (*it)->Reset();
    hide();
}

// -----------------------------------------------------------------------------
// Name: PreferencesDialog::AddLayer
// Created: AGE 2007-01-04
// -----------------------------------------------------------------------------
void PreferencesDialog::AddLayer( const QString& name, gui::Layer_ABC& layer )
{
    layersPanel_->AddLayer( name, layer );
}

// -----------------------------------------------------------------------------
// Name: PreferencesDialog::AddLayer
// Created: AGE 2007-01-17
// -----------------------------------------------------------------------------
void PreferencesDialog::AddLayer( const QString& name, gui::Elevation2dLayer& layer )
{
    AddPage( tr( "2D/Elevation" ), *new ElevationPanel( this, layer ) );
    layersPanel_->AddLayer( name, layer );
}
