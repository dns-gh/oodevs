// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamHumanWoundList.h"
#include "moc_ParamHumanWoundList.cpp"
#include "clients_gui/ValuedListItem.h"
#include "clients_kernel/OrderParameter.h"
#include "gaming/tools.h"
#include "gaming/Action_ABC.h"
#include "gaming/ActionParameterMedicalPriorities.h"
#include "icons.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: ParamHumanWoundList constructor
// Created: SBO 2005-09-27
// -----------------------------------------------------------------------------
ParamHumanWoundList::ParamHumanWoundList( QObject* parent, const kernel::OrderParameter& parameter )
    : QObject( parent )
    , Param_ABC( parameter.GetName() )
    , parameter_( parameter )
    , list_( 0 )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: ParamHumanWoundList destructor
// Created: SBO 2005-09-27
// -----------------------------------------------------------------------------
ParamHumanWoundList::~ParamHumanWoundList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamHumanWoundList::BuildInterface
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
void ParamHumanWoundList::BuildInterface( QWidget* parent )
{
    QHBox* hBox = new QHBox( parent );
    list_ = new QListView( hBox );
    list_->addColumn( "", 0 );
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
// Name: ParamHumanWoundList::CommitTo
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void ParamHumanWoundList::CommitTo( ActionParameterContainer_ABC& action ) const
{
    std::auto_ptr< ActionParameterMedicalPriorities > param( new ActionParameterMedicalPriorities( parameter_ ) );
    for( QListViewItemIterator it( list_ ); it.current(); ++it )
        param->AddMedicalPriority( E_HumanWound( it.current()->text( 0 ).toUInt() ) );
    action.AddParameter( *param.release() );
}

// -----------------------------------------------------------------------------
// Name: ParamHumanWoundList::OnUp
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void ParamHumanWoundList::OnUp()
{
    QListViewItem* item = list_->currentItem();
    if( item && item->itemAbove() )
        item->itemAbove()->moveItem( item );
}

// -----------------------------------------------------------------------------
// Name: ParamHumanWoundList::OnDown
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void ParamHumanWoundList::OnDown()
{
    QListViewItem* item = list_->currentItem();
    if( item && item->itemBelow() )
        item->moveItem( item->itemBelow() );
}

// -----------------------------------------------------------------------------
// Name: ParamHumanWoundList::OnAdd
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void ParamHumanWoundList::OnAdd( int value )
{
    QListViewItem* item = new QListViewItem( list_, list_->lastItem() );
    item->setText( 1, tools::ToString( E_HumanWound( value ) ) );
    item->setText( 0, QString::number( value ) );
}

// -----------------------------------------------------------------------------
// Name: ParamHumanWoundList::OnRemove
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void ParamHumanWoundList::OnRemove()
{
    delete list_->currentItem();
}

// -----------------------------------------------------------------------------
// Name: ParamHumanWoundList::OnContextMenu
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void ParamHumanWoundList::OnContextMenu( QListViewItem* item, const QPoint& point, int )
{
    QPopupMenu* menu = new QPopupMenu( list_ );
    QPopupMenu* items = new QPopupMenu( menu );
    for( unsigned int i = eHumanWound_BlesseUrgence1; i < int( eNbrHumanWound ); ++i )
    {
        const QString name = tools::ToString( ( E_HumanWound )i );
        if( ! list_->findItem( name, 1 ) )
        {
            int id = items->insertItem( name, this, SLOT( OnAdd( int ) ) );
            items->setItemParameter( id, i );
        }
    }
    if( items->count() )
        menu->insertItem( tr( "Add..." ), items );
    if( item )
        menu->insertItem( tr( "Remove" ), this, SLOT( OnRemove() ) );
    menu->popup( point );
}
