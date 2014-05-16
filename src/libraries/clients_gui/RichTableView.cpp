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
    , proxyModel_( parent )
    , dataModel_( 0, proxyModel_, parent )
    , delegate_( parent )
{
    proxyModel_.setSourceModel( &dataModel_ );
    proxyModel_.setDynamicSortFilter( true );
    proxyModel_.setSortRole( Roles::DataRole );

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
// Name: RichTableView::ApplyFilter
// Created: ABR 2014-04-28
// -----------------------------------------------------------------------------
void RichTableView::ApplyFilters( const std::map< int, std::vector< std::shared_ptr< Filter_ABC > > >& filters )
{
    dataModel_.ApplyFilters( filters );
}

// -----------------------------------------------------------------------------
// Name: RichTableView::CreateFilters
// Created: ABR 2012-08-14
// -----------------------------------------------------------------------------
void RichTableView::CreateFilters( RichView_ABC& /* richView */ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RichTableView::Purge
// Created: ABR 2012-08-14
// -----------------------------------------------------------------------------
void RichTableView::Purge()
{
    //dataModel_.Purge();
    dataModel_.removeRows( 0, dataModel_.rowCount() );
}

// -----------------------------------------------------------------------------
// Name: RichTableView::GetHeader
// Created: ABR 2014-04-25
// -----------------------------------------------------------------------------
QHeaderView* RichTableView::GetHeader() const
{
    return horizontalHeader();
}
