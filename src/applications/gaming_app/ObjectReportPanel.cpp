// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ObjectReportPanel.h"

#include "FireResultListView.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Object_ABC.h"
#include "gaming/Explosions.h"
#include "gaming/Equipment.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ObjectReportPanel constructor
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
ObjectReportPanel::ObjectReportPanel( QWidget* parent, PanelStack_ABC& panel, Controllers& controllers, ItemFactory_ABC& factory )
    : InfoPanel_ABC( parent, panel, tr( "Reports" ) )
    , controllers_ ( controllers )
    , selected_    ( controllers )
{
    reports_ = new FireResultListView( this, controllers, factory );
    setWidget( reports_ );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectReportPanel destructor
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
ObjectReportPanel::~ObjectReportPanel()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectReportPanel::NotifyUpdated
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void ObjectReportPanel::NotifyUpdated( const Explosions& explosions )
{
    if( ! IsVisible() || ! selected_ || selected_->Retrieve< Explosions >() != &explosions )
        return;

    // $$$$ AGE 2006-03-10: try to keep the selection if possible
    ValuedListItem* item = reports_->DisplayList( explosions.agentExplosions_.begin(), explosions.agentExplosions_.end() );
                    item = reports_->DisplayList( explosions.populationExplosions_.begin(), explosions.populationExplosions_.end(), reports_, item );
    reports_->DeleteTail( item );
}

// -----------------------------------------------------------------------------
// Name: ObjectReportPanel::NotifyDeleted
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void ObjectReportPanel::NotifyDeleted( const Object_ABC& object )
{
    if( & object == selected_ )
        NotifySelected( 0 );
}

// -----------------------------------------------------------------------------
// Name: ObjectReportPanel::NotifySelected
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void ObjectReportPanel::NotifySelected( const Object_ABC* object )
{
    if( object != selected_ || !object )
    {
        selected_ = object;
        if( selected_ )
        {
            Show();
            const Explosions* explosions = selected_->Retrieve< Explosions >();
            if( explosions )
                NotifyUpdated( *explosions  );
        }
        else
            Hide();
    }
}
