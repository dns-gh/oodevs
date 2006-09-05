// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ModelBuilder.h"
#include "moc_ModelBuilder.cpp"
#include "preparation/Model.h"
#include "preparation/TeamsModel.h"
#include "preparation/AgentsModel.h"
#include "clients_kernel/Controllers.h"
#include "clients_gui/Tools.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/Team_ABC.h"

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
void ModelBuilder::NotifyContextMenu( const Model&, ContextMenu& menu )
{
    menu.InsertItem( "Ordre", tools::translate( "Preparation", "Créer une armée" ), this, SLOT( OnCreate() ) );
}
    
// -----------------------------------------------------------------------------
// Name: ModelBuilder::NotifyContextMenu
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
void ModelBuilder::NotifyContextMenu( const kernel::Team_ABC&, ContextMenu& menu )
{
    menu.InsertItem( "Ordre", tools::translate( "Preparation", "Créer un groupe de connaissance" ), this, SLOT( OnCreate() ) );
    InsertDefaultMenu( menu );
}
    
// -----------------------------------------------------------------------------
// Name: ModelBuilder::NotifyContextMenu
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
void ModelBuilder::NotifyContextMenu( const kernel::KnowledgeGroup_ABC&, ContextMenu& menu )
{
    InsertDefaultMenu( menu );
}
    
// -----------------------------------------------------------------------------
// Name: ModelBuilder::NotifyContextMenu
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
void ModelBuilder::NotifyContextMenu( const kernel::Agent_ABC&, ContextMenu& menu )
{
    InsertDefaultMenu( menu );
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::OnCreate
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
void ModelBuilder::OnCreate()
{
    if( selectedTeam_ )
        model_.teams_.CreateKnowledgeGroup( *selectedTeam_ );
    else
        model_.teams_.CreateTeam();
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::OnDelete
// Created: SBO 2006-09-04
// -----------------------------------------------------------------------------
void ModelBuilder::OnDelete()
{
    if( selectedAgent_ )
        delete (const Agent_ABC*)selectedAgent_;
    else if( selectedGroup_ )
        delete (const KnowledgeGroup_ABC*)selectedGroup_;
    else if( selectedTeam_ )
        delete (const Team_ABC*)selectedTeam_;
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::BeforeSelection
// Created: SBO 2006-09-04
// -----------------------------------------------------------------------------
void ModelBuilder::BeforeSelection()
{
    selectedTeam_  = 0;
    selectedGroup_ = 0;
    selectedAgent_ = 0;
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
void ModelBuilder::Select( const kernel::Team_ABC& element )
{
    selectedTeam_ = &element;
}
    
// -----------------------------------------------------------------------------
// Name: ModelBuilder::Select
// Created: SBO 2006-09-04
// -----------------------------------------------------------------------------
void ModelBuilder::Select( const kernel::KnowledgeGroup_ABC& element )
{
    selectedGroup_ = &element;
}
    
// -----------------------------------------------------------------------------
// Name: ModelBuilder::Select
// Created: SBO 2006-09-04
// -----------------------------------------------------------------------------
void ModelBuilder::Select( const kernel::Agent_ABC& element )
{
    selectedAgent_ = &element;
}
