// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "RichTableView.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: RichTableView constructor
// Created: LGY 2014-01-09
// -----------------------------------------------------------------------------
RichTableView::RichTableView( const QString& objectName, QWidget* parent )
    : gui::RichWidget< QTableView >( objectName, parent )
    , dataModel_( parent )
    , proxyModel_( parent )
    , delegate_( parent )
{
    proxyModel_.setSourceModel( &dataModel_ );
    proxyModel_.setDynamicSortFilter( true );
    proxyModel_.setSortRole( Qt::UserRole );

    setModel( &proxyModel_ );
    setItemDelegate( &delegate_ );

    setShowGrid( true );
    setAlternatingRowColors( true );
    verticalHeader()->setVisible( false );
}

// -----------------------------------------------------------------------------
// Name: RichTableView destructor
// Created: LGY 2014-01-09
// -----------------------------------------------------------------------------
RichTableView::~RichTableView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RichTableView::Purge
// Created: ABR 2014-04-23
// -----------------------------------------------------------------------------
void RichTableView::Purge()
{
    dataModel_.removeRows( 0, dataModel_.rowCount() );
}
