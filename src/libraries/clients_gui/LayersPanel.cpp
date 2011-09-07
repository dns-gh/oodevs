// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

/* TRANSLATOR gui::LayersPanel */

#include "clients_gui_pch.h"
#include "LayersPanel.h"
#include "moc_LayersPanel.cpp"
#include "Layer_ABC.h"
#include "CheckBox.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/OptionVariant.h"
#include "ValuedListItem.h"
#include "resources.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: LayersPanel constructor
// Created: AGE 2007-01-04
// -----------------------------------------------------------------------------
LayersPanel::LayersPanel( QWidget* parent, kernel::Controllers& controllers )
    : PreferencePanel_ABC( parent, "LayersPanel" )
    , controllers_       ( controllers )
    , options_           ( controllers.options_ )
    , currentLayer_      ( -1 )
{
    Q3VBox* container = new Q3VBox( this );
    {
        Q3GroupBox* box = new Q3GroupBox( 1, Qt::Horizontal, tr( "Fog of war" ), container );
        fogOfWar_ = new CheckBox( tr( "Display fog of war" ), box );
        connect( fogOfWar_, SIGNAL( toggled( bool ) ), SLOT( OnFogOfWarChanged( bool ) ) );
    }

    {
        Q3GroupBox* box = new Q3GroupBox( 2, Qt::Horizontal, tr( "Infrastructures" ), container );
        infra_ = new CheckBox( tr( "Display Infrastructures" ), box );
        connect( infra_, SIGNAL( toggled( bool ) ), SLOT( OnInfraChanged( bool ) ) );
    }

    {
        Q3GroupBox* groupBox = new Q3GroupBox( 1, Qt::Horizontal, tr( "Layer display order and transparency" ), container );
        Q3VBox* vBox = new Q3VBox( groupBox );
        vBox->setSpacing( 6 );
        Q3HBox* box = new Q3HBox( vBox );
        box->setSpacing( 5 );
        layersList_ = new Q3ListView( box );
        layersList_->addColumn( tr( "Layer" ) );
        layersList_->setResizeMode( Q3ListView::LastColumn );
        layersList_->header()->hide();
        layersList_->setSorting( -1 );
        layersList_->setBackgroundColor( Qt::white );
        connect( layersList_, SIGNAL( selectionChanged( Q3ListViewItem * ) ), SLOT( OnSelectionChanged( Q3ListViewItem * ) ) );

        Q3VBox* buttonBox = new Q3VBox( box );
        buttonBox->layout()->setAlignment( Qt::AlignVCenter );
        QPushButton* up   = new QPushButton( MAKE_PIXMAP( arrow_up ), QString::null, buttonBox );
        up->setFixedSize( 32, 32 );
        QToolTip::add( up, tr( "Move the selected layer forwards" ) );
        connect( up, SIGNAL( clicked() ), SLOT( OnUp() ) );
        QPushButton* down = new QPushButton( MAKE_PIXMAP( arrow_down ), QString::null, buttonBox );
        down->setFixedSize( 32, 32 );
        QToolTip::add( down, tr( "Move the selected layer backwards" ) );
        connect( down, SIGNAL( clicked() ), SLOT( OnDown() ) );

        transparencyLabel_ = new QLabel( tr( "Transparency " ), vBox );
        transparency_ = new QSlider( 0, 100, 1, 100, Qt::Horizontal, vBox );
        transparency_->setMaximumWidth( 258 );
        connect( transparency_, SIGNAL( valueChanged( int ) ), SLOT( OnValueChanged() ) );
    }
    setWidget( container );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: LayersPanel destructor
// Created: AGE 2007-01-04
// -----------------------------------------------------------------------------
LayersPanel::~LayersPanel()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: LayersPanel::AddLayer
// Created: AGE 2007-01-04
// -----------------------------------------------------------------------------
void LayersPanel::AddLayer( const QString& name, Layer_ABC& layer )
{
    ValuedListItem* item = new ValuedListItem( layersList_ );
    item->SetValue( &layer );
    item->setText( 0, "  "+name );

    layers_       .push_back( &layer );
    currentLayers_.push_back( &layer );
    newLayers_    .push_back( &layer );
    current_.push_back( 1 );
    new_    .push_back( 1 );
    names_  .push_back( name.ascii() );
}

// -----------------------------------------------------------------------------
// Name: LayersPanel::Commit
// Created: AGE 2007-01-04
// -----------------------------------------------------------------------------
void LayersPanel::Commit()
{
    fogOfWar_->Commit();
    infra_->Commit();
    current_       = new_;
    currentLayers_ = newLayers_;
    for( unsigned i = 0; i < layers_.size(); ++i )
    {
        options_.Change( std::string( "Layers/" ) + names_[ i ].ascii() + "/Alpha", current_[ i ] );
        T_Layers::const_iterator it = std::find( currentLayers_.begin(), currentLayers_.end(), layers_[i] );
        if( it != currentLayers_.end() )
            options_.Change( std::string( "Layers/" ) + names_[ i ].ascii() + "/Position", static_cast< int >( it - currentLayers_.begin() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: LayersPanel::Reset
// Created: AGE 2007-01-04
// -----------------------------------------------------------------------------
void LayersPanel::Reset()
{
    fogOfWar_->Revert();
    //infra_->Revert();
    new_       = current_;
    newLayers_ = currentLayers_;
    for( unsigned i = 0; i < layers_.size(); ++i )
        layers_[ i ]->SetAlpha( new_[ i ] );
    layersList_->clear();
    for( T_Layers::const_iterator it = newLayers_.begin(); it != newLayers_.end(); ++it )
    {
        if( it != newLayers_.end() - 1 )
            (*it)->MoveBelow( **(it+1) );
        ValuedListItem* item = new ValuedListItem( layersList_ );
        item->SetValue( *it );
        T_Layers::const_iterator lit = std::find( layers_.begin(), layers_.end(), *it );
        if( lit != layers_.end() )
        {
            item->setText( 0, "  "+names_[ lit - layers_.begin() ]);
        }
    }
    UpdateLeastAndMostVisible();
    layersList_->setCurrentItem( layersList_-> firstChild() );
}

// -----------------------------------------------------------------------------
// Name: LayersPanel::Reset
// Created: RPD 2008-08-13
// -----------------------------------------------------------------------------
void LayersPanel::UpdateLeastAndMostVisible()
{
    if( layersList_->childCount() > 1 )
    {
        for( Q3ListViewItem* item = layersList_->firstChild(); item; item = item->nextSibling() )
            for( T_Names::const_iterator it = names_.begin(); it != names_.end(); ++it )
                if( item->text( 0 ).contains( *it ) )
                    item->setText( 0, "  " + *it );
        layersList_->firstChild()->setText( 0, layersList_->firstChild()->text( 0 ) + tr( " (foreground)" ) );
        layersList_->lastItem()->setText( 0, layersList_->lastItem()->text( 0 ) + tr( " (background)" ) );
    }
}

// -----------------------------------------------------------------------------
// Name: LayersPanel::OnValueChanged
// Created: AGE 2007-01-04
// -----------------------------------------------------------------------------
void LayersPanel::OnValueChanged()
{
    if( currentLayer_ != -1 && currentLayer_ < int( layers_.size() ) )
    {
        new_   [ currentLayer_ ] = transparency_->value() * 0.01f;
        layers_[ currentLayer_ ]->SetAlpha( new_[ currentLayer_ ] );
    }
}

// -----------------------------------------------------------------------------
// Name: LayersPanel::OnSelectionChanged
// Created: AGE 2007-04-27
// -----------------------------------------------------------------------------
void LayersPanel::OnSelectionChanged( Q3ListViewItem* i )
{
    currentLayer_ = -1;
    ValuedListItem* item = static_cast< ValuedListItem* >( i );
    T_Layers::const_iterator it = std::find( layers_.begin(), layers_.end(), item->GetValue< Layer_ABC >() );

    QString transparencyLabelText( tr( "Transparency " ) );
    if( it != layers_.end() )
    {
        currentLayer_ = static_cast< int >( it - layers_.begin() );
        transparency_->setValue( int( new_[ currentLayer_ ] * 100 ) );
        for( T_Names::const_iterator nIT = names_.begin(); nIT != names_.end(); ++nIT )
            if( i->text( 0 ).contains( *nIT ) )
            {
                transparencyLabelText += tr( "for %1:" ).arg( *nIT );
                break;
            }
    }
    transparencyLabel_->setText( transparencyLabelText );
}

// -----------------------------------------------------------------------------
// Name: LayersPanel::OnUp
// Created: AGE 2007-04-27
// -----------------------------------------------------------------------------
void LayersPanel::OnUp()
{
    if( currentLayer_ != -1 && currentLayer_ < int( layers_.size() ) )
    {
        Layer_ABC* layer = layers_[ currentLayer_ ];
        if( ValuedListItem* item = FindItem( layer, layersList_->firstChild() ) )
        {
            ValuedListItem* previous = static_cast< ValuedListItem* >( item->itemAbove() );
            if( previous )
            {
                layer->MoveAbove( *previous->GetValue< Layer_ABC >() );
                previous->moveItem( item );
                T_Layers::iterator it = std::find( newLayers_.begin(), newLayers_.end(), layer );
                if( it < newLayers_.end() - 1 )
                    std::swap( *it, *(it+1) );
            }
        }
    }
    UpdateLeastAndMostVisible();
}

// -----------------------------------------------------------------------------
// Name: LayersPanel::OnDown
// Created: AGE 2007-04-27
// -----------------------------------------------------------------------------
void LayersPanel::OnDown()
{
    if( currentLayer_ != -1 && currentLayer_ < int( layers_.size() ) )
    {
        Layer_ABC* layer = layers_[ currentLayer_ ];
        ValuedListItem* item = FindItem( layer, layersList_->firstChild() );
        if( item )
        {
            ValuedListItem* next = static_cast< ValuedListItem* >( item->nextSibling() );
            if( next )
            {
                layer->MoveBelow( *next->GetValue< Layer_ABC >() );
                item->moveItem( next );
                T_Layers::iterator it = std::find( newLayers_.begin(), newLayers_.end(), layer );
                if( it != newLayers_.begin() && it != newLayers_.end() )
                    std::swap( *it, *(it-1) );
            }
        }
    }
    UpdateLeastAndMostVisible();
}

// -----------------------------------------------------------------------------
// Name: LayersPanel::OptionChanged
// Created: SBO 2007-03-27
// -----------------------------------------------------------------------------
void LayersPanel::OptionChanged( const std::string& name, const kernel::OptionVariant& value )
{
    if( name == "FogOfWar" )
    {
        fogOfWar_->setChecked( value.To< bool >() );
        return;
    }
    if( name == "Infra" )
    {
        infra_->setChecked( value.To< bool >() );
        return;
    }
    QString option( name.c_str() );
    if( !option.startsWith( "Layers/" ) )
        return;
    option.remove( "Layers/" );
    for( unsigned i = 0; i < layers_.size(); ++i )
    {
        if( names_[i] + "/Alpha" == option )
        {
            const float alpha = value.To< float >();
            new_[ i ] = current_[ i ] = alpha;
            layers_[ i ]->SetAlpha( new_[ i ] );
            return;
        }
        if( names_[i] + "/Position" == option )
        {
            T_Layers::iterator oldPosition = std::find( currentLayers_.begin(), currentLayers_.end(), layers_[i] );
            const int newIndex = value.To< int >();
            if( oldPosition != currentLayers_.end() && newIndex < int( currentLayers_.size() ) )
            {
                T_Layers::iterator newPosition = currentLayers_.begin() + newIndex;
                std::swap( *oldPosition, *newPosition );
                Reset();
                return;
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LayersPanel::OnFogOfWarChanged
// Created: SBO 2008-04-15
// -----------------------------------------------------------------------------
void LayersPanel::OnFogOfWarChanged( bool value )
{
    options_.Change( "FogOfWar", value );
}

// -----------------------------------------------------------------------------
// Name: LayersPanel::OnInfraChanged
// Created: SLG 2011-01-24
// -----------------------------------------------------------------------------
void LayersPanel::OnInfraChanged( bool value )
{
    options_.Change( "Infra", value );
}
