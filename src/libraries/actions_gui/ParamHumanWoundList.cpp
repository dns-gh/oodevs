// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamHumanWoundList.h"
#include "moc_ParamHumanWoundList.cpp"
#include "actions/Action_ABC.h"
#include "actions/MedicalPriorities.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_gui/resources.h"
#include "clients_gui/Tools.h"
#include "clients_gui/ValuedListItem.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamHumanWoundList constructor
// Created: SBO 2005-09-27
// -----------------------------------------------------------------------------
ParamHumanWoundList::ParamHumanWoundList( QObject* parent, const kernel::OrderParameter& parameter )
    : QObject( parent )
    , Param_ABC( parameter.GetName().c_str(), parameter.IsOptional() )
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
QWidget* ParamHumanWoundList::BuildInterface( QWidget* parent )
{
    Param_ABC::BuildInterface( parent );
    QGridLayout* layout = new QGridLayout( group_ );

    list_ = new Q3ListView( parent );
    list_->addColumn( "", 0 );
    list_->addColumn( "" );
    list_->header()->hide();
    list_->setSorting( -1, true );
    list_->setResizeMode( Q3ListView::LastColumn );
    list_->setAllColumnsShowFocus( true );

    QPushButton* upBtn = new QPushButton( MAKE_ICON( arrow_up ), QString::null, parent );
    upBtn->setFixedSize( 32, 32 );
    QPushButton* downBtn = new QPushButton( MAKE_ICON( arrow_down ), QString::null, parent );
    downBtn->setFixedSize( 32, 32 );

    layout->addWidget( list_, 0, 0, 2, 1 );
    layout->addWidget( upBtn, 0, 1 );
    layout->addWidget( downBtn, 1, 1 );

    connect( upBtn, SIGNAL( clicked() ), SLOT( OnUp() ) );
    connect( downBtn, SIGNAL( clicked() ), SLOT( OnDown() ) );
    connect( list_, SIGNAL( contextMenuRequested( Q3ListViewItem*, const QPoint&, int ) ), SLOT( OnContextMenu( Q3ListViewItem*, const QPoint&, int ) ) );
    return group_;
}

// -----------------------------------------------------------------------------
// Name: ParamHumanWoundList::CommitTo
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void ParamHumanWoundList::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    std::auto_ptr< actions::parameters::MedicalPriorities > param( new actions::parameters::MedicalPriorities( parameter_ ) );
    if( IsChecked() )
        for( Q3ListViewItemIterator it( list_ ); it.current(); ++it )
            param->AddMedicalPriority( E_HumanWound( it.current()->text( 0 ).toUInt() ) );
    action.AddParameter( *param.release() );
}

// -----------------------------------------------------------------------------
// Name: ParamHumanWoundList::OnUp
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void ParamHumanWoundList::OnUp()
{
    Q3ListViewItem* item = list_->currentItem();
    if( item && item->itemAbove() )
        item->itemAbove()->moveItem( item );
}

// -----------------------------------------------------------------------------
// Name: ParamHumanWoundList::OnDown
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void ParamHumanWoundList::OnDown()
{
    Q3ListViewItem* item = list_->currentItem();
    if( item && item->itemBelow() )
        item->moveItem( item->itemBelow() );
}

// -----------------------------------------------------------------------------
// Name: ParamHumanWoundList::OnAdd
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void ParamHumanWoundList::OnAdd( int value )
{
    Q3ListViewItem* item = new Q3ListViewItem( list_, list_->lastItem() );
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
void ParamHumanWoundList::OnContextMenu( Q3ListViewItem* item, const QPoint& point, int )
{
    Q3PopupMenu* menu = new Q3PopupMenu( list_ );
    Q3PopupMenu* items = new Q3PopupMenu( menu );
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

// -----------------------------------------------------------------------------
// Name: ParamHumanWoundList::InternalCheckValidity
// Created: LDC 2009-11-12
// -----------------------------------------------------------------------------
bool ParamHumanWoundList::InternalCheckValidity() const
{
    return list_ && list_->childCount() != 0;
}
