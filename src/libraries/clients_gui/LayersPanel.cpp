// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************


#include "clients_gui_pch.h"
#include "LayersPanel.h"
#include "moc_LayersPanel.cpp"
#include "CheckBox.h"
#include "GlProxy.h"
#include "Layer_ABC.h"
#include "SignalAdapter.h"
#include "RichGroupBox.h"
#include "RichPushButton.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/OptionsController.h"
#include "clients_kernel/OptionVariant.h"
#include "resources.h"
#include "ENT/ENT_Tr.h"

using namespace gui;

Q_DECLARE_METATYPE( T_Layer )

namespace
{
    enum LayersRole
    {
        eData = Qt::UserRole,
        ePosition,
        eAlpha,
        eDynamic,
        eOptionName,
        ePositionBackup,
        eAlphaBackup
    };
    QStandardItem* GetItemFromProxy( const QStandardItemModel& model, const QSortFilterProxyModel& proxy, int row )
    {
        return model.itemFromIndex( proxy.mapToSource( proxy.index( row, 0 ) ) );
    }
    std::string GetLayerOptionText( const QStandardItem& item, const std::string& option )
    {
        return "Layers/" + item.data( eOptionName ).toString().toStdString() + "/" + option;
    }
    void SetItemPosition( kernel::OptionsController& options, QStandardItem& item, int position )
    {
        item.setData( position, ePosition );
        options.Change( GetLayerOptionText( item, "Position" ), position );
    }
    QCheckBox* AddCheckbox( QVBoxLayout* layout,
                            const QString& objectName,
                            const QString& boxName,
                            const QString& title,
                            const QString& boxTitle )
    {
        QCheckBox* checkbox = new CheckBox( objectName, title );
        RichGroupBox* box = new RichGroupBox( boxName, boxTitle );
        QHBoxLayout* boxlayout = new QHBoxLayout( box );
        boxlayout->addWidget( checkbox );
        layout->addWidget( box, 1 );
        return checkbox;
    }
    QPushButton* AddButton( const QString& objectName, const QIcon& icon, const QString& tooltip, const QObject* receiver, const char* member )
    {
        RichPushButton* button = new RichPushButton( objectName, icon, QString::null );
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
LayersPanel::LayersPanel( QWidget* parent,
                          kernel::Controllers& controllers,
                          GlProxy& proxy )
    : PreferencePanel_ABC( parent, "LayersPanel" )
    , controllers_( controllers )
    , options_( controllers.options_ )
    , proxy_( proxy )
    , dataModel_( this )
    , proxyModel_( this )
{
    QVBoxLayout* container = new QVBoxLayout( this );
    container->setSizeConstraint( QLayout::SetMinimumSize );

    // checkboxes
    fogOfWar_ = AddCheckbox( container, "fogOfWar", "fogBox", tr( "Display fog of war" ), tr( "Fog of war" ) );
    infra_ = AddCheckbox( container, "infra", "infraBox", tr( "Display Infrastructures" ), tr( "Infrastructures" ) );
    gui::connect( fogOfWar_, SIGNAL( toggled( bool ) ), [&]{
        options_.Change( "FogOfWar", fogOfWar_->isChecked() );
    } );
    gui::connect( infra_, SIGNAL( toggled( bool ) ), [&]{
        options_.Change( "Infra", infra_->isChecked() );
    } );

    // listview
    layersListView_ = new QListView();
    layersListView_->setObjectName( "layers-list" );
    proxyModel_.setSourceModel( &dataModel_ );
    proxyModel_.setSortRole( ePosition );
    proxyModel_.setDynamicSortFilter( true );
    proxyModel_.sort( 0, Qt::DescendingOrder );
    layersListView_->setModel( &proxyModel_ );
    connect( layersListView_->selectionModel(),
             SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection& ) ),
             SLOT( OnSelectionChanged( const QItemSelection&, const QItemSelection& ) ) );

    // buttons
    upButton_ = AddButton( "up", MAKE_PIXMAP( arrow_up ),   tr( "Move the selected layer forwards" ), this, SLOT( OnUp() ) );
    downButton_ = AddButton( "down", MAKE_PIXMAP( arrow_down ), tr( "Move the selected layer backwards" ), this, SLOT( OnDown() ) );
    QWidget* add  = AddButton( "add", MAKE_PIXMAP( add_point ),  tr( "Add a user raster layer" ), parent, SIGNAL( OnAddRaster() ) );
    removeButton_ = AddButton( "remove", MAKE_PIXMAP( trash ),      tr( "Remove a user raster layer" ), this, SLOT( OnRemoveDynamicLayer() ) );
    QWidget* buttonBox = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout( buttonBox );
    layout->setMargin( 10 );
    layout->addWidget( upButton_ );
    layout->addWidget( downButton_ );
    layout->addStretch();
    layout->addWidget( add );
    layout->addWidget( removeButton_ );

    // transparency slider
    transparencyLabel_ = new QLabel( tr( "Transparency " ) );
    transparency_ = new QSlider( 0, 100, 1, 100, Qt::Horizontal );
    transparency_->setObjectName( "transparency" );
    transparency_->setMaximumWidth( layersListView_->width() );
    connect( transparency_, SIGNAL( valueChanged( int ) ), SLOT( OnAlphaChanged() ) );

    // Layout
    RichGroupBox* groupBox = new RichGroupBox( "LayerDisplay", tr( "Layer display order and transparency" ) );
    QGridLayout* groupBoxLayout = new QGridLayout( groupBox );
    groupBoxLayout->setSpacing( 6 );
    groupBoxLayout->addWidget( layersListView_, 0, 0 );
    groupBoxLayout->addWidget( buttonBox, 0, 1 );
    groupBoxLayout->addWidget( transparencyLabel_, 1, 0 );
    groupBoxLayout->addWidget( transparency_, 2, 0 );
    container->addWidget( groupBox, 100 );

    setLayout( container );
}

// -----------------------------------------------------------------------------
// Name: LayersPanel destructor
// Created: AGE 2007-01-04
// -----------------------------------------------------------------------------
LayersPanel::~LayersPanel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LayersPanel::Load
// Created: ABR 2014-08-04
// -----------------------------------------------------------------------------
void LayersPanel::Load( const GlProxy& proxy )
{
    // purge
    dataModel_.clear();
    // load
    fogOfWar_->setChecked( options_.GetOption( "FogOfWar" ).To< bool >() );
    infra_->setChecked( options_.GetOption( "Infra" ).To< bool >() );
    proxy.ApplyToLayers( [&]( const T_Layer& layer ) {
        if( !layer->IsConfigurable() )
            return;
        const auto optionName = layer->GetOptionName();
        const int position = options_.GetOption( "Layers/" + optionName + "/Position" ).To< int >();
        const float alpha = options_.GetOption( "Layers/" + optionName + "/Alpha" ).To< float >();
        auto* item = new QStandardItem( layer->GetName() );
        item->setFlags( layer->IsEnabled() ? Qt::ItemIsSelectable | Qt::ItemIsEnabled : Qt::ItemIsSelectable );
        item->setData( QVariant::fromValue( layer ), eData );
        item->setData( position, ePosition );
        item->setData( alpha, eAlpha );
        item->setData( layer->GetType() == eLayerTypes_RasterDynamic, eDynamic );
        item->setData( QString::fromStdString( optionName ), eOptionName );
        item->setData( position, ePositionBackup );
        item->setData( alpha, eAlphaBackup );
        dataModel_.appendRow( QList< QStandardItem* >() << item );
    } );
    UpdateLeastAndMostVisible();
    layersListView_->selectionModel()->select( proxyModel_.index( 0, 0 ), QItemSelectionModel::SelectCurrent );
}

// -----------------------------------------------------------------------------
// Name: LayersPanel::Reset
// Created: ABR 2014-09-30
// -----------------------------------------------------------------------------
void LayersPanel::Reset()
{
    for( int row = 0; row < dataModel_.rowCount(); ++row )
        if( auto item = dataModel_.item( row ) )
        {
            options_.Change( GetLayerOptionText( *item, "Alpha" ), item->data( eAlphaBackup ).toFloat() ); 
            options_.Change( GetLayerOptionText( *item, "Position" ), item->data( ePositionBackup ).toInt() ); 
        }
    proxy_.UpdateLayerOrder( *options_.GetViewOptions() );
    dataModel_.clear();
}

// -----------------------------------------------------------------------------
// Name: LayersPanel::OnAlphaChanged
// Created: AGE 2007-01-04
// -----------------------------------------------------------------------------
void LayersPanel::OnAlphaChanged()
{
    QStandardItem* item = GetSelectedItem();
    if( !item )
        return;
    const float alpha = transparency_->value() * 0.01f;
    item->setData( alpha, eAlpha );
    options_.Change( GetLayerOptionText( *item, "Alpha" ), alpha ); 
    item->data( eData ).value< T_Layer >()->SetAlpha( alpha );
}

// -----------------------------------------------------------------------------
// Name: LayersPanel::OnSelectionChanged
// Created: AGE 2007-04-27
// -----------------------------------------------------------------------------
void LayersPanel::OnSelectionChanged( const QItemSelection&, const QItemSelection& )
{
    UpdateButtonsStates();
    auto item = GetSelectedItem();
    if( item )
        transparency_->setValue( static_cast< int >( item->data( eAlpha ).toFloat() * 100 ) );
    auto layer = item->data( eData ).value< T_Layer >();
    transparencyLabel_->setText( tr( "Transparency " ) + tr( "for %1:" ).arg( layer->GetName() ) );
}

// -----------------------------------------------------------------------------
// Name: LayersPanel::OnUp
// Created: AGE 2007-04-27
// -----------------------------------------------------------------------------
void LayersPanel::OnUp()
{
    SwapSelection( +1 );
}

// -----------------------------------------------------------------------------
// Name: LayersPanel::OnDown
// Created: AGE 2007-04-27
// -----------------------------------------------------------------------------
void LayersPanel::OnDown()
{
    SwapSelection( -1 );
}

// -----------------------------------------------------------------------------
// Name: LayersPanel::OnRemoveDynamicLayer
// Created: ABR 2012-06-11
// -----------------------------------------------------------------------------
void LayersPanel::OnRemoveDynamicLayer()
{
    QStandardItem* item = GetSelectedItem();
    if( !item )
        return;
    auto layer = item->data( eData ).value< T_Layer >();
    proxy_.RemoveLayer( layer );
    dataModel_.removeRow( item->index().row() );
}

// -----------------------------------------------------------------------------
// Name: LayersPanel::MoveSelection
// Created: ABR 2014-08-04
// -----------------------------------------------------------------------------
void LayersPanel::SwapSelection( int direction )
{
    QStandardItem* item = GetSelectedItem();
    if( !item || direction == 0 )
        return;
    int position = item->data( ePosition ).toInt();
    const auto targetIndexes = dataModel_.match( dataModel_.index( 0, 0 ), ePosition, position + direction, -1, Qt::MatchExactly );
    if( !targetIndexes.size() )
        return;
    QStandardItem* targetItem = dataModel_.itemFromIndex( targetIndexes[ 0 ] );
    if( !targetItem )
        return;
    SetItemPosition( options_, *item, position + direction );
    SetItemPosition( options_, *targetItem, position );
    if( direction > 0 )
        proxy_.MoveBelow( targetItem->data( eData ).value< T_Layer >(),
                          item->data( eData ).value< T_Layer >() );
    else
        proxy_.MoveBelow( item->data( eData ).value< T_Layer >(),
                          targetItem->data( eData ).value< T_Layer >() );
    UpdateLeastAndMostVisible();
}

// -----------------------------------------------------------------------------
// Name: LayersPanel::UpdateLeastAndMostVisible
// Created: RPD 2008-08-13
// -----------------------------------------------------------------------------
void LayersPanel::UpdateLeastAndMostVisible()
{
    if( dataModel_.rowCount() <= 1 )
        return;
    const auto foreground = tr( " (foreground)" );
    const auto background = tr( " (background)" );
    for( int i = 0; i < dataModel_.rowCount(); ++i )
    {
        auto item = GetItemFromProxy( dataModel_, proxyModel_, i );
        auto text = item->text().remove( foreground ).remove( background );
        if( item == GetItemFromProxy( dataModel_, proxyModel_, 0 ) )
            text += foreground;
        if( item == GetItemFromProxy( dataModel_, proxyModel_, dataModel_.rowCount() - 1 ) )
            text += background;
        item->setText( text );
    }
    UpdateButtonsStates();
}

// -----------------------------------------------------------------------------
// Name: LayersPanel::UpdateUpAndDownButtons
// Created: ABR 2014-08-04
// -----------------------------------------------------------------------------
void LayersPanel::UpdateButtonsStates()
{
    auto item = GetSelectedItem();
    upButton_->setEnabled( item && proxyModel_.mapFromSource( item->index() ).row() != 0 );
    downButton_->setEnabled( item && proxyModel_.mapFromSource( item->index() ).row() != dataModel_.rowCount() - 1 );
    removeButton_->setEnabled( item && item->data( eDynamic ).toBool() );
    transparency_->setEnabled( item != 0 );
}

// -----------------------------------------------------------------------------
// Name: LayersPanel::GetSelection
// Created: ABR 2014-08-04
// -----------------------------------------------------------------------------
QStandardItem* LayersPanel::GetSelectedItem() const
{
    const auto selectedIndexes = layersListView_->selectionModel()->selectedIndexes();
    if( !selectedIndexes.size() )
        return 0;
    auto item = dataModel_.itemFromIndex( proxyModel_.mapToSource( selectedIndexes[ 0 ] ) );
    return item && item->isEnabled() ? item : 0;
}
