// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "PopulationTreeView.h"
#include "clients_gui/ItemPixmapDelegate.h"
#include "clients_kernel/Profile_ABC.h"
#include "tools/GeneralConfig.h"
#include <boost/bind.hpp>

// -----------------------------------------------------------------------------
// Name: PopulationTreeView constructor
// Created: JSR 2013-01-18
// -----------------------------------------------------------------------------
PopulationTreeView::PopulationTreeView( const QString& objectName, kernel::Controllers& controllers, const kernel::Profile_ABC& profile, gui::ModelObserver_ABC& modelObserver, QWidget* parent /*= 0*/ )
    : gui::PopulationTreeView( objectName, controllers, profile, modelObserver, true, parent )
    , icon_user_( tools::GeneralConfig::BuildResourceChildFile( "images/gaming/icon_user.png" ).ToUTF8().c_str() )
{
    setItemDelegate( new gui::ItemPixmapDelegate( dataModel_, boost::bind( &PopulationTreeView::GetEntityPixmap, this, _1 ), this, 20 ) );
}

// -----------------------------------------------------------------------------
// Name: PopulationTreeView destructor
// Created: JSR 2013-01-18
// -----------------------------------------------------------------------------
PopulationTreeView::~PopulationTreeView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationTreeView::GetEntityPixmap
// Created: JSR 2013-01-18
// -----------------------------------------------------------------------------
std::vector< const QPixmap* > PopulationTreeView::GetEntityPixmap( const kernel::Entity_ABC& entity )
{
    std::vector< const QPixmap* > ret;
    if( dynamic_cast< const kernel::Population_ABC* >( &entity ) )
        if( profile_.CanBeOrdered( entity ) )
            ret.push_back( &icon_user_ );
    QStandardItem* item = dataModel_.FindDataItem( entity );
    if( item && !isExpanded( proxyModel_->mapFromSource( dataModel_.indexFromItem( item ) ) ) )
        for( int i = 0; i < item->rowCount(); ++i )
            if( QStandardItem* sub = item->child( i ) )
                if( kernel::Population_ABC* population = dataModel_.GetDataFromItem< kernel::Population_ABC >( *sub ) )
                    if( profile_.CanBeOrdered( *population ) )
                    {
                        ret.push_back( &icon_user_ );
                        break;
                    }
    return ret;
}
