// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "TemplatesPanel.h"
#include "moc_TemplatesPanel.cpp"
#include "TemplateListView.h"
#include "clients_kernel/ContextMenu.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/ModelLoaded.h"
#include "tools/ExerciseConfig.h"

// -----------------------------------------------------------------------------
// Name: TemplatesPanel constructor
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
TemplatesPanel::TemplatesPanel( QWidget* parent,
                                gui::PanelStack_ABC& panel,
                                kernel::Controllers& controllers,
                                AgentsModel& agents,
                                FormationModel& formations,
                                GhostModel& ghosts,
                                const kernel::AgentTypes& types,
                                ColorController& colorController )
    : InfoPanel_ABC( parent, panel, tr( "Templates" ) )
    , controllers_ ( controllers )
    , menuEntity_  ( controllers )
{
    gui::SubObjectName subObject( "templatesPanel" );
    list_ = new TemplateListView( "templateListView", this, controllers, agents, formations, ghosts, types, colorController );
    setWidget( list_ );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: TemplatesPanel destructor
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
TemplatesPanel::~TemplatesPanel()
{
    try
    {
        if( ! templateFile_.IsEmpty() )
            list_->SaveTemplates( templateFile_ );
    }
    catch( ... )
    {
    }
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: TemplatesPanel::NotifyUpdated
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
void TemplatesPanel::NotifyUpdated( const kernel::ModelLoaded& model )
{
    Show();
    templateFile_ = model.config_.BuildPhysicalChildFile( "templates.xml" );
    list_->LoadTemplates( templateFile_ );
}

// -----------------------------------------------------------------------------
// Name: TemplatesPanel::NotifyUpdated
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
void TemplatesPanel::NotifyUpdated( const kernel::ModelUnLoaded& )
{
    list_->LoadTemplates( "" );
}

// -----------------------------------------------------------------------------
// Name: TemplatesPanel::NotifyContextMenu
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
void TemplatesPanel::NotifyContextMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu )
{
    if( entity.GetTypeName() == kernel::Formation_ABC::typeName_ ||
        entity.GetTypeName() == kernel::Automat_ABC::typeName_ ||
        entity.GetTypeName() == kernel::Agent_ABC::typeName_ )
    {
        menuEntity_ = &entity;
        menu.InsertItem( "Command", tr( "Create template" ), this, SLOT( OnCreateTemplate() ), false, 3 );
    }
}

// -----------------------------------------------------------------------------
// Name: TemplatesPanel::OnCreateTemplate
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
void TemplatesPanel::OnCreateTemplate()
{
    if( menuEntity_ )
        list_->CreateTemplate( *menuEntity_ );
    if( ! templateFile_.IsEmpty() )
        list_->SaveTemplates( templateFile_ );
}
