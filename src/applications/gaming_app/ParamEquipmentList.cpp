// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamEquipmentList.h"
#include "moc_ParamEquipmentList.cpp"
#include "clients_kernel/EquipmentType.h"
#include "clients_gui/ValuedListItem.h"
#include "gaming/Action_ABC.h"
#include "gaming/ActionParameterMaintenancePriorities.h"
#include "game_asn/Asn.h"
#include "icons.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ParamEquipmentList constructor
// Created: SBO 2005-09-27
// -----------------------------------------------------------------------------
ParamEquipmentList::ParamEquipmentList( QObject* parent, const OrderParameter& parameter, const Resolver< EquipmentType >& resolver )
    : QObject( parent )
    , Param_ABC( parameter.GetName() )
    , parameter_( parameter )
    , resolver_( resolver )
    , list_( 0 )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: ParamEquipmentList destructor
// Created: SBO 2005-09-27
// -----------------------------------------------------------------------------
ParamEquipmentList::~ParamEquipmentList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamEquipmentList::BuildInterface
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
void ParamEquipmentList::BuildInterface( QWidget* parent )
{
    QHBox* hBox = new QHBox( parent );
    list_ = new QListView( hBox );
    list_->addColumn( "" );
    list_->header()->hide();
    list_->setSorting( -1, true );
    list_->setResizeMode( QListView::ResizeMode::LastColumn );
    list_->setAllColumnsShowFocus( true );

    QVBox* buttonBox = new QVBox( hBox );
    buttonBox->layout()->setAlignment( Qt::AlignVCenter );
    QPushButton* upBtn = new QPushButton( MAKE_ICON( arrow_up ), QString::null, buttonBox );
    upBtn->setFixedSize( 32, 32 );
    QPushButton* downBtn = new QPushButton( MAKE_ICON( arrow_down ), QString::null, buttonBox );
    downBtn->setFixedSize( 32, 32 );

    connect( upBtn, SIGNAL( clicked() ), SLOT( OnUp() ) );
    connect( downBtn, SIGNAL( clicked() ), SLOT( OnDown() ) );
    connect( list_, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), SLOT( OnContextMenu( QListViewItem*, const QPoint&, int ) ) );
}

// -----------------------------------------------------------------------------
// Name: ParamEquipmentList::CommitTo
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
void ParamEquipmentList::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.value.t = T_MissionParameter_value_logMaintenancePriorities;
    ASN1T_LogMaintenancePriorities*& list = asn.value.u.logMaintenancePriorities = new ASN1T_LogMaintenancePriorities();
    list->n = list_->childCount();
    asn.null_value = list->n ? 0 : 1;
    if( asn.null_value )
        return;
    list->elem = new ASN1T_EquipmentType[ list->n ];
    QListViewItemIterator it( list_ );
    for( unsigned int i = 0; i < list->n && it.current(); ++i, ++it )
        list->elem[i] = static_cast< const ValuedListItem* >( *it )->GetValue< EquipmentType >()->GetId();
}

// -----------------------------------------------------------------------------
// Name: ParamEquipmentList::Clean
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
void ParamEquipmentList::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.logMaintenancePriorities )
        delete[] asn.value.u.logMaintenancePriorities->elem;
    delete asn.value.u.logMaintenancePriorities;
}

// -----------------------------------------------------------------------------
// Name: ParamEquipmentList::CommitTo
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void ParamEquipmentList::CommitTo( Action_ABC& action ) const
{
    std::auto_ptr< ActionParameterMaintenancePriorities > param( new ActionParameterMaintenancePriorities( parameter_ ) );
    for( QListViewItemIterator it( list_ ); it.current(); ++it )
        param->AddPriority( *static_cast< const ValuedListItem* >( it.current() )->GetValue< EquipmentType >() );
    action.AddParameter( *param.release() );
}

// -----------------------------------------------------------------------------
// Name: ParamEquipmentList::OnUp
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void ParamEquipmentList::OnUp()
{
    QListViewItem* item = list_->currentItem();
    if( item && item->itemAbove() )
        item->itemAbove()->moveItem( item );
}

// -----------------------------------------------------------------------------
// Name: ParamEquipmentList::OnDown
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void ParamEquipmentList::OnDown()
{
    QListViewItem* item = list_->currentItem();
    if( item && item->itemBelow() )
        item->moveItem( item->itemBelow() );
}

// -----------------------------------------------------------------------------
// Name: ParamEquipmentList::OnAdd
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void ParamEquipmentList::OnAdd( int index )
{
    ValuedListItem* item = new ValuedListItem( list_, list_->lastItem() );
    const EquipmentType& type = resolver_.Get( index );
    item->SetNamed( type );
}

// -----------------------------------------------------------------------------
// Name: ParamEquipmentList::OnRemove
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void ParamEquipmentList::OnRemove()
{
    delete list_->currentItem();
}

// -----------------------------------------------------------------------------
// Name: ParamEquipmentList::OnContextMenu
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void ParamEquipmentList::OnContextMenu( QListViewItem* item, const QPoint& point, int )
{
    QPopupMenu* menu = new QPopupMenu( list_ );
    QPopupMenu* items = new QPopupMenu( menu );
    Iterator< const EquipmentType& > it( resolver_.CreateIterator() );
    while( it.HasMoreElements() )
    {
        const EquipmentType& type = it.NextElement();
        if( !FindItem( &type, list_->firstChild() ) )
        {
            int id = items->insertItem( type.GetName(), this, SLOT( OnAdd( int ) ) );
            items->setItemParameter( id, type.GetId() );
        }
    }
    if( items->count() )
        menu->insertItem( tr( "Add..." ), items );
    if( item )
        menu->insertItem( tr( "Remove" ), this, SLOT( OnRemove() ) );
    menu->popup( point );
}
