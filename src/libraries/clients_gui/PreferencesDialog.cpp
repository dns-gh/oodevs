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
#include "ActiveViewComboBox.h"
#include "CoordinateSystemsPanel.h"
#include "ElevationPanel.h"
#include "RefreshRatePanel.h"
#include "GLMainProxy.h"
#include "GLOptions.h"
#include "GraphicsPanel.h"
#include "LayersPanel.h"
#include "LightingPanel.h"
#include "PreferencesList.h"
#include "resources.h"
#include "RichPushButton.h"
#include "SignalAdapter.h"
#include "SubObjectName.h"
#include "SymbolSizePanel.h"
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
                                      GLMainProxy& mainProxy )
    : ModalDialog( parent, "PreferencesDialog", false )
    , controllers_( controllers )
    , mainProxy_( mainProxy )
{
    // ActiveCombo needs be created before the preference dialog register its observer,
    // because we call UpdateComboVisibility below which is based on the number of item
    // in the active combo.
    activeCombo_ = new ActiveViewComboBox( mainProxy, "StatusActiveViewComboBox" );
    mainProxy_.AddActiveChangeObserver( this, [&]( const GLView_ABC::T_View& view ) {
        if( !isVisible() || !view )
            return;
        Load( *view );
    } );
    mainProxy_.AddCreationObserver( this, [&]( const GLView_ABC::T_View& view ) {
        if( !isVisible() || !view )
            return;
        previousViewsOptions_[ view->GetID() ] = std::unique_ptr< GLOptions >( new GLOptions( view->GetActiveOptions() ) );
        UpdateComboVisibility();
    } );
    mainProxy_.AddDeletionObserver( this, [&]( const GLView_ABC::T_View& view ) {
        if( !isVisible() || !view )
            return;
        previousViewsOptions_.erase( view->GetID() );
        UpdateComboVisibility();
    } );

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
    gui::connect( list_->selectionModel(), SIGNAL( currentChanged( const QModelIndex&, const QModelIndex& ) ), [ &]() {
        UpdateComboVisibility();
    } );

    RichPushButton* okButton = new RichPushButton( "ok", tr( "Ok" ) );
    RichPushButton* cancelButton = new RichPushButton( "cancel", tr( "Cancel" ) );
    okButton->setDefault( true );

    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->setMargin( 0 );
    topLayout->addWidget( title );
    topLayout->addStretch( 1 );
    topLayout->addWidget( activeCombo_ );
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
    AddPage( tr( "2D" ),                   true,  *new LayersPanel( this, options, mainProxy ) );
    AddPage( tr( "2D/Terrain" ),           true,  *new GraphicsPanel( this, options ) );
    AddPage( tr( "2D/Elevation" ),         true,  *new ElevationPanel( this, options, model.detection_ ) );
    AddPage( tr( "3D" ),                   true,  *new LightingPanel( this, options ) );
    AddPage( tr( "Coordinate System" ),    false, *new CoordinateSystemsPanel( this, options, model.coordinateConverter_ ) );
    AddPage( tr( "Refresh Rate" ),         false, *new RefreshRatePanel( this, options ) );
    AddPage( tr( "Symbol Sizes" ),         true,  *new SymbolSizePanel( this, options ) );
    AddPage( tr( "Visualisation Scales" ), true,  *new VisualisationScalesPanel( this, options ) );

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
void PreferencesDialog::AddPage( const QString& name,
                                 bool showActiveCombo,
                                 PreferencePanel_ABC& page )
{
    list_->AddPage( name, &page );
    stack_->addWidget( &page );
    panels_[ &page ] = showActiveCombo;
}

// -----------------------------------------------------------------------------
// Name: PreferencesDialog::showEvent
// Created: ABR 2012-05-14
// -----------------------------------------------------------------------------
void PreferencesDialog::showEvent( QShowEvent * event )
{
    previousGeneralOptions_.reset( new kernel::Options( *controllers_.options_.GetGeneralOptions() ) );
    previousViewsOptions_.clear();
    mainProxy_.ApplyToViews( [ &]( const GLView_ABC::T_View& view ) {
        previousViewsOptions_[ view->GetID() ] = std::unique_ptr< GLOptions >( new GLOptions( view->GetActiveOptions() ) );
    } );
    Load( mainProxy_ );
    UpdateComboVisibility();
    QDialog::showEvent( event );
}

namespace
{
    void RestoreOptions( kernel::Options& previousOptions,
                         const kernel::Options& currentOptions )
    {
        // Update our previous values with changes done outside the preferences panel
        currentOptions.Apply( [ &]( const std::string& name, const OptionVariant& value, bool isInPreferencePanel ) {
            if( !isInPreferencePanel )
                previousOptions.Set( name, value );
        } );
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
    // restore general options
    RestoreOptions( *previousGeneralOptions_, generalOptions );
    generalOptions = *previousGeneralOptions_;
    optionsController.UpdateGeneralOptions();
    previousGeneralOptions_.reset();
    // restore views options
    for( auto it = previousViewsOptions_.begin(); it != previousViewsOptions_.end(); ++it )
    {
        auto view = mainProxy_.GetView( it->first );
        if( !view )
            throw MASA_EXCEPTION( "Unable to restore previous options." );
        auto& previousOptions = *it->second;
        auto& options = view->GetActiveOptions();
        RestoreOptions( *previousOptions.GetOptions(), *options.GetOptions() );
        previousOptions.SetFilterEntity( options.GetFilterEntity() );
        previousOptions.SetLockedEntity( options.GetLockedEntity() );
        previousOptions.Disaggregate();
        const auto& entities = options.GetAggregatedEntities();
        for( auto entity = entities.begin(); entity != entities.end(); ++entity )
            previousOptions.Aggregate( **entity );
        options = previousOptions;
    }
    optionsController.UpdateViewOptions();
    mainProxy_.UpdateLayerOrder();
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
void PreferencesDialog::Load( const GLView_ABC& )
{
    try
    {
        for( auto it = panels_.begin(); it != panels_.end(); ++it )
            it->first->Load( mainProxy_ );
    }
    catch( std::exception& )
    {
        setVisible( false );
        throw;
    }
}

// -----------------------------------------------------------------------------
// Name: PreferencesDialog::UpdateComboVisibility
// Created: ABR 2014-08-06
// -----------------------------------------------------------------------------
void PreferencesDialog::UpdateComboVisibility()
{
    auto currentWidget = list_->GetCurrentWidget();
    activeCombo_->setVisible( activeCombo_->count() > 1 &&
                              currentWidget &&
                              panels_.at( currentWidget ) );
}
