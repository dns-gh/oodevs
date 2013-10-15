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
#include "clients_kernel/Tools.h"
#include <boost/bind.hpp>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: TacticalTreeView constructor
// Created: JSR 2012-08-31
// -----------------------------------------------------------------------------
TacticalTreeView::TacticalTreeView( const QString& objectName, kernel::Controllers& controllers, const kernel::Profile_ABC& profile, ModelObserver_ABC& modelObserver, const EntitySymbols& symbols, QWidget* parent /*= 0*/ )
    : HierarchyTreeView< kernel::TacticalHierarchies >( objectName, controllers, profile, modelObserver, symbols, parent )
    , commandPost_( MAKE_PIXMAP( commandpost ) )
    , lock_( MAKE_PIXMAP( lock ) )
{
    SetLessThanEntityFunctor( &tools::LessThanByPC );
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
    if( tools::IsCommandPost( entity ) )
        ret.push_back( &commandPost_ );
    if( tools::IsEngaged( entity ) )
        ret.push_back( &lock_ );
    return ret;
}
