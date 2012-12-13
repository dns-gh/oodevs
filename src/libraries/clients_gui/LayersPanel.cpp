// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "LayersPanel.h"
#include "moc_LayersPanel.cpp"
#include "CheckBox.h"
#include "GlSelector.h"
#include "Layer_ABC.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/OptionVariant.h"
#include "resources.h"

using namespace gui;

Q_DECLARE_METATYPE( Layer_ABC* )

namespace
{
    QWidget* AddButton( const QIcon& icon, const QString& tooltip, const QObject* receiver, const char* member )
    {
        QPushButton* button = new QPushButton( icon, QString::null );
        button->setFixedSize( 32, 32 );
        QToolTip::add( button, tooltip );
        button->connect( button, SIGNAL( clicked() ), receiver, member );
        return button;
    }
}

// -----------------------------------------------------------------------------
// Name: LayersPanel constructor
// Created: AGE 2007-01-04
// -----------------------------------------------------------------------------
LayersPanel::LayersPanel( QWidget* parent, kernel::Controllers& controllers, GlSelector& selector )
    : PreferencePanel_ABC( parent, "LayersPanel" )
    , controllers_       ( controllers )
    , options_           ( controllers.options_ )
    , currentLayer_      ( -1 )
    , selector_          ( selector )
    , layersModel_       ( new QStandardItemModel() )
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
        // ListView
        Q3GroupBox* groupBox = new Q3GroupBox( 1, Qt::Horizontal, tr( "Layer display order and transparency" ), container );
        Q3VBox* vBox = new Q3VBox( groupBox );
        vBox->setSpacing( 6 );
        Q3HBox* box = new Q3HBox( vBox );
        box->setSpacing( 5 );
        layersList_ = new QTreeView( box );
        layersList_->setRootIsDecorated( false );
        layersList_->header()->hide();
        layersList_->setModel( layersModel_ );
        connect( layersList_->selectionModel(), SIGNAL( currentChanged( const QModelIndex&, const QModelIndex& ) ), SLOT( OnSelectionChanged() ) );

        // Buttons
        QWidget* up   = AddButton( MAKE_PIXMAP( arrow_up ),   tr( "Move the selected layer forwards" ),  this,   SLOT( OnUp() ) );
        QWidget* down = AddButton( MAKE_PIXMAP( arrow_down ), tr( "Move the selected layer backwards" ), this,   SLOT( OnDown() ) );
        QWidget* add  = AddButton( MAKE_PIXMAP( add_point ),  tr( "Add a user raster layer" ),           parent, SIGNAL( OnAddRaster() ) );
        removeButton_ = AddButton( MAKE_PIXMAP( trash ),      tr( "Remove a user raster layer" ),        this,   SLOT( OnRemoveDynamicLayer() ) );
        QWidget* buttonBox = new QWidget( box );
        QVBoxLayout* layout = new QVBoxLayout( buttonBox );
        layout->setMargin( 10 );
        layout->addWidget( up );
        layout->addWidget( down );
        layout->addStretch();
        layout->addWidget( add );
        layout->addWidget( removeButton_ );

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

namespace
{
    QStandardItem* CreateItem( const QString& name, gui::Layer_ABC& layer, QStandardItemModel& model )
    {
        QStandardItem* item = new QStandardItem( "  " + name );
        item->setEditable( false );
        item->setData( QVariant::fromValue( &layer ) );
        model.insertRow( 0, item );
        return item;
    }
}

// -----------------------------------------------------------------------------
// Name: LayersPanel::AddLayer
// Created: AGE 2007-01-04
// -----------------------------------------------------------------------------
void LayersPanel::AddLayer( const QString& name, Layer_ABC& layer, bool dynamic /* = false */ )
{
    CreateItem( "  " + name, layer, *layersModel_ );

    if( dynamic )
        dynamicLayers_.push_back( &layer );
    layers_       .push_back( &layer );
    currentLayers_.push_back( &layer );
    newLayers_    .push_back( &layer );
    current_.push_back( 1 );
    new_    .push_back( 1 );
    names_  .push_back( name );
}

// -----------------------------------------------------------------------------
// Name: LayersPanel::OnRemoveDynamicLayer
// Created: ABR 2012-06-11
// -----------------------------------------------------------------------------
void LayersPanel::OnRemoveDynamicLayer()
{
    QStandardItem* item = layersModel_->itemFromIndex( layersList_->selectionModel()->currentIndex() );
    if( !item )
        return;
    RemoveDynamicLayer( *item );
}

namespace
{
    template< typename T >
    void RemoveFromVector( Layer_ABC* layer, std::vector< Layer_ABC* >& layerVector, std::vector< T >& otherVector )
    {
        for( unsigned i = 0; i < layerVector.size(); ++i )
            if( layerVector[ i ] == layer )
            {
                layerVector.erase( layerVector.begin() + i );
                otherVector.erase( otherVector.begin() + i );
                break;
            }
    }
}

// -----------------------------------------------------------------------------
// Name: LayersPanel::RemoveDynamicLayer
// Created: ABR 2012-06-12
// -----------------------------------------------------------------------------
void LayersPanel::RemoveDynamicLayer( QStandardItem& item )
{
    Layer_ABC* layer = item.data().value< Layer_ABC* >();
    if( !layer )
        return;

    T_Layers::iterator itDynamic = std::find( dynamicLayers_.begin(), dynamicLayers_.end(), layer );
    if( itDynamic == dynamicLayers_.end() )
        return;

    dynamicLayers_.erase( itDynamic );

    RemoveFromVector( layer, currentLayers_, current_ );
    RemoveFromVector( layer, newLayers_, new_ );
    RemoveFromVector( layer, layers_, names_ );

    layersModel_->removeRow( item.row() );
    selector_.RemoveLayer( *layer );
    UpdateLeastAndMostVisible();
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
        if( std::find( dynamicLayers_.begin(), dynamicLayers_.end(), layers_[ i ] ) != dynamicLayers_.end() )
            continue;
        options_.Change( std::string( "Layers/" ) + names_[ i ].toStdString() + "/Alpha", current_[ i ] );
        T_Layers::const_iterator it = std::find( currentLayers_.begin(), currentLayers_.end(), layers_[i] );
        if( it != currentLayers_.end() )
            options_.Change( std::string( "Layers/" ) + names_[ i ].toStdString() + "/Position", static_cast< int >( it - currentLayers_.begin() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: LayersPanel::Reset
// Created: AGE 2007-01-04
// -----------------------------------------------------------------------------
void LayersPanel::Reset()
{
    fogOfWar_->Revert();
    infra_->Revert();
    ResetLayers();
    QStandardItem* root = layersModel_->invisibleRootItem();
    for( int row = 0; row < root->rowCount(); ++row )
        if( QStandardItem* childItem = root->child( row ) )
            RemoveDynamicLayer( *childItem );
}

// -----------------------------------------------------------------------------
// Name: LayersPanel::ResetLayers
// Created: LGY 2012-04-04
// -----------------------------------------------------------------------------
void LayersPanel::ResetLayers()
{
    new_       = current_;
    newLayers_ = currentLayers_;
    for( unsigned i = 0; i < layers_.size(); ++i )
        layers_[ i ]->SetAlpha( new_[ i ] );
    layersModel_->clear();
    for( T_Layers::iterator it = newLayers_.begin(); it != newLayers_.end(); ++it )
    {
        if( it != newLayers_.end() - 1 )
            (*it)->MoveBelow( **(it+1) );

        QStandardItem* item = CreateItem( "", **it, *layersModel_ );

        T_Layers::const_iterator lit = std::find( layers_.begin(), layers_.end(), *it );
        if( lit != layers_.end() )
            item->setText( "  " + names_[ lit - layers_.begin() ]);
    }
    UpdateLeastAndMostVisible();
    if( QStandardItem* child = layersModel_->invisibleRootItem()->child( 0 ) )
    {
        QItemSelectionModel* selectionModel = layersList_->selectionModel();
        selectionModel->clear();
        selectionModel->select( child->index(), QItemSelectionModel::Select );
    }
}

// -----------------------------------------------------------------------------
// Name: LayersPanel::UpdateLeastAndMostVisible
// Created: RPD 2008-08-13
// -----------------------------------------------------------------------------
void LayersPanel::UpdateLeastAndMostVisible()
{
    QStandardItem* root = layersModel_->invisibleRootItem();
    int count = root->rowCount();
    if( count > 1 )
    {
        for( int row = 0; row < count; ++row )
            for( T_Names::const_iterator it = names_.begin(); it != names_.end(); ++it )
                 if( QStandardItem* childItem = root->child( row ) )
                     if( childItem->text().contains( *it ) )
                         childItem->setText( "  " + *it );
        root->child( 0 )->setText( root->child( 0 )->text() + tr( " (foreground)" ) );
        root->child( count - 1 )->setText( root->child( count - 1 )->text() + tr( " (background)" ) );
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
void LayersPanel::OnSelectionChanged()
{
    QModelIndex index = layersList_->selectionModel()->currentIndex();
    if( QStandardItem* item = layersModel_->itemFromIndex( index ) )
    {
        currentLayer_ = -1;
        T_Layers::const_iterator it = std::find( layers_.begin(), layers_.end(), item->data().value< Layer_ABC* >() );
        QString transparencyLabelText( tr( "Transparency " ) );

        if( it != layers_.end() )
        {
            currentLayer_ = static_cast< int >( it - layers_.begin() );
            transparency_->setValue( int( new_[ currentLayer_ ] * 100 ) ); 
            for( T_Names::const_iterator nIT = names_.begin(); nIT != names_.end(); ++nIT )
                if( item->text().contains( *nIT ) )
                {
                    transparencyLabelText += tr( "for %1:" ).arg( *nIT );
                    break;
                }
            removeButton_->setEnabled( std::find( dynamicLayers_.begin(), dynamicLayers_.end(), *it ) != dynamicLayers_.end() );
        }
        transparencyLabel_->setText( transparencyLabelText );
    }
}

// -----------------------------------------------------------------------------
// Name: LayersPanel::MoveItem
// Created: LGY 2012-10-01
// -----------------------------------------------------------------------------
void LayersPanel::MoveItem( int row, Layer_ABC* layer, int newPlace, int oldPlace, int step )
{
    if( layer )
    {
        QStandardItem* item = layersModel_->takeItem( row );
        layersModel_->insertRow( row + newPlace, item );
        layersModel_->removeRow( row + oldPlace );
        T_Layers::iterator it = std::find( newLayers_.begin(), newLayers_.end(), layer );
        std::swap( *it, *(it+step) );
        layersList_->selectionModel()->select( item->index(), QItemSelectionModel::Select );
        UpdateLeastAndMostVisible();
    }
}

// -----------------------------------------------------------------------------
// Name: LayersPanel::GetCurrentLayer
// Created: LGY 2012-10-01
// -----------------------------------------------------------------------------
Layer_ABC* LayersPanel::GetCurrentLayer() const
{
    if( currentLayer_ != -1 && currentLayer_ < static_cast< int >( layers_.size() ) )
        return layers_[ currentLayer_ ];
    return 0;
}

// -----------------------------------------------------------------------------
// Name: LayersPanel::GetCurrentRow
// Created: LGY 2012-10-01
// -----------------------------------------------------------------------------
int LayersPanel::GetCurrentRow( Layer_ABC* layer ) const
{
    QModelIndexList list = layersModel_->match( layersModel_->index( 0, 0 ), Qt::UserRole + 1, QVariant::fromValue( layer ), -1, Qt::MatchRecursive );
    if( !list.empty() )
        return list.front().row();
    return -1;
}

// -----------------------------------------------------------------------------
// Name: LayersPanel::OnUp
// Created: AGE 2007-04-27
// -----------------------------------------------------------------------------
void LayersPanel::OnUp()
{
    Layer_ABC* layer = GetCurrentLayer();
    if( layer )
    {
        int row = GetCurrentRow( layer );
        if( row != -1 && row != 0 )
        {
            if( QStandardItem* previous = layersModel_->item( row - 1 ) )
                layer->MoveAbove( *previous->data().value< Layer_ABC* >() );
            MoveItem( row, layer, -1, 1, 1 );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LayersPanel::OnDown
// Created: AGE 2007-04-27
// -----------------------------------------------------------------------------
void LayersPanel::OnDown()
{
    Layer_ABC* layer = GetCurrentLayer();
    if( layer )
    {
        int row = GetCurrentRow( layer );
        if( row != -1 && row != layersModel_->rowCount() - 1 )
        {
            if( QStandardItem* next = layersModel_->item( row + 1 ) )
                layer->MoveBelow( *next->data().value< Layer_ABC* >() );
            MoveItem( row, layer, 2, 0, -1 );
        }
    }
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
                ResetLayers();
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

// -----------------------------------------------------------------------------
// Name: LayersPanel::Update
// Created: ABR 2012-05-14
// -----------------------------------------------------------------------------
void LayersPanel::Update()
{
    QStandardItem* root = layersModel_->invisibleRootItem();
    for( int row = 0; row < root->rowCount(); ++row )
        if( QStandardItem* childItem = root->child( row ) )
            if( Layer_ABC* layer = childItem->data().value< Layer_ABC* >() )
                childItem->setEnabled( layer->IsEnabled() );
}

// -----------------------------------------------------------------------------
// Name: LayersPanel::showEvent
// Created: LGY 2012-09-27
// -----------------------------------------------------------------------------
void LayersPanel::showEvent( QShowEvent* /*event*/ )
{
    currentLayer_ = -1;
}
