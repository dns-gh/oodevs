// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "GroupDialog.h"
#include "moc_GroupDialog.cpp"
#include "preparation/InitialState.h"
#include "preparation/InitialStateEquipment.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/tools.h"
#include "clients_gui/RichSpinBox.h"

using namespace gui;

class GroupsDelegate : public gui::CommonDelegate
{
public:
    explicit GroupsDelegate( QObject* parent, const QSortFilterProxyModel& proxyModel, const QStandardItemModel& dataModel )
        : gui::CommonDelegate( parent )
        , proxyModel_( proxyModel )
        , dataModel_ ( dataModel )
    {}
    virtual QWidget* createEditor( QWidget* parent, const QStyleOptionViewItem& /*option*/, const QModelIndex& i ) const
    {
        const QModelIndex index = proxyModel_.mapToSource( i );
        const int row = index.row();
        int max = dataModel_.item( row, 1 )->text().toInt();
        for( int c = 2; c < dataModel_.columnCount(); ++c )
            if( c != index.column() )
                max -= dataModel_.item( row, c )->text().toInt();
        return new gui::RichSpinBox( parent, 0, max );
    }
    const QSortFilterProxyModel& proxyModel_;
    const QStandardItemModel& dataModel_;
};

// -----------------------------------------------------------------------------
// Name: GroupDialog constructor
// Created: NPT 2013-04-11
// -----------------------------------------------------------------------------
GroupDialog::GroupDialog( QWidget* parent, kernel::Controllers& controllers )
    : QDialog( parent )
    , controllers_( controllers )
    , selected_( controllers )
    , dataModel_( parent )
    , proxyModel_( parent )
    , delegate_ ( new GroupsDelegate( parent, proxyModel_, dataModel_ ) )
{
    setWindowTitle( tr( "Groups" ) );
    resize( 900, 600 );

    proxyModel_.setSourceModel( &dataModel_ );
    table_.setModel( &proxyModel_ );
    table_.setItemDelegate( delegate_.get() );
    table_.setSortingEnabled( true );
    table_.setShowGrid( true );
    table_.setAlternatingRowColors( true );
    table_.verticalHeader()->setVisible( false );
    table_.setSelectionMode( QAbstractItemView::SingleSelection );
    table_.setSelectionBehavior( QAbstractItemView::SelectItems );
    table_.setEditTriggers( QAbstractItemView::AllEditTriggers );
    table_.setContextMenuPolicy( Qt::CustomContextMenu );
    connect( &table_, SIGNAL( customContextMenuRequested( const QPoint& ) ), SLOT( OnContextMenuRequested( const QPoint& ) ) );

    QPushButton* okButton = new QPushButton( tr( "Ok" ) );
    connect( okButton, SIGNAL( clicked() ), SLOT( OnValidate() ) );

    QPushButton* cancelButton = new QPushButton( tr( "Cancel" ) );
    connect( cancelButton, SIGNAL( clicked() ), SLOT( reject() ) );

    QGridLayout* mainLayout = new QGridLayout( this );
    mainLayout->addWidget( &table_, 0, 0, 9, 7 );
    mainLayout->addWidget( okButton, 10, 5 );
    mainLayout->addWidget( cancelButton, 10, 6 );

    controllers_.Register( *this );
    setVisible( false );
}

// -----------------------------------------------------------------------------
// Name: GroupDialog destructor
// Created: NPT 2013-04-11
// -----------------------------------------------------------------------------
GroupDialog::~GroupDialog()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: GroupDialog::NotifyContextMenu
// Created: NPT 2013-04-11
// -----------------------------------------------------------------------------
void GroupDialog::NotifyContextMenu( const kernel::Agent_ABC& entity, kernel::ContextMenu& menu )
{
    selected_ = const_cast< kernel::Agent_ABC* >( &entity );
    menu.InsertItem( "Helpers", tr( "Edit groups" ), this, SLOT( OnShow() ), false, 1 );
}

// -----------------------------------------------------------------------------
// Name: GroupDialog::OnValidate
// Created: NPT 2013-04-11
// -----------------------------------------------------------------------------
void GroupDialog::OnValidate()
{
    accept();
    InitialState& extension = selected_.ConstCast()->Get< InitialState >();
    for( auto it = extension.equipments_.begin(); it != extension.equipments_.end(); ++it )
        it->group_ = FindGroup( it->name_ );
    controllers_.controller_.Update( *selected_ );
}

// -----------------------------------------------------------------------------
// Name: GroupDialog::FindGroup
// Created: MCO 2013-04-22
// -----------------------------------------------------------------------------
boost::optional< QString > GroupDialog::FindGroup( const QString& name )
{
    for( int row = 0; row < dataModel_.rowCount(); ++row )
        if( name == dataModel_.item( row, 0 )->text() )
        {
            for( int column = 2; column < dataModel_.columnCount(); ++column )
                if( Decrease( row, column ) )
                    return dataModel_.horizontalHeaderItem( column )->text();
            return boost::none;
        }
    return boost::none;
}

// -----------------------------------------------------------------------------
// Name: GroupDialog::OnShow
// Created: NPT 2013-04-12
// -----------------------------------------------------------------------------
void GroupDialog::OnShow()
{
    if( !selected_ )
        return;
    dataModel_.clear();
    QStringList horizontalHeaders;
    horizontalHeaders << tr( "Equipment" ) << tr( "Total" );
    dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
    table_.horizontalHeader()->setResizeMode( 0, QHeaderView::ResizeToContents );
    table_.horizontalHeader()->setDefaultAlignment( Qt::AlignLeft );
    const InitialState& extension = selected_->Get< InitialState >();
    for( auto it = extension.equipments_.begin(); it != extension.equipments_.end(); ++it )
    {
        const int row = AddEquipment( it->name_ );
        Increase( row, 1 );
        if( it->group_ )
            Increase( row, AddGroup( *it->group_ ) );
    }
    exec();
}

// -----------------------------------------------------------------------------
// Name: GroupDialog::AddGroup
// Created: MCO 2013-04-16
// -----------------------------------------------------------------------------
int GroupDialog::AddGroup( const QString& name )
{
    int column = 2;
    for( ; column < dataModel_.columnCount(); ++column )
        if( dataModel_.horizontalHeaderItem( column )->text() == name )
            return column;
    QStandardItem* item = new QStandardItem( name );
    dataModel_.setHorizontalHeaderItem( column, item );
    delegate_->AddSpinBoxOnColumn( column );
    for( int row = 0; row < dataModel_.rowCount(); ++row )
    {
        QStandardItem* item = new QStandardItem( "0" );
        item->setFlags( item->flags() | Qt::ItemIsEditable );
        item->setTextAlignment( Qt::AlignRight );
        dataModel_.setItem( row, column, item );
    }
    return column;
}

// -----------------------------------------------------------------------------
// Name: GroupDialog::AddEquipment
// Created: MCO 2013-04-17
// -----------------------------------------------------------------------------
int GroupDialog::AddEquipment( const QString& name )
{
    QList< QStandardItem* > items = dataModel_.findItems( name );
    if( ! items.empty() )
        return items.first()->row();
    const int row = dataModel_.rowCount();
    AddItem( row, 0, name );
    AddItem( row, 1 );
    for( int column = 2; column < dataModel_.columnCount(); ++column )
        AddItem( row, column, "0", Qt::ItemIsEditable );
    return row;
}

// -----------------------------------------------------------------------------
// Name: GroupDialog::AddItem
// Created: MCO 2013-04-17
// -----------------------------------------------------------------------------
void GroupDialog::AddItem( int row, int column, const QString& text, Qt::ItemFlags flags )
{
    QStandardItem* item = new QStandardItem( text );
    item->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable | flags );
    item->setTextAlignment( Qt::AlignRight );
    dataModel_.setItem( row, column, item );
}

// -----------------------------------------------------------------------------
// Name: GroupDialog::Increase
// Created: MCO 2013-04-17
// -----------------------------------------------------------------------------
void GroupDialog::Increase( int row, int column )
{
    QStandardItem* item = dataModel_.item( row, column );
    const int count = item->text().toInt();
    item->setText( QString::number( count + 1 ) );
}

// -----------------------------------------------------------------------------
// Name: GroupDialog::Decrease
// Created: MCO 2013-04-22
// -----------------------------------------------------------------------------
bool GroupDialog::Decrease( int row, int column )
{
    QStandardItem* item = dataModel_.item( row, column );
    const int count = item->text().toInt();
    if( count == 0 )
        return false;
    item->setText( QString::number( count - 1 ) );
    return true;
}

// -----------------------------------------------------------------------------
// Name: GroupDialog::OnContextMenuRequested
// Created: MCO 2013-04-18
// -----------------------------------------------------------------------------
void GroupDialog::OnContextMenuRequested( const QPoint& pos )
{
    Q3PopupMenu menu( this );
    menu.insertItem( tr( "Create group" ), 0 );
    QModelIndex index = table_.indexAt( pos );
    if( index.column() >= 2 )
    {
        menu.insertItem( tr( "Delete group" ), 1 );
        menu.insertItem( tr( "Rename group" ), 2 );
    }
    int result = menu.exec( table_.viewport()->mapToGlobal( pos ) );
    if( result == 1 )
        dataModel_.removeColumn( index.column() );
    else if( result == 0 )
    {
        const QString group = InputName( tr( "Create group" ), "" );
        if( !group.isEmpty() )
            AddGroup( group );
    }
    else if( result == 2 )
    {
        const QString group = InputName( tr( "Rename group" ),
            dataModel_.horizontalHeaderItem( index.column() )->text() );
        if( !group.isEmpty() )
            dataModel_.horizontalHeaderItem( index.column() )->setText( group );
    }
}

// -----------------------------------------------------------------------------
// Name: GroupDialog::InputName
// Created: MCO 2013-04-19
// -----------------------------------------------------------------------------
QString GroupDialog::InputName( const QString& label, const QString& previous )
{
    bool ok;
    return QInputDialog::getText(
        this, label, tr( "Group name" ), QLineEdit::Normal, previous, &ok );
}
