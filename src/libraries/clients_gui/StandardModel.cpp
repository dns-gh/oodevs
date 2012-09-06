// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "StandardModel.h"
#include "moc_StandardModel.cpp"

using namespace gui;

const QString StandardModel::showValue_ = "show";
const QString StandardModel::hideValue_ = "hide";

// -----------------------------------------------------------------------------
// Name: StandardModel constructor
// Created: ABR 2012-08-13
// -----------------------------------------------------------------------------
StandardModel::StandardModel( kernel::Controllers& controllers, QSortFilterProxyModel& proxy, QObject* parent /*= 0*/ )
    : QStandardItemModel( parent )
    , controllers_( controllers )
    , proxy_( proxy )
{
    proxy.setSourceModel( this );
}

// -----------------------------------------------------------------------------
// Name: StandardModel destructor
// Created: ABR 2012-08-13
// -----------------------------------------------------------------------------
StandardModel::~StandardModel()
{
    Purge();
}

// -----------------------------------------------------------------------------
// Name: StandardModel::setData
// Created: JSR 2012-08-31
// -----------------------------------------------------------------------------
bool StandardModel::setData( const QModelIndex& index, const QVariant& value, int role /*= Qt::EditRole*/ )
{
    bool ret = QStandardItemModel::setData( index, value, role );
    if( ret && role == Qt::EditRole )
        emit DataChanged( index, value );
    return ret;
}

// -----------------------------------------------------------------------------
// Name: StandardModel::Purge
// Created: ABR 2012-08-14
// -----------------------------------------------------------------------------
void StandardModel::Purge()
{
    // $$$$ ABR 2012-08-14: TODO
}

namespace
{
    bool HasAnyChildVisible( QStandardItem* root, boost::function< bool ( QStandardItem* ) > func )
    {
        // $$$$ ABR 2012-08-16: TODO: Not tested yet, need to test it on gaming
        if( !root )
            return false;

        bool isVisible = func( root );
        for( int row = 0; row < root->rowCount(); ++row )
        {
            QStandardItem* childItem = root->child( row, 0 );
            if( childItem )
            {
                bool childVisible = HasAnyChildVisible( childItem, func );
                childItem->setData( childVisible ? StandardModel::showValue_ : StandardModel::hideValue_, StandardModel::FilterRole );
                isVisible = isVisible || childVisible;
            }
        }
        return isVisible;
    }
}

// -----------------------------------------------------------------------------
// Name: StandardModel::function< bool
// Created: ABR 2012-08-17
// -----------------------------------------------------------------------------
void StandardModel::ApplyFilter( boost::function< bool ( QStandardItem* ) > func )
{
    for( int row = 0; row < rowCount(); ++row )
    {
        QStandardItem* childItem = item( row, 0 );
        if( childItem )
            childItem->setData( ( ::HasAnyChildVisible( childItem, func ) ) ? StandardModel::showValue_ : StandardModel::hideValue_, StandardModel::FilterRole );
    }
}
