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
#include "moc_DrawingsTreeView.cpp"
#include "ModelObserver_ABC.h"
#include "ParametersLayer.h"
#include "clients_kernel/Drawing_ABC.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/TacticalLine_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Pathfind_ABC.h"
#include "clients_kernel/Tools.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: DrawingsTreeView constructor
// Created: LGY 2014-05-07
// -----------------------------------------------------------------------------
DrawingsTreeView::DrawingsTreeView( const QString& objectName, kernel::Controllers& controllers, const kernel::Profile_ABC& profile,
                                    ModelObserver_ABC& modelObserver, ParametersLayer& paramLayer, QWidget* parent /*= 0*/ )
    : EntityTreeView_ABC( objectName, controllers, profile, modelObserver, parent )
    , paramLayer_( paramLayer )
    , entity_( controllers )
{
    drawingsItem_ = dataModel_.AddRootTextItem( dataModel_.rowCount(), 0, tools::translate( "DrawingsTreeView", "Drawings" ), "" );
    itinerariesItem_ = dataModel_.AddRootTextItem( dataModel_.rowCount(), 0, tools::translate( "DrawingsTreeView", "Itineraries" ), "" );
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

namespace
{
    bool CanBeOrdered( const kernel::Entity_ABC& entity, const kernel::Profile_ABC& profile )
    {
        const kernel::Entity_ABC* superior = entity.Get< kernel::TacticalHierarchies >().GetSuperior();
        return superior && profile.CanBeOrdered( *superior );
    }
}

// -----------------------------------------------------------------------------
// Name: DrawingsTreeView::NotifyCreated
// Created: LGY 2014-05-07
// -----------------------------------------------------------------------------
void DrawingsTreeView::NotifyCreated( const kernel::TacticalLine_ABC& line )
{
    const auto flags = CanBeOrdered( line, profile_ ) ? Qt::ItemIsEditable | ItemSpecificFlags( line ) :
        ItemSpecificFlags( line );
    QStandardItem* parent = line.IsLimit() ? limitsItem_ : phaseLinesItem_;
    dataModel_.AddChildSafeItem( parent, parent->rowCount(), 0, line.GetName(),
        line.GetTooltip(), line, flags );
    EntityTreeView_ABC::ApplyProfileFilter();
}

// -----------------------------------------------------------------------------
// Name: DrawingsTreeView::NotifyCreated
// Created: LGY 2014-05-07
// -----------------------------------------------------------------------------
void DrawingsTreeView::NotifyCreated( const kernel::Drawing_ABC& drawing )
{
    dataModel_.AddChildSafeItem( drawingsItem_, drawingsItem_->rowCount(), 0, drawing.GetName(),
        drawing.GetTooltip(), drawing, Qt::ItemIsEditable | ItemSpecificFlags( drawing ) );
    EntityTreeView_ABC::ApplyProfileFilter();
}

// -----------------------------------------------------------------------------
// Name: DrawingsTreeView::NotifyCreated
// Created: LGY 2014-05-07
// -----------------------------------------------------------------------------
void DrawingsTreeView::NotifyCreated( const kernel::Pathfind_ABC& pathfind )
{
    dataModel_.AddChildSafeItem( itinerariesItem_, itinerariesItem_->rowCount(), 0, pathfind.GetName(),
        pathfind.GetTooltip(), pathfind, Qt::ItemIsEditable | ItemSpecificFlags( pathfind ) );
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
    dataModel_.PurgeChildren( *itinerariesItem_ );
}

// -----------------------------------------------------------------------------
// Name: DrawingsTreeView::ApplyProfileFilter
// Created: LGY 2014-05-07
// -----------------------------------------------------------------------------
bool DrawingsTreeView::ApplyProfileFilter( QStandardItem& item ) const
{
    const auto index = item.index();
    if( drawingsItem_->index() == index || limitsItem_->index() == index ||
        phaseLinesItem_->index() == index )
        return true;
    if( const kernel::Entity_ABC* drawing = dataModel_.GetDataFromIndex< kernel::Entity_ABC >( index ) )
        if( drawing->GetTypeName() == kernel::Drawing_ABC::typeName_ )
        {
            if( const kernel::Entity_ABC* entity = static_cast< const kernel::Drawing_ABC* >( drawing )->GetDiffusionEntity() )
                return profile_.IsVisible( *entity );
            return true;
        }
    return EntityTreeView_ABC::ApplyProfileFilter( item );
}

// -----------------------------------------------------------------------------
// Name: DrawingsTreeView::keyPressEvent
// Created: LGY 2014-05-07
// -----------------------------------------------------------------------------
void DrawingsTreeView::keyPressEvent( QKeyEvent* event )
{
    const QModelIndex index = selectionModel()->currentIndex();
    if( event && index.isValid() )
    {
        if( event->key() == Qt::Key_Delete )
        {
            if( kernel::Entity_ABC* entity = dataModel_.GetDataFromIndex< kernel::Entity_ABC >( index ) )
                modelObserver_.DeleteEntity( *entity );
        }
        else if( event->key() == Qt::Key_Escape )
            paramLayer_.Reset();
    }
    EntityTreeView_ABC::keyPressEvent( event );
}

// -----------------------------------------------------------------------------
// Name: DrawingsTreeView::AddCommonMenu
// Created: LGY 2014-05-07
// -----------------------------------------------------------------------------
void DrawingsTreeView::AddCommonMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu )
{
    entity_ = &entity;
    menu.InsertItem( "Command", tr( "Rename" ), this, SLOT( OnRename() ), false, 4 );
}

// -----------------------------------------------------------------------------
// Name: DrawingsTreeView::NotifyContextMenu
// Created: LGY 2014-05-07
// -----------------------------------------------------------------------------
void DrawingsTreeView::NotifyContextMenu( const kernel::Drawing_ABC& drawing, kernel::ContextMenu& menu )
{
    AddCommonMenu( drawing, menu );
}

// -----------------------------------------------------------------------------
// Name: DrawingsTreeView::NotifyContextMenu
// Created: LGY 2014-05-07
// -----------------------------------------------------------------------------
void DrawingsTreeView::NotifyContextMenu( const kernel::TacticalLine_ABC& line, kernel::ContextMenu& menu )
{
    if( CanBeOrdered( line, profile_ ) )
        AddCommonMenu( line, menu );
}

// -----------------------------------------------------------------------------
// Name: DrawingsTreeView::NotifyContextMenu
// Created: LGY 2014-05-07
// -----------------------------------------------------------------------------
void DrawingsTreeView::NotifyContextMenu( const kernel::Pathfind_ABC& pathfind, kernel::ContextMenu& menu )
{
    AddCommonMenu( pathfind, menu );
}

// -----------------------------------------------------------------------------
// Name: DrawingsTreeView::OnRename
// Created: LGY 2014-05-07
// -----------------------------------------------------------------------------
void DrawingsTreeView::OnRename()
{
    if( entity_ )
        Rename( *entity_.ConstCast() );
}
