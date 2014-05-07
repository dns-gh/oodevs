// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "DrawingsTreeView.h"
#include "clients_kernel/TacticalLine_ABC.h"
#include "clients_kernel/Drawing_ABC.h"
#include "clients_kernel/Tools.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: DrawingsTreeView constructor
// Created: LGY 2014-05-07
// -----------------------------------------------------------------------------
DrawingsTreeView::DrawingsTreeView( const QString& objectName, kernel::Controllers& controllers, const kernel::Profile_ABC& profile,
                                    ModelObserver_ABC& modelObserver, QWidget* parent /*= 0*/ )
    : EntityTreeView_ABC( objectName, controllers, profile, modelObserver, parent )
{
    drawingsItem_ = dataModel_.AddRootTextItem( dataModel_.rowCount(), 0, tools::translate( "DrawingsTreeView", "Drawings" ), "" );
    limitsItem_ = dataModel_.AddRootTextItem( dataModel_.rowCount(), 0, tools::translate( "DrawingsTreeView", "Limits" ), "" );
    phaseLinesItem_ = dataModel_.AddRootTextItem( dataModel_.rowCount(), 0, tools::translate( "DrawingsTreeView", "Phase lines" ), "" );
    controllers.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: DrawingsTreeView destructor
// Created: LGY 2014-05-07
// -----------------------------------------------------------------------------
DrawingsTreeView::~DrawingsTreeView()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: DrawingsTreeView::NotifyCreated
// Created: LGY 2014-05-07
// -----------------------------------------------------------------------------
void DrawingsTreeView::NotifyCreated( const kernel::TacticalLine_ABC& line )
{
    QStandardItem* parent = line.IsLimit() ? limitsItem_ : phaseLinesItem_;
    dataModel_.AddChildSafeItem( parent, parent->rowCount(), 0, line.GetName(),
            line.GetTooltip(), line, ItemSpecificFlags( line ) );
    EntityTreeView_ABC::ApplyProfileFilter();
}

// -----------------------------------------------------------------------------
// Name: DrawingsTreeView::NotifyCreated
// Created: LGY 2014-05-07
// -----------------------------------------------------------------------------
void DrawingsTreeView::NotifyCreated( const kernel::Drawing_ABC& drawing )
{
    dataModel_.AddChildSafeItem( drawingsItem_, drawingsItem_->rowCount(), 0, drawing.GetName(),
        drawing.GetTooltip(), drawing, ItemSpecificFlags( drawing ) );
    EntityTreeView_ABC::ApplyProfileFilter();
}

// -----------------------------------------------------------------------------
// Name: DrawingsTreeView::NotifyCreated
// Created: LGY 2014-05-07
// -----------------------------------------------------------------------------
void DrawingsTreeView::NotifyCreated( const kernel::Team_ABC& /*team*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DrawingsTreeView::Purge
// Created: LGY 2014-05-07
// -----------------------------------------------------------------------------
void DrawingsTreeView::Purge()
{
    dataModel_.PurgeChildren( *drawingsItem_ );
    dataModel_.PurgeChildren( *limitsItem_ );
    dataModel_.PurgeChildren( *phaseLinesItem_ );
}
// -----------------------------------------------------------------------------
// Name: DrawingsTreeView::ApplyProfileFilter
// Created: LGY 2014-05-07
// -----------------------------------------------------------------------------
bool DrawingsTreeView::ApplyProfileFilter( QStandardItem& item, StandardModel& model ) const
{
    const auto index = item.index();
    if( drawingsItem_->index() == index || limitsItem_->index() == index ||
        phaseLinesItem_->index() == index )
        return true;
    return EntityTreeView_ABC::ApplyProfileFilter( item, model );
}
