// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

/* TRANSLATOR gui::PreferencesDialog */

#include "clients_gui_pch.h"
#include "PreferencesDialog.h"
#include "moc_PreferencesDialog.cpp"
#include "GraphicsPanel.h"
#include "GraphicPreferences.h"
#include "PreferencesList.h"
#include "PreferencePanel_ABC.h"
#include "CoordinateSystemsPanel.h"
#include "VisualisationScalesPanel.h"
#include "LightingPanel.h"
#include "LayersPanel.h"
#include "InhabitantPanel.h"
#include "resources.h"
#include "ElevationPanel.h"
#include "Elevation2dLayer.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: PreferencesDialog constructor
// Created: SBO 2006-05-03
// -----------------------------------------------------------------------------
PreferencesDialog::PreferencesDialog( QWidget* parent, Controllers& controllers, LightingProxy& lighting, kernel::CoordinateSystems& coordSystems,
                                      const Painter_ABC& painter )
    : QDialog( parent, "PreferencesDialog" )
    , controllers_      ( controllers )
    , painter_          ( painter )
    , pGraphicPrefPanel_( 0 )
{
    setCaption( tr( "Preferences" ) );
    QGridLayout* grid = new QGridLayout( this, 3, 2 );
    grid->setColStretch( 0, 0 );
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

    pGraphicPrefPanel_ = new GraphicsPanel( this, controllers );
    layersPanel_       = new LayersPanel( this, controllers );
    pCoordinateSystemsPanel_ = new CoordinateSystemsPanel( this, controllers, coordSystems );
    AddPage( tr( "Coordinate System" ), *pCoordinateSystemsPanel_ );
    AddPage( tr( "Visualisation Scales" ), *new VisualisationScalesPanel( this, controllers ) );
    AddPage( tr( "3D" ), *new LightingPanel( this, lighting, controllers ) );
    AddPage( tr( "2D" )        , *layersPanel_ );
    AddPage( tr( "2D/Terrain" ), *pGraphicPrefPanel_ );
    AddPage( tr( "2D/Population" ), *new InhabitantPanel( this, controllers ) );

    box = new QHBox( this );
    box->setMargin( 5 );
    box->setMaximumHeight( 40 );
    QPushButton* okBtn = new QPushButton( tr( "Ok" ), box );
    QButton* cancelBtn = new QPushButton( tr( "Cancel" ), box );
    okBtn->setDefault( true );
    grid->addWidget( box, 2, 1, Qt::AlignRight );

    connect( okBtn, SIGNAL( clicked() ), SLOT( OnOk() ) );
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
    return QSize( 500, 400 );
}

// -----------------------------------------------------------------------------
// Name: PreferencesDialog::show
// Created: RPD 2008-08-14
// -----------------------------------------------------------------------------
void PreferencesDialog::show()
{
    QDialog::show();
}

// -----------------------------------------------------------------------------
// Name: PreferencesDialog::reject
// Created: RPD 2008-08-22
// -----------------------------------------------------------------------------
void PreferencesDialog::reject()
{
    OnCancel();
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
    for( IT_Pages it = pages_.begin(); it != pages_.end(); ++it )
        (*it)->Commit();
    hide();
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
    AddPage( tr( "2D/Elevation" ), *new ElevationPanel( this, layer, controllers_, painter_ ) );
    layersPanel_->AddLayer( name, layer );
}
