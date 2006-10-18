// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "AgentStatePanel.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_gui/DisplayBuilder.h"
#include "clients_gui/GroupDisplayer.h"
#include "clients_gui/LabelDisplayer.h"
#include "clients_gui/Tools.h"
#include "clients_gui/PropertiesTable.h"
#include "clients_kernel/PropertiesDictionary.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: AgentStatePanel constructor
// Created: SBO 2006-10-11
// -----------------------------------------------------------------------------
AgentStatePanel::AgentStatePanel( QWidget* parent, PanelStack_ABC& panel, Controllers& controllers, ItemFactory_ABC& factory )
    : InfoPanel_ABC( parent, panel, tools::translate( "Preparation", "Agent State" ) )
    , controllers_( controllers )
    , selected_( controllers )
{
    properties_ = new PropertiesTable( this );


//    display_ = new DisplayBuilder( this, factory );
//    display_->AddGroup( tr( "Info" ) )
//                .AddLabel( tr( "Nom:" ), true );
//
//    display_->AddGroup( tr( "Facteurs humains" ) )
//                .AddLabel( tr( "Experience:" ) )
//                .AddLabel( tr( "Moral:" ) )
//                .AddLabel( tr( "Fatigue:" ) );
//
//    display_->AddGroup( tr( "Liens logistiques" ) )
//                .AddLabel( tr( "TC2:" ) )
//                .AddLabel( tr( "Supérieur maintenance:" ) )
//                .AddLabel( tr( "Supérieur santé:" ) )
//                .AddLabel( tr( "Supérieur ravitaillement:" ) );

    controllers_.Register( *this );
}
    
// -----------------------------------------------------------------------------
// Name: AgentStatePanel destructor
// Created: SBO 2006-10-11
// -----------------------------------------------------------------------------
AgentStatePanel::~AgentStatePanel()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentStatePanel::showEvent
// Created: SBO 2006-10-11
// -----------------------------------------------------------------------------
void AgentStatePanel::showEvent( QShowEvent* )
{
    const Agent_ABC* selected = selected_;
    selected_ = 0;
    NotifySelected( selected );
}

// -----------------------------------------------------------------------------
// Name: AgentStatePanel::NotifySelected
// Created: SBO 2006-10-11
// -----------------------------------------------------------------------------
void AgentStatePanel::NotifySelected( const kernel::Agent_ABC* element )
{
    if( selected_ != element || ! element )
    {
        selected_ = element;
        if( selected_ )
        {
            Show();
            PropertiesDictionary& dico = const_cast< kernel::Agent_ABC* >( element )->Get< PropertiesDictionary >();
            dico.Display( *properties_ );
//            display_->Group( "Info" ) .Display( "Nom:", selected_ );
        }
        else
            Hide();
    }
}

// $$$$ AGE 2006-02-16: Centraliser tout ca : 
// -----------------------------------------------------------------------------
// Name: AgentStatePanel::NotifyDeleted
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void AgentStatePanel::NotifyDeleted( const Agent_ABC& agent )
{
    if( selected_ = & agent )
        NotifySelected( 0 );
}