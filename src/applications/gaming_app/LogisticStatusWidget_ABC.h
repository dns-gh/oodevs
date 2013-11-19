// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticStatusWidget_ABC_h_
#define __LogisticStatusWidget_ABC_h_

#include "clients_kernel/SafePointer.h"
#include "clients_gui/RichWidget.h"
#include "clients_gui/CommonDelegate.h"
#include "clients_kernel/Entity_ABC.h"
#include <tools/ElementObserver_ABC.h>
#include <tools/SelectionObserver_ABC.h>
#include <QtGui/QTableView>

namespace kernel
{
    class Controllers;
    class Entity_ABC;
}

namespace gui
{
    class ItemFactory_ABC;
    class DisplayBuilder;
}

// =============================================================================
/** @class  LogisticStatusWidget_ABC
    @brief  LogisticStatusWidget_ABC
*/
// Created: SBO 2007-02-19
// =============================================================================
template< typename Extension >
class LogisticStatusWidget_ABC : public gui::RichWidget< QTableView >
                               , public tools::Observer_ABC
                               , public tools::ElementObserver_ABC< Extension >
                               , public tools::SelectionObserver< kernel::Entity_ABC >
{
public:
    LogisticStatusWidget_ABC( const QString& objectName, QWidget* parent, kernel::Controllers& controllers )
        : gui::RichWidget< QTableView >( objectName, parent )
        , controllers_( controllers )
        , selected_( controllers )
    {
        dataModel_ = new QStandardItemModel( this );
        proxyModel_= new QSortFilterProxyModel( this );
        delegate_  = new gui::CommonDelegate( this );
        dataModel_->setColumnCount( 2 );
        proxyModel_->setSourceModel( dataModel_ );
        proxyModel_->setSortRole( Qt::UserRole );
        setModel( proxyModel_ );
        setItemDelegate( delegate_ );
        setSortingEnabled( false );
        setShowGrid( false );
        setAlternatingRowColors( false );
        horizontalHeader()->setVisible( false );
        verticalHeader()->setVisible( false );
        setSelectionMode( NoSelection );
        setEditTriggers( NoEditTriggers );
        setFrameStyle( QFrame::NoFrame );
        setFocusPolicy( Qt::NoFocus );
        horizontalHeader()->setResizeMode( QHeaderView::Stretch );
        QPalette p = palette();
        p.setColor( QPalette::Base, palette().background().color() );
        setPalette( p );
        controllers_.Register( *this );
    }
    virtual ~LogisticStatusWidget_ABC()
    {
        controllers_.Unregister( *this );
    }

    void Add( const QString& title, const QString& value )
    {
        const int row = dataModel_->rowCount();
        QStandardItem* itemTitle = new QStandardItem();
        itemTitle->setTextAlignment( Qt::AlignLeft | Qt::AlignVCenter );
        QFont curFont = itemTitle->font();
        curFont.setBold( true );
        curFont.setWeight( curFont.weight() - 2 );
        itemTitle->setFont( curFont );
        itemTitle->setData( QVariant( title ), Qt::DisplayRole );
        dataModel_->setItem( row, 0, itemTitle );

        QStandardItem* itemValue = new QStandardItem();
        itemValue->setTextAlignment( Qt::AlignLeft | Qt::AlignVCenter );
        itemValue->setData( QVariant( value ), Qt::DisplayRole );
        dataModel_->setItem( row, 1, itemValue );
    }

private:
    void showEvent( QShowEvent* )
    {
        const kernel::Entity_ABC* selected = selected_;
        selected_ = 0;
        NotifySelected( selected );
    }

    virtual void NotifyUpdated( const Extension& element )
    {
        if( selected_ && selected_->Retrieve< Extension >() == &element )
        {
            dataModel_->clear();
            OnUpdated( element );
        }
    }

    virtual void NotifySelected( const kernel::Entity_ABC* element )
    {
        selected_ = element;
        if( !selected_ )
            return;
        if( const Extension* extension = selected_->Retrieve< Extension >() )
        {
            show();
            NotifyUpdated( *extension );
        }
        else
            hide();
    }

    virtual void OnUpdated( const Extension& element ) = 0;

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    QStandardItemModel* dataModel_;
    QSortFilterProxyModel* proxyModel_;
    gui::CommonDelegate* delegate_;
    //@}
};

#endif // __LogisticStatusWidget_ABC_h_
