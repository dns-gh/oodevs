// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "TacticalTreeView.h"
#include "resources.h"
#include "ItemPixmapDelegate.h"
#include "clients_kernel/AutomatDecisions_ABC.h"
#include "clients_kernel/CommandPostAttributes_ABC.h"
#include <boost/bind.hpp>

using namespace gui;

namespace
{
    bool IsCommandPost( const kernel::Entity_ABC& entity )
    {
        if( const kernel::CommandPostAttributes_ABC* pAttributes = entity.Retrieve< kernel::CommandPostAttributes_ABC >() )
            return pAttributes->IsCommandPost();
        return false;
    }

    bool IsEngaged( const kernel::Entity_ABC& entity )
    {
        if( const kernel::AutomatDecisions_ABC* decisions = entity.Retrieve< kernel::AutomatDecisions_ABC >() )
            return decisions->IsEmbraye();
        return false;
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView constructor
// Created: JSR 2012-08-31
// -----------------------------------------------------------------------------
TacticalTreeView::TacticalTreeView( const QString& objectName, kernel::Controllers& controllers, const kernel::Profile_ABC& profile, ModelObserver_ABC& modelObserver, const EntitySymbols& symbols, QWidget* parent /*= 0*/ )
    : HierarchyTreeView< kernel::TacticalHierarchies >( objectName, controllers, profile, modelObserver, symbols, parent )
    , commandPost_( MAKE_PIXMAP( commandpost ) )
    , lock_( MAKE_PIXMAP( lock ) )
{
    setItemDelegate( new ItemPixmapDelegate( dataModel_, boost::bind( &TacticalTreeView::GetEntityPixmap, this, _1 ), this ) );
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView destructor
// Created: JSR 2012-08-31
// -----------------------------------------------------------------------------
TacticalTreeView::~TacticalTreeView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::GetEntityPixmap
// Created: JSR 2012-09-14
// -----------------------------------------------------------------------------
std::vector< const QPixmap* > TacticalTreeView::GetEntityPixmap( const kernel::Entity_ABC& entity )
{
    std::vector< const QPixmap* > ret;
    if( IsCommandPost( entity ) )
        ret.push_back( &commandPost_ );
    if( IsEngaged( entity ) )
        ret.push_back( &lock_ );
    return ret;
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::LessThan
// Created: JSR 2012-09-06
// -----------------------------------------------------------------------------
bool TacticalTreeView::LessThan( const QModelIndex& left, const QModelIndex& right, bool& valid ) const
{
    const kernel::Entity_ABC* entity1 = dataModel_.GetDataFromIndex< kernel::Entity_ABC >( left );
    const kernel::Entity_ABC* entity2 = dataModel_.GetDataFromIndex< kernel::Entity_ABC >( right );
    if( !entity1 || !entity2 )
        return false;
    valid = true;
    if( IsCommandPost( *entity1 ) )
        return true;
    if( IsCommandPost( *entity2 ) )
        return false;
    return entity1->GetId() < entity2->GetId();
}
