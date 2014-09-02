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
#include "CoordinateSystemsPanel.h"
#include "ElevationPanel.h"
#include "Elevation2dLayer.h"
#include "RefreshRatePanel.h"
#include "GraphicsPanel.h"
#include "GraphicPreferences.h"
#include "InhabitantPanel.h"
#include "LayersPanel.h"
#include "LightingPanel.h"
#include "PreferencesList.h"
#include "PreferencePanel_ABC.h"
#include "resources.h"
#include "RichPushButton.h"
#include "SubObjectName.h"
#include "VisualisationScalesPanel.h"
#include "clients_kernel/ModeController.h"
#include "clients_kernel/Tools.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: PreferencesDialog constructor
// Created: SBO 2006-05-03
// -----------------------------------------------------------------------------
PreferencesDialog::PreferencesDialog( QWidget* parent,
                                      Controllers& controllers,
                                      LightingProxy& lighting,
                                      kernel::CoordinateConverter_ABC& coordConverter,
                                      const Painter_ABC& painter,
                                      GlSelector& selector,
                                      Elevation2dLayer& elevation2dLayer,
                                      GraphicPreferences& preferences )
    : ModalDialog( parent, "PreferencesDialog", false )
    , controllers_      ( controllers )
    , painter_          ( painter )
    , pGraphicPrefPanel_( 0 )
    , lighting_         ( lighting )
    , elevation2dLayer_ ( elevation2dLayer )
{
    SubObjectName subObject( "PreferencesDialog" );
    setCaption( tr( "Preferences" ) );
    QGridLayout* grid = new QGridLayout( this );
    grid->setColumnStretch( 0, 1 );
    grid->setColumnStretch( 1, 3 );
    grid->setRowStretch( 0, 1 );
    grid->setRowStretch( 1, 6 );
    grid->setRowStretch( 2, 1 );

    Q3HBox* box = new Q3HBox( this );
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
    box->setMaximumHeight( 64 );
    grid->addMultiCellWidget( box, 0, 0, 0, 1 );

    QStackedWidget* pages = new QStackedWidget( this );

    grid->addWidget( pages, 1, 1 );
    box = new Q3HBox( this );
    box->setMargin( 5 );
    list_ = new PreferencesList( "preferencesList", box, *pages );
    grid->addWidget( box, 1, 0 );

    pGraphicPrefPanel_       = new GraphicsPanel( this, preferences );
    layersPanel_             = new LayersPanel( this, controllers, selector );
    pCoordinateSystemsPanel_ = new CoordinateSystemsPanel( this, controllers, coordConverter );

    box = new Q3HBox( this );
    box->setMargin( 5 );
    box->setMaximumHeight( 40 );
    RichPushButton* okBtn = new RichPushButton( "ok", tr( "Ok" ), box );
    RichPushButton* cancelBtn = new RichPushButton( "cancel", tr( "Cancel" ), box );
    okBtn->setDefault( true );
    grid->addWidget( box, 2, 1, Qt::AlignRight );

    connect( okBtn, SIGNAL( clicked() ), SLOT( OnOk() ) );
    connect( cancelBtn, SIGNAL( clicked() ), SLOT( OnCancel() ) );
    connect( this, SIGNAL( OnAddRaster() ), parent, SLOT( OnAddRaster() ) );

    BuildSettings();
    hide();
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: PreferencesDialog destructor
// Created: SBO 2006-05-03
// -----------------------------------------------------------------------------
PreferencesDialog::~PreferencesDialog()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: PreferencesDialog::AddPage
// Created: SBO 2007-01-03
// -----------------------------------------------------------------------------
void PreferencesDialog::AddPage( const QString& name, PreferencePanel_ABC& page )
{
    list_->AddPage( name, &page );
    page.setFrameStyle( QFrame::NoFrame );
    pages_.push_back( &page );
}

// -----------------------------------------------------------------------------
// Name: PreferencesDialog::sizeHint
// Created: SBO 2006-05-03
// -----------------------------------------------------------------------------
QSize PreferencesDialog::sizeHint() const
{
    return QSize( 750, 550 );
}

// -----------------------------------------------------------------------------
// Name: PreferencesDialog::showEvent
// Created: ABR 2012-05-14
// -----------------------------------------------------------------------------
void PreferencesDialog::showEvent( QShowEvent * event )
{
    layersPanel_->Update();
    QDialog::showEvent( event );
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
// Name: PreferencesDialog::OnOk
// Created: SBO 2007-01-03
// -----------------------------------------------------------------------------
void PreferencesDialog::OnOk()
{
    for( auto it = pages_.begin(); it != pages_.end(); ++it )
        (*it)->Commit();
    hide();
}

// -----------------------------------------------------------------------------
// Name: PreferencesDialog::OnCancel
// Created: SBO 2007-01-03
// -----------------------------------------------------------------------------
void PreferencesDialog::OnCancel()
{
    for( auto it = pages_.begin(); it != pages_.end(); ++it )
        (*it)->Reset();
    hide();
}

// -----------------------------------------------------------------------------
// Name: PreferencesDialog::AddLayer
// Created: AGE 2007-01-04
// -----------------------------------------------------------------------------
void PreferencesDialog::AddLayer( const QString& name, gui::Layer& layer, bool dynamic /* = false */ )
{
    layersPanel_->AddLayer( name, layer, dynamic );
}

// -----------------------------------------------------------------------------
// Name: PreferencesDialog::PurgeDialog
// Created: NPT 2013-07-15
// -----------------------------------------------------------------------------
void PreferencesDialog::PurgeDialog()
{
    list_->Purge();
    pages_.clear();
}

// -----------------------------------------------------------------------------
// Name: PreferencesDialog::BuildSettings
// Created: NPT 2013-07-15
// -----------------------------------------------------------------------------
void PreferencesDialog::BuildSettings()
{
    AddPage( tools::translate( "PreferencesDialog", "Coordinate System" ), *pCoordinateSystemsPanel_ );
    AddPage( tools::translate( "PreferencesDialog", "Visualisation Scales" ), *new VisualisationScalesPanel( this, controllers_ ) );
    AddPage( tools::translate( "PreferencesDialog", "2D" ), *layersPanel_ );
    AddPage( tools::translate( "PreferencesDialog", "2D/Terrain" ), *pGraphicPrefPanel_ );
    AddPage( tools::translate( "PreferencesDialog", "2D/Population" ), *new InhabitantPanel( this, controllers_ ) );
    AddPage( tools::translate( "PreferencesDialog", "2D/Elevation" ), *new ElevationPanel( this, elevation2dLayer_, controllers_, painter_ ) );
    AddPage( tools::translate( "PreferencesDialog", "3D" ), *new LightingPanel( this, lighting_, controllers_ ) );
    AddPage( tools::translate( "PreferencesDialog", "Refresh rate" ), *new RefreshRatePanel( this, controllers_ ) );
}

// -----------------------------------------------------------------------------
// Name: PreferencesDialog::NotifyUpdated
// Created: ABR 2014-08-27
// -----------------------------------------------------------------------------
void PreferencesDialog::NotifyUpdated( const kernel::ModelUnLoaded& )
{
    OnCancel();
}
