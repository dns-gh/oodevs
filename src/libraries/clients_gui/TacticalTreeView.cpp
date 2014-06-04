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
#include "moc_TacticalTreeView.cpp"
#include "ChangeSuperiorDialog.h"
#include "ItemPixmapDelegate.h"
#include "resources.h"
#include "Tools.h"

#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Ghost_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include <boost/bind.hpp>

using namespace gui;

namespace
{
    bool CanChangeSuperior( const kernel::Entity_ABC& entity, const kernel::Entity_ABC& superior )
    {
        if( !superior.Retrieve< kernel::TacticalHierarchies >() )
            return false;
        if( entity.GetTypeName() == kernel::Agent_ABC::typeName_ )
            return superior.GetTypeName() == kernel::Automat_ABC::typeName_;
        if( entity.GetTypeName() == kernel::Automat_ABC::typeName_ )
            return superior.GetTypeName() == kernel::Formation_ABC::typeName_;
        if( entity.GetTypeName() == kernel::Formation_ABC::typeName_ )
            return superior.GetTypeName() == kernel::Formation_ABC::typeName_ && entity.GetId() != superior.GetId() ||
                   superior.GetTypeName() == kernel::Team_ABC::typeName_;
        if( const kernel::Ghost_ABC* ghost = dynamic_cast< const kernel::Ghost_ABC* >( &entity ) )
            return ghost->GetGhostType() == eGhostType_Automat && superior.GetTypeName() == kernel::Formation_ABC::typeName_ ||
                   ghost->GetGhostType() == eGhostType_Agent && superior.GetTypeName() == kernel::Automat_ABC::typeName_;
        if( entity.GetTypeName() == kernel::KnowledgeGroup_ABC::typeName_ )
            return superior.GetTypeName() == kernel::Formation_ABC::typeName_;
        return false;
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView constructor
// Created: JSR 2012-08-31
// -----------------------------------------------------------------------------
TacticalTreeView::TacticalTreeView( const QString& objectName,
                                    kernel::Controllers& controllers,
                                    const kernel::Profile_ABC& profile,
                                    ModelObserver_ABC& modelObserver,
                                    const EntitySymbols& symbols,
                                    gui::ChangeSuperiorDialog& changeSuperiorDialog,
                                    QWidget* parent /*= 0*/ )
    : HierarchyTreeView< kernel::TacticalHierarchies >( objectName, controllers, profile, modelObserver, symbols, parent )
    , commandPost_( MAKE_PIXMAP( commandpost ) )
    , lock_( MAKE_PIXMAP( lock ) )
    , changeSuperiorDialog_( changeSuperiorDialog )
    , contextMenuEntity_( controllers )
{
    connect( this,                   SIGNAL( SelectionChanged( const std::vector< const kernel::Entity_ABC* >& ) ),
             &changeSuperiorDialog_, SLOT( OnTacticalSelectionChanged( const std::vector< const kernel::Entity_ABC* >& ) ) );
    changeSuperiorDialog_.SetTacticalSuperiorFunctors(
        [&] ( const kernel::Entity_ABC& entity, const kernel::Entity_ABC& superior ) -> bool {
            if( !profile.IsVisible( entity ) )
                return false;
            return CanChangeSuperior( entity, superior );
        },
        [&] ( kernel::Entity_ABC& entity, const kernel::Entity_ABC& superior ) {
            if( auto agent = dynamic_cast< kernel::Agent_ABC* >( &entity ) )
                Drop( *agent, superior );
            else if( auto automat = dynamic_cast< kernel::Automat_ABC* >( &entity ) )
                Drop( *automat, superior );
            else if( auto formation = dynamic_cast< kernel::Formation_ABC* >( &entity ) )
                Drop( *formation, superior );
            else if( auto ghost = dynamic_cast< kernel::Ghost_ABC* >( &entity ) )
                Drop( *ghost, superior );
            else if( auto kg = dynamic_cast< kernel::KnowledgeGroup_ABC* >( &entity ) )
                Drop( *kg, superior );
        } );

    SetLessThanEntityFunctor( &tools::LessThanByPC );
    setItemDelegate( new ItemPixmapDelegate( dataModel_, boost::bind( &TacticalTreeView::GetEntityPixmap, this, _1 ), this ) );
    controllers_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView destructor
// Created: JSR 2012-08-31
// -----------------------------------------------------------------------------
TacticalTreeView::~TacticalTreeView()
{
    controllers_.Unregister( *this );
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
    if( entity.GetTypeName() == kernel::Automat_ABC::typeName_ && tools::IsEngaged( entity ) )
        ret.push_back( &lock_ );
    return ret;
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::AddCommonMenu
// Created: ABR 2014-06-02
// -----------------------------------------------------------------------------
bool TacticalTreeView::AddCommonMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu )
{
    if( !isVisible() || !profile_.IsVisible( entity ) )
        return false;
    contextMenuEntity_ = &entity;
    if( const kernel::TacticalHierarchies* hierarchies = entity.Retrieve< kernel::TacticalHierarchies >() )
        if( hierarchies->GetSuperior() != 0 &&
            entity.GetTypeName() != kernel::Object_ABC::typeName_ &&
            controllers_.GetCurrentMode() != eModes_Replay )
            menu.InsertItem( "Command", tr( "Change superior" ), this, SLOT( OnChangeSuperior() ), false, 1 );
    return true;
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::NotifyContextMenu
// Created: ABR 2014-06-02
// -----------------------------------------------------------------------------
void TacticalTreeView::NotifyContextMenu( const kernel::Agent_ABC& entity, kernel::ContextMenu& menu )
{
    AddCommonMenu( entity, menu );
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::NotifyContextMenu
// Created: ABR 2014-06-02
// -----------------------------------------------------------------------------
void TacticalTreeView::NotifyContextMenu( const kernel::Automat_ABC& entity, kernel::ContextMenu& menu )
{
    AddCommonMenu( entity, menu );
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::NotifyContextMenu
// Created: ABR 2014-06-02
// -----------------------------------------------------------------------------
void TacticalTreeView::NotifyContextMenu( const kernel::Formation_ABC& entity, kernel::ContextMenu& menu )
{
    AddCommonMenu( entity, menu );
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::OnChangeSuperior
// Created: ABR 2014-06-02
// -----------------------------------------------------------------------------
void TacticalTreeView::OnChangeSuperior()
{
    if( contextMenuEntity_ )
        changeSuperiorDialog_.Show( *contextMenuEntity_.ConstCast(),
                                    tr( "Change superior" ),
                                    contextMenuEntity_->GetTypeName() == kernel::Automat_ABC::typeName_
                                        ? gui::ChangeSuperiorDialog::eTacticalSuperior |
                                          gui::ChangeSuperiorDialog::eOptionalKnowledgeGroup |
                                          gui::ChangeSuperiorDialog::eOptionalLogisticSuperior
                                        : gui::ChangeSuperiorDialog::eTacticalSuperior );
}
