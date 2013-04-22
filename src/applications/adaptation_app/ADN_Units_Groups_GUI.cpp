// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Units_Groups_GUI.h"
#include "moc_ADN_Units_Groups_GUI.cpp"
#include "ADN_Units_Data.h"
#include "ADN_TableItem_Edit.h"
#include "clients_gui/RichSpinBox.h"
#include "clients_gui/CommonDelegate.h"
#include <boost/make_shared.hpp>
#include <boost/lexical_cast.hpp>

typedef ADN_Units_Data::ComposanteInfos UnitComposanteInfos;

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
// Name: ADN_Units_Groups_GUI constructor
// Created: MCO 2013-04-16
// -----------------------------------------------------------------------------
ADN_Units_Groups_GUI::ADN_Units_Groups_GUI( const Q3Table& model, QWidget* parent /*= 0*/ )
    : QTableView( parent )
    , dataModel_( parent )
    , delegate_ ( new GroupsDelegate( this, proxyModel_, dataModel_ ) )
{
    setObjectName( "groups" );
    proxyModel_.setSourceModel( &dataModel_ );
    setModel( &proxyModel_ );
    setItemDelegate( delegate_.get() );
    setEditTriggers( AllEditTriggers );
    setSortingEnabled( true );
    setShowGrid( true );
    setAlternatingRowColors( true );
    setSelectionMode( SingleSelection );
    setSelectionBehavior( SelectItems );
    verticalHeader()->setVisible( false );
    QStringList horizontalHeaders;
    horizontalHeaders << tr( "Equipment" ) << tr( "Total" );
    dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
    horizontalHeader()->setResizeMode( 0, QHeaderView::ResizeToContents );
    horizontalHeader()->setDefaultAlignment( Qt::AlignLeft );
    for( int row = 0; row < model.numRows(); ++row )
    {
        ADN_TableItem_ABC* item = static_cast< ADN_TableItem_ABC* >( model.item( row, 0 ) );
        UnitComposanteInfos* infos = static_cast< UnitComposanteInfos* >( item->GetData() );
        AddItem( row, 0, infos->ptrComposante_.GetData()->strName_.GetData().c_str(), infos );
        AddItem( row, 1, QString::number( infos->nNb_.GetData() ) );
        for( auto it = infos->groups_.begin(); it != infos->groups_.end(); ++it )
        {
            const int column = AddGroup( it->first.c_str(), model.numRows() );
            dataModel_.item( row, column )->setText( QString::number( it->second ) );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_Groups_GUI destructor
// Created: MCO 2013-04-16
// -----------------------------------------------------------------------------
ADN_Units_Groups_GUI::~ADN_Units_Groups_GUI()
{
    // NOTHING
}

namespace
{
    bool IsInt( const QString& text )
    {
        bool result = false;
        text.toInt( &result );
        return result;
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_Groups_GUI::AddItem
// Created: MCO 2013-04-17
// -----------------------------------------------------------------------------
void ADN_Units_Groups_GUI::AddItem( int row, int column, const QString& text, void* data )
{
    QStandardItem* item = new QStandardItem( text );
    item->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
    if( IsInt( text ) )
        item->setTextAlignment( Qt::AlignRight );
    if( data )
        item->setData( QVariant::fromValue( data ) );
    dataModel_.setItem( row, column, item );
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_Groups_GUI::AddGroup
// Created: MCO 2013-04-16
// -----------------------------------------------------------------------------
int ADN_Units_Groups_GUI::AddGroup( const QString& name, int rows )
{
    int column = 2;
    for( ; column < dataModel_.columnCount(); ++column )
        if( dataModel_.horizontalHeaderItem( column )->text() == name )
            return column;
    QStandardItem* item = new QStandardItem( name );
    dataModel_.setHorizontalHeaderItem( column, item );
    delegate_->AddSpinBoxOnColumn( column );
    for( int row = 0; row < rows; ++row )
    {
        QStandardItem* item = new QStandardItem( "0" );
        item->setFlags( item->flags() | Qt::ItemIsEditable );
        item->setTextAlignment( Qt::AlignRight );
        dataModel_.setItem( row, column, item );
    }
    return column;
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_Groups_GUI::contextMenuEvent
// Created: MCO 2013-04-17
// -----------------------------------------------------------------------------
void ADN_Units_Groups_GUI::contextMenuEvent( QContextMenuEvent* event )
{
    Q3PopupMenu menu( this );
    menu.insertItem( tr( "Create group" ), 0 );
    QModelIndex index = indexAt( event->pos() );
    if( index.column() >= 2 )
    {
        menu.insertItem( tr( "Delete group" ), 1 );
        menu.insertItem( tr( "Rename group" ), 2 );
    }
    int result = menu.exec( event->globalPos() );
    if( result == 1 )
        dataModel_.removeColumn( index.column() );
    else if( result == 0 )
    {
        const QString group = InputName( tr( "Create group" ), "" );
        if( !group.isEmpty() )
            AddGroup( group, dataModel_.rowCount() );
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
// Name: ADN_Units_Groups_GUI::InputName
// Created: MCO 2013-04-19
// -----------------------------------------------------------------------------
QString ADN_Units_Groups_GUI::InputName( const QString& label, const QString& previous )
{
    bool ok;
    return QInputDialog::getText(
        this, label, tr( "Group name" ), QLineEdit::Normal, previous, &ok );
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_Groups_GUI::Commit
// Created: MCO 2013-04-16
// -----------------------------------------------------------------------------
void ADN_Units_Groups_GUI::Commit() const
{
    for( int row = 0; row < dataModel_.rowCount(); ++row )
    {
        QStandardItem* item = dataModel_.item( row, 0 );
        UnitComposanteInfos* infos = static_cast< UnitComposanteInfos* >( item->data().value< void* >() );
        infos->groups_.clear();
        for( int column = 2; column < dataModel_.columnCount(); ++column )
        {
            const int count = dataModel_.item( row, column )->text().toInt();
            if( count != 0 )
                infos->groups_.push_back(
                    std::make_pair( dataModel_.horizontalHeaderItem( column )->text().toStdString(), count ) );
        }
    }
}
