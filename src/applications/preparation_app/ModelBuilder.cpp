// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ModelBuilder.h"
#include "moc_ModelBuilder.cpp"
#include "preparation/Model.h"
#include "preparation/TeamsModel.h"
#include "preparation/AgentsModel.h"
#include "preparation/FormationModel.h"
#include "preparation/FormationLevels.h"
#include "preparation/Level.h"
#include "clients_kernel/Controllers.h"
#include "clients_gui/Tools.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Formation_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ModelBuilder constructor
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
ModelBuilder::ModelBuilder( Controllers& controllers, Model& model )
    : controllers_( controllers )
    , model_( model )
    , selectedTeam_( controllers )
    , selectedGroup_( controllers )
    , selectedAgent_( controllers )
    , selectedFormation_( controllers )
{
    controllers_.Register( *this );
}
    
// -----------------------------------------------------------------------------
// Name: ModelBuilder destructor
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
ModelBuilder::~ModelBuilder()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::InsertDefaultMenu
// Created: SBO 2006-09-04
// -----------------------------------------------------------------------------
void ModelBuilder::InsertDefaultMenu( kernel::ContextMenu& menu ) const
{
    menu.InsertItem( "Commande", tools::translate( "Preparation", "Supprimer" ), this, SLOT( OnDelete() ) );
}
    
// -----------------------------------------------------------------------------
// Name: ModelBuilder::NotifyContextMenu
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
void ModelBuilder::NotifyContextMenu( const Team_ABC&, ContextMenu& menu )
{
    QPopupMenu* subMenu = menu.SubMenu( "Commande", tools::translate( "Preparation", "Cr�er une formation" ) );
    const Level* level = model_.formations_.levels_.GetRoot();
    while( level && ( level = level->GetNext() ) )
        subMenu->insertItem( level->GetName(), this, SLOT( OnCreateFormation( int ) ), 0, level->GetId() );
    InsertDefaultMenu( menu );
}
    
// -----------------------------------------------------------------------------
// Name: ModelBuilder::NotifyContextMenu
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
void ModelBuilder::NotifyContextMenu( const KnowledgeGroup_ABC&, ContextMenu& menu )
{
    InsertDefaultMenu( menu );
}
    
// -----------------------------------------------------------------------------
// Name: ModelBuilder::NotifyContextMenu
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
void ModelBuilder::NotifyContextMenu( const Agent_ABC&, ContextMenu& menu )
{
    InsertDefaultMenu( menu );
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::NotifyContextMenu
// Created: SBO 2006-09-19
// -----------------------------------------------------------------------------
void ModelBuilder::NotifyContextMenu( const Formation_ABC&, ContextMenu& menu )
{
    const Level* level = model_.formations_.levels_.Resolve( selectedFormation_->GetLevel() );
    if( level && level->GetNext() )
    {
        QPopupMenu* subMenu = menu.SubMenu( "Ordre", tools::translate( "Preparation", "Cr�er une formation" ) );
        while( level && ( level = level->GetNext() ) )
            subMenu->insertItem( level->GetName(), this, SLOT( OnCreateFormation( int ) ), 0, level->GetId() );
    }
    InsertDefaultMenu( menu );
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::OnCreateFormation
// Created: SBO 2006-09-21
// -----------------------------------------------------------------------------
void ModelBuilder::OnCreateFormation( int levelId )
{
    if( selectedTeam_ )
        model_.formations_.Create( *selectedTeam_.ConstCast(), levelId );
    else if( selectedFormation_ )
        model_.formations_.Create( *selectedFormation_.ConstCast(), levelId );
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::OnCreate
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
void ModelBuilder::OnCreate()
{
    model_.teams_.CreateTeam();
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::OnDelete
// Created: SBO 2006-09-04
// -----------------------------------------------------------------------------
bool ModelBuilder::OnDelete()
{
    if( selectedAgent_ )
        delete (const Agent_ABC*)selectedAgent_;
    else if( selectedGroup_ )
        delete (const KnowledgeGroup_ABC*)selectedGroup_;
    else if( selectedTeam_ )
        delete (const Team_ABC*)selectedTeam_;
    else if( selectedFormation_ )
        delete (const Formation_ABC*)selectedFormation_;
    else
        return false;
    return true;
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::ClearSelection
// Created: SBO 2006-09-21
// -----------------------------------------------------------------------------
void ModelBuilder::ClearSelection()
{
    selectedTeam_ = 0;
    selectedGroup_ = 0;
    selectedAgent_ = 0;
    selectedFormation_ = 0;
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::BeforeSelection
// Created: SBO 2006-09-04
// -----------------------------------------------------------------------------
void ModelBuilder::BeforeSelection()
{
    ClearSelection();
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::AfterSelection
// Created: SBO 2006-09-04
// -----------------------------------------------------------------------------
void ModelBuilder::AfterSelection()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::Select
// Created: SBO 2006-09-04
// -----------------------------------------------------------------------------
void ModelBuilder::Select( const Team_ABC& element )
{
    selectedTeam_ = &element;
}
    
// -----------------------------------------------------------------------------
// Name: ModelBuilder::Select
// Created: SBO 2006-09-04
// -----------------------------------------------------------------------------
void ModelBuilder::Select( const KnowledgeGroup_ABC& element )
{
    selectedGroup_ = &element;
}
    
// -----------------------------------------------------------------------------
// Name: ModelBuilder::Select
// Created: SBO 2006-09-04
// -----------------------------------------------------------------------------
void ModelBuilder::Select( const Agent_ABC& element )
{
    selectedAgent_ = &element;
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::Select
// Created: SBO 2006-09-19
// -----------------------------------------------------------------------------
void ModelBuilder::Select( const Formation_ABC& element )
{
    selectedFormation_ = &element;
}
