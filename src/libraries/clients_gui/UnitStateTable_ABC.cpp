// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "UnitStateTable_ABC.h"

#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"

using namespace gui;

namespace bfs = boost::filesystem;

// -----------------------------------------------------------------------------
// Name: UnitStateTable_ABC constructor
// Created: ABR 2011-07-05
// -----------------------------------------------------------------------------
UnitStateTable_ABC::UnitStateTable_ABC( QWidget* parent, int numCols )
    : QTableView( parent )
    , dataModel_ ( parent )
    , proxyModel_( parent )
    , delegate_  ( parent )
{
    dataModel_.setColumnCount( numCols );
    proxyModel_.setSourceModel( &dataModel_ );
    proxyModel_.setSortRole( Qt::UserRole );
    setModel( &proxyModel_ );
    setItemDelegate( &delegate_ );

    setSortingEnabled( true );
    setShowGrid( false );
    setAlternatingRowColors( true );
    verticalHeader()->setVisible( false );
    setSelectionMode( NoSelection );
    setSelectionBehavior( SelectRows );
    setEditTriggers( AllEditTriggers );
}

// -----------------------------------------------------------------------------
// Name: UnitStateTable_ABC destructor
// Created: ABR 2011-07-05
// -----------------------------------------------------------------------------
UnitStateTable_ABC::~UnitStateTable_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitStateTable_ABC::Purge
// Created: ABR 2011-07-05
// -----------------------------------------------------------------------------
void UnitStateTable_ABC::Purge()
{
    dataModel_.clear();
    dataModel_.setHorizontalHeaderLabels( horizontalHeaders_ );
    horizontalHeader()->setResizeMode( QHeaderView::ResizeToContents );
    horizontalHeader()->setResizeMode( 0, QHeaderView::Stretch );
}

// -----------------------------------------------------------------------------
// Name: UnitStateTable_ABC::RecursiveLoad
// Created: ABR 2011-07-06
// -----------------------------------------------------------------------------
void UnitStateTable_ABC::RecursiveLoad( kernel::Entity_ABC& selected )
{
    QString typeName = selected.GetTypeName();
    if( typeName == kernel::Agent_ABC::typeName_ )
        Load( selected );
    else
    {
        assert( typeName == kernel::Automat_ABC::typeName_ || typeName == kernel::Formation_ABC::typeName_ || typeName == kernel::Team_ABC::typeName_ );
        const kernel::TacticalHierarchies& hierarchy = selected.Get< kernel::TacticalHierarchies >();
        tools::Iterator< const kernel::Entity_ABC& > it = hierarchy.CreateSubordinateIterator();
        while( it.HasMoreElements() )
            RecursiveLoad( const_cast< kernel::Entity_ABC& >( it.NextElement() ) );
    }
}

namespace
{
    std::string GetSeparator()
    {
        char buffer[256];
        GetLocaleInfo( LOCALE_SYSTEM_DEFAULT, LOCALE_SLIST, buffer, 256 );
        return buffer;
    }
}

// -----------------------------------------------------------------------------
// Name: UnitStateTable_ABC::Serialize
// Created: LGY 2012-01-30
// -----------------------------------------------------------------------------
void UnitStateTable_ABC::Serialize( const boost::filesystem::path& path ) const
{
    const std::string separator = GetSeparator();
    std::ofstream file( path.string().c_str() );
    for( QStringList::const_iterator it = horizontalHeaders_.constBegin(); it != horizontalHeaders_.constEnd(); ++it )
        if( *it != tools::translate( "UnitStateTable_ABC", "Location" ) )
            file << *it << separator;
    file  << std::endl;
    for( int i = 0; i < dataModel_.rowCount(); ++i )
    {
        for( int j = 0; j < dataModel_.columnCount(); ++j )
        {
            if( QStandardItem* item = dataModel_.item( i, j ) )
            {
                QString text;
                if( !item->isEnabled() )
                    text = item->text().isEmpty() ? tools::translate( "UnitStateTable_ABC", "No" ) : "";
                else
                {
                    if( item->text().isEmpty() )
                        text = item->checkState() == Qt::Checked ? tools::translate( "UnitStateTable_ABC", "Yes" ) :
                                                                   tools::translate( "UnitStateTable_ABC", "No" );
                    else
                        text = item->text();
                }
                file << text << separator;
            }
        }
        file << std::endl;
    }
}
