// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "RenameInterface.h"
#include "moc_RenameInterface.cpp"

#include "EntityTreeView_ABC.h"
#include "LongNameHelper.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Drawing_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Ghost_ABC.h"
#include "clients_kernel/Inhabitant_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/Pathfind_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/TacticalLine_ABC.h"
#include "clients_kernel/Team_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: RenameInterface constructor
// Created: ABR 2014-09-03
// -----------------------------------------------------------------------------
RenameInterface::RenameInterface( kernel::Controllers& controllers, QObject* parent /* = 0 */ )
    : QObject( parent )
    , controllers_( controllers )
    , contextMenuEntity_( controllers )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: RenameInterface destructor
// Created: ABR 2014-09-03
// -----------------------------------------------------------------------------
RenameInterface::~RenameInterface()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: RenameInterface::AddEntityTreeView
// Created: ABR 2014-09-03
// -----------------------------------------------------------------------------
void RenameInterface::AddEntityTreeView( gui::EntityTreeView_ABC* view )
{
    views_.push_back( view );
}

// -----------------------------------------------------------------------------
// Name: RenameInterface::OnRename
// Created: ABR 2014-09-03
// -----------------------------------------------------------------------------
void RenameInterface::OnRename()
{
    if( !contextMenuEntity_ )
        return;
    if( contextMenuView_ && contextMenuView_->IsActivated() && contextMenuView_->Exist( *contextMenuEntity_ ) )
        contextMenuView_->Rename( *contextMenuEntity_.ConstCast() );
    else
        gui::longname::ShowRenameDialog( qApp->activeWindow(), contextMenuEntity_ );
    contextMenuEntity_ = 0;
    contextMenuView_ = 0;
}

// -----------------------------------------------------------------------------
// Name: RenameInterface::AddCommonMenu
// Created: ABR 2014-09-03
// -----------------------------------------------------------------------------
void RenameInterface::AddCommonMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu )
{
    contextMenuView_ = 0;
    contextMenuEntity_ = &entity;
    if( entity.CanBeRenamed() )
    {
        auto it = std::find( views_.begin(), views_.end(), controllers_.actions_.GetEmitter() );
        if( it != views_.end() )
            contextMenuView_ = *it;
        menu.InsertItem( "Command", tr( "Rename" ), this, SLOT( OnRename() ), false, 4 );
    }
}

// -----------------------------------------------------------------------------
// Name: RenameInterface::NotifyContextMenu
// Created: ABR 2014-09-03
// -----------------------------------------------------------------------------
void RenameInterface::NotifyContextMenu( const kernel::Agent_ABC& agent, kernel::ContextMenu& menu )
{
    AddCommonMenu( agent, menu );
}

// -----------------------------------------------------------------------------
// Name: RenameInterface::NotifyContextMenu
// Created: ABR 2014-09-03
// -----------------------------------------------------------------------------
void RenameInterface::NotifyContextMenu( const kernel::Automat_ABC& automat, kernel::ContextMenu& menu )
{
    AddCommonMenu( automat, menu );
}

// -----------------------------------------------------------------------------
// Name: RenameInterface::NotifyContextMenu
// Created: ABR 2014-09-03
// -----------------------------------------------------------------------------
void RenameInterface::NotifyContextMenu( const kernel::Drawing_ABC& drawing, kernel::ContextMenu& menu )
{
    AddCommonMenu( drawing, menu );
}

// -----------------------------------------------------------------------------
// Name: RenameInterface::NotifyContextMenu
// Created: ABR 2014-09-03
// -----------------------------------------------------------------------------
void RenameInterface::NotifyContextMenu( const kernel::Formation_ABC& formation, kernel::ContextMenu& menu )
{
    AddCommonMenu( formation, menu );
}

// -----------------------------------------------------------------------------
// Name: RenameInterface::NotifyContextMenu
// Created: ABR 2014-09-03
// -----------------------------------------------------------------------------
void RenameInterface::NotifyContextMenu( const kernel::Ghost_ABC& ghost, kernel::ContextMenu& menu )
{
    AddCommonMenu( ghost, menu );
}

// -----------------------------------------------------------------------------
// Name: RenameInterface::NotifyContextMenu
// Created: ABR 2014-09-03
// -----------------------------------------------------------------------------
void RenameInterface::NotifyContextMenu( const kernel::Inhabitant_ABC& inhabitant, kernel::ContextMenu& menu )
{
    AddCommonMenu( inhabitant, menu );
}

// -----------------------------------------------------------------------------
// Name: RenameInterface::NotifyContextMenu
// Created: ABR 2014-09-03
// -----------------------------------------------------------------------------
void RenameInterface::NotifyContextMenu( const kernel::KnowledgeGroup_ABC& knowledgeGroup, kernel::ContextMenu& menu )
{
    AddCommonMenu( knowledgeGroup, menu );
}

// -----------------------------------------------------------------------------
// Name: RenameInterface::NotifyContextMenu
// Created: ABR 2014-09-03
// -----------------------------------------------------------------------------
void RenameInterface::NotifyContextMenu( const kernel::Object_ABC& objet, kernel::ContextMenu& menu )
{
    AddCommonMenu( objet, menu );
}

// -----------------------------------------------------------------------------
// Name: RenameInterface::NotifyContextMenu
// Created: ABR 2014-09-03
// -----------------------------------------------------------------------------
void RenameInterface::NotifyContextMenu( const kernel::Pathfind_ABC& pathfind, kernel::ContextMenu& menu )
{
    AddCommonMenu( pathfind, menu );
}

// -----------------------------------------------------------------------------
// Name: RenameInterface::NotifyContextMenu
// Created: ABR 2014-09-03
// -----------------------------------------------------------------------------
void RenameInterface::NotifyContextMenu( const kernel::Population_ABC& population, kernel::ContextMenu& menu )
{
    AddCommonMenu( population, menu );
}

// -----------------------------------------------------------------------------
// Name: RenameInterface::NotifyContextMenu
// Created: ABR 2014-09-03
// -----------------------------------------------------------------------------
void RenameInterface::NotifyContextMenu( const kernel::TacticalLine_ABC& line, kernel::ContextMenu& menu )
{
    AddCommonMenu( line, menu );
}

// -----------------------------------------------------------------------------
// Name: RenameInterface::NotifyContextMenu
// Created: ABR 2014-09-03
// -----------------------------------------------------------------------------
void RenameInterface::NotifyContextMenu( const kernel::Team_ABC& team, kernel::ContextMenu& menu )
{
    AddCommonMenu( team, menu );
}
