// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "PreferencesDialog.h"
#include "moc_PreferencesDialog.cpp"
#include "CoordinateSystemsPanel.h"
#include "ElevationPanel.h"
#include "RefreshRatePanel.h"
#include "GLOptions.h"
#include "GlProxy.h"
#include "GraphicsPanel.h"
#include "LayersPanel.h"
#include "LightingPanel.h"
#include "PreferencesList.h"
#include "resources.h"
#include "RichPushButton.h"
#include "SubObjectName.h"
#include "TerrainSettings.h"
#include "VisualisationScalesPanel.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/OptionsController.h"
#include "clients_kernel/StaticModel.h"
#include "clients_kernel/Options.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: PreferencesDialog constructor
// Created: SBO 2006-05-03
// -----------------------------------------------------------------------------
PreferencesDialog::PreferencesDialog( QWidget* parent,
                                      Controllers& controllers,
                                      const kernel::StaticModel& model,
                                      GlProxy& proxy )
    : ModalDialog( parent, "PreferencesDialog", false )
    , controllers_( controllers )
    , proxy_( proxy )
{
    SubObjectName subObject( "PreferencesDialog" );
    setCaption( tr( "Preferences" ) );
    setMinimumSize( 600, 600 );
    hide();

    QLabel* title = new QLabel( caption() );
    QFont font;
    font.setBold( true );
    font.setPointSize( 16 );
    title->setFont( font );
    title->setMargin( 10 );
    title->setBackgroundColor( Qt::white );

    QLabel* icon = new QLabel();
    icon->setPixmap( MAKE_PIXMAP( option_general ) );
    icon->setMaximumWidth( 64 );
    icon->setBackgroundColor( Qt::white );

    stack_ = new QStackedWidget();
    list_ = new PreferencesList( "preferencesList", *stack_ );

    RichPushButton* okButton = new RichPushButton( "ok", tr( "Ok" ) );
    RichPushButton* cancelButton = new RichPushButton( "cancel", tr( "Cancel" ) );
    okButton->setDefault( true );

    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->setMargin( 0 );
    topLayout->addWidget( title );
    topLayout->addStretch( 1 );
    topLayout->addWidget( icon );

    QHBoxLayout* contentLayout = new QHBoxLayout();
    contentLayout->addWidget( list_, 1 );
    contentLayout->addWidget( stack_, 3 );

    QHBoxLayout* bottomLayout = new QHBoxLayout();
    bottomLayout->setMargin( 0 );
    bottomLayout->addStretch( 1 );
    bottomLayout->addWidget( okButton );
    bottomLayout->addWidget( cancelButton );

    QVBoxLayout* layout = new QVBoxLayout( this );
    layout->addLayout( topLayout );
    layout->addLayout( contentLayout, 1 );
    layout->addLayout( bottomLayout );

    auto& options = controllers.options_;
    // common pages between preparation and gaming
    AddPage( tr( "2D" ),                   *new LayersPanel( this, options, proxy ) );
    AddPage( tr( "2D/Terrain" ),           *new GraphicsPanel( this, options ) );
    AddPage( tr( "2D/Elevation" ),         *new ElevationPanel( this, options, model.detection_ ) );
    AddPage( tr( "3D" ),                   *new LightingPanel( this, options ) );
    AddPage( tr( "Coordinate System" ),    *new CoordinateSystemsPanel( this, options, model.coordinateConverter_ ) );
    AddPage( tr( "Refresh rate" ),         *new RefreshRatePanel( this, options ) );
    AddPage( tr( "Visualisation Scales" ), *new VisualisationScalesPanel( this, options ) );

    connect( okButton, SIGNAL( clicked() ), SLOT( accept() ) );
    connect( cancelButton, SIGNAL( clicked() ), SLOT( reject() ) );
    connect( this, SIGNAL( OnAddRaster() ), parent, SLOT( OnAddRaster() ) );

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
    stack_->addWidget( &page );
    panels_.push_back( &page );
}

// -----------------------------------------------------------------------------
// Name: PreferencesDialog::showEvent
// Created: ABR 2012-05-14
// -----------------------------------------------------------------------------
void PreferencesDialog::showEvent( QShowEvent * event )
{
    previousGeneralOptions_ = std::make_shared< kernel::Options >( *controllers_.options_.GetGeneralOptions() );
    previousViewOptions_.reset( new GLOptions( proxy_.GetOptions() ) );
    Load( proxy_ );
    QDialog::showEvent( event );
}

namespace
{
    void RestoreOptions( kernel::Options& previousOptions,
                         kernel::Options& currentOptions )
    {
        std::vector< std::string > toDelete;
        // Update our previous values with changes done outside the preferences panel
        // and remove the current values that were added.
        currentOptions.Apply( [ &]( const std::string& name, const OptionVariant& value, bool isInPreferencePanel ) {
            if( !previousOptions.Has( name ) )
                toDelete.push_back( name );
            else if( !isInPreferencePanel )
                previousOptions.Set( name, value );

        } );
        std::for_each( toDelete.begin(), toDelete.end(), [ &]( const std::string& name ) { currentOptions.Remove( name ); } );
    }
}

// -----------------------------------------------------------------------------
// Name: PreferencesDialog::reject
// Created: RPD 2008-08-22
// -----------------------------------------------------------------------------
void PreferencesDialog::reject()
{
    auto& optionsController = controllers_.options_;
    auto& generalOptions = *optionsController.GetGeneralOptions();
    auto& viewOptions = *optionsController.GetViewOptions();

    RestoreOptions( *previousViewOptions_->GetOptions(), viewOptions );
    proxy_.GetOptions() = *previousViewOptions_;
    optionsController.UpdateViewOptions();
    previousViewOptions_.reset();

    RestoreOptions( *previousGeneralOptions_, generalOptions );
    generalOptions = *previousGeneralOptions_;
    optionsController.UpdateGeneralOptions();
    previousGeneralOptions_.reset();

    proxy_.UpdateLayerOrder( viewOptions );
    proxy_.GetOptions().Load();
    ModalDialog::reject();
}

// -----------------------------------------------------------------------------
// Name: PreferencesDialog::NotifyUpdated
// Created: ABR 2014-08-27
// -----------------------------------------------------------------------------
void PreferencesDialog::NotifyUpdated( const kernel::ModelUnLoaded& )
{
    if( isVisible() )
        reject();
}

// -----------------------------------------------------------------------------
// Name: PreferencesDialog::Load
// Created: ABR 2014-10-01
// -----------------------------------------------------------------------------
void PreferencesDialog::Load( const GlProxy& )
{
    try
    {
        for( auto it = panels_.begin(); it != panels_.end(); ++it )
            ( *it )->Load( proxy_ );
    }
    catch( std::exception& )
    {
        setVisible( false );
        throw;
    }
}
