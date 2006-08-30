// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "ModelBuilder.h"
#include "moc_ModelBuilder.cpp"
#include "Model.h"
#include "TeamsModel.h"
#include "clients_kernel/Controllers.h"
#include "clients_gui/Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ModelBuilder constructor
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
ModelBuilder::ModelBuilder( Controllers& controllers, Model& model )
    : controllers_( controllers )
    , model_( model )
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
// Name: ModelBuilder::NotifyContextMenu
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
void ModelBuilder::NotifyContextMenu( const Model&, ContextMenu& menu )
{
    currentTeam_ = 0;
    menu.InsertItem( "Creation", tools::translate( "Preparation", "Créer une armée" ), this, SLOT( OnCreateTeam() ) );
}
    
// -----------------------------------------------------------------------------
// Name: ModelBuilder::NotifyContextMenu
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
void ModelBuilder::NotifyContextMenu( const kernel::Team_ABC& team, ContextMenu& menu )
{
    currentTeam_ = &team;
    menu.InsertItem( "Creation", tools::translate( "Preparation", "Créer un groupe de connaissance" ), this, SLOT( OnCreateKnowledgeGroup() ) );
}
    
// -----------------------------------------------------------------------------
// Name: ModelBuilder::NotifyContextMenu
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
void ModelBuilder::NotifyContextMenu( const kernel::KnowledgeGroup_ABC& group, ContextMenu& menu )
{
    currentTeam_ = 0;
}
    
// -----------------------------------------------------------------------------
// Name: ModelBuilder::NotifyContextMenu
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
void ModelBuilder::NotifyContextMenu( const kernel::Agent_ABC& agent, ContextMenu& menu )
{
    currentTeam_ = 0;
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::OnCreateTeam
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
void ModelBuilder::OnCreateTeam()
{
    model_.teams_.CreateTeam();
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::OnCreateKnowledgeGroup
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
void ModelBuilder::OnCreateKnowledgeGroup()
{
    if( !currentTeam_ )
        return;
    model_.teams_.CreateKnowledgeGroup( const_cast< Team_ABC& >( *currentTeam_ ) );
}
