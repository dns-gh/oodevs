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
#include "ADN_TableItem_ABC.h"
#include "ADN_TableItem_Edit.h"
#include "clients_gui/RichSpinBox.h"
#include <boost/make_shared.hpp>

typedef ADN_Units_Data::ComposanteInfos UnitComposanteInfos;

class GroupsHeader : public QHeaderView
{
public:
    GroupsHeader( QWidget* parent )
        : QHeaderView( Qt::Horizontal, parent )
    {
        setClickable( true );
    }
public:
    int AddHeader( const std::string& name )
    {
        int column = 2;
        for( ; column < widgets_.size(); ++column )
            if( widgets_[ column ]->text() == name.c_str() )
                return column;
        widgets_.resize( column + 1 );
        QTextEdit*& edit = widgets_[ column ];
        edit = new QTextEdit( name.c_str(), this );
        edit->setVisible( true );
        return column;
    }
    void RemoveHeader( int column )
    {
        if( column >= widgets_.size() )
            return;
        delete widgets_[ column ];
        widgets_.erase( widgets_.begin() + column );
    }
    std::string GetHeader( int column ) const
    {
        if( column >= widgets_.size() || ! widgets_[ column ] )
            return "";
        return widgets_[ column ]->text().toStdString();
    }

private:
    virtual void paintEvent( QPaintEvent* e )
    {
        for( int i = 2; i < count(); ++i )
            widgets_[ i ]->setGeometry( sectionViewportPosition( i ), 0, sectionSize( i ) - 2, height() );
        QHeaderView::paintEvent( e );
    }

private:
    std::vector< QTextEdit* > widgets_;
};

class GroupsDelegate : public gui::CommonDelegate
{
public:
    explicit GroupsDelegate( QObject* parent, const QStandardItemModel& dataModel )
        : gui::CommonDelegate( parent )
        , dataModel_( dataModel )
    {}
    virtual QWidget* createEditor( QWidget* parent, const QStyleOptionViewItem& /*option*/, const QModelIndex& index ) const
    {
        const int row = index.row();
        int max = dataModel_.item( row, 1 )->text().toInt();
        for( int c = 2; c < dataModel_.columnCount(); ++c )
            max -= dataModel_.item( row, c )->text().toInt();
        return new gui::RichSpinBox( parent, 0, max );
    }
    const QStandardItemModel& dataModel_;
};

// -----------------------------------------------------------------------------
// Name: ADN_Units_Groups_GUI constructor
// Created: MCO 2013-04-16
// -----------------------------------------------------------------------------
ADN_Units_Groups_GUI::ADN_Units_Groups_GUI( const Q3Table& model, QWidget* parent /*= 0*/ )
    : QTableView( parent )
    , header_   ( new GroupsHeader( this ) )
    , dataModel_( parent )
    , delegate_ ( new GroupsDelegate( this, dataModel_ ) )
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
    setHorizontalHeader( header_ );
    QStringList horizontalHeaders;
    horizontalHeaders << tr( "Equipment" ) << tr( "Total" );
    dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
    horizontalHeader()->setResizeMode( 0, QHeaderView::ResizeToContents );
    for( int row = 0; row < model.numRows(); ++row )
    {
        ADN_TableItem_ABC* item = static_cast< ADN_TableItem_ABC* >( model.item( row, 0 ) );
        UnitComposanteInfos* infos = static_cast< UnitComposanteInfos* >( item->GetData() );
        AddItem( row, 0, infos->ptrComposante_.GetData()->strName_.GetData().c_str(), infos );
        AddItem( row, 1, QString::number( infos->nNb_.GetData() ) );
        for( auto it = infos->groups_.begin(); it != infos->groups_.end(); ++it )
        {
            const int column = AddGroup( it->first, model.numRows() );
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

// -----------------------------------------------------------------------------
// Name: ADN_Units_Groups_GUI::AddItem
// Created: MCO 2013-04-17
// -----------------------------------------------------------------------------
void ADN_Units_Groups_GUI::AddItem( int row, int column, const QString& text, void* data )
{
    QStandardItem* item = new QStandardItem( text );
    item->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
    if( data )
        item->setData( QVariant::fromValue( data ) );
    dataModel_.setItem( row, column, item );
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_Groups_GUI::AddGroup
// Created: MCO 2013-04-16
// -----------------------------------------------------------------------------
int ADN_Units_Groups_GUI::AddGroup( const std::string& name, int rows )
{
    int column = header_->AddHeader( name );
    if( column >= dataModel_.columnCount() )
    {
        delegate_->AddSpinBoxOnColumn( column );
        for( int row = 0; row < rows; ++row )
        {
            QStandardItem* item = new QStandardItem( 0 );
            dataModel_.setItem( row, column, item );
            item->setFlags( item->flags() | Qt::ItemIsEditable );
        }
    }
    return column;
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_Groups_GUI::RemoveGroup
// Created: MCO 2013-04-16
// -----------------------------------------------------------------------------
void ADN_Units_Groups_GUI::RemoveGroup( int column )
{
    dataModel_.removeColumn( column );
    header_->RemoveHeader( column );
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_Groups_GUI::contextMenuEvent
// Created: MCO 2013-04-17
// -----------------------------------------------------------------------------
void ADN_Units_Groups_GUI::contextMenuEvent( QContextMenuEvent* event )
{
    Q3PopupMenu menu( this );
    menu.insertItem( tr( "Add group" ), 0 );
    QModelIndex index = indexAt( event->pos() );
    if( index.column() >= 2 )
        menu.insertItem( tr( "Delete group" ), 1 );
    int result = menu.exec( event->globalPos() );
    if( result == 1 )
        RemoveGroup( index.column() );
    else if( result == 0 )
        AddGroup( "", dataModel_.rowCount() );
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
                infos->groups_.push_back( std::make_pair( header_->GetHeader( column ), count ) );
        }
    }
}
