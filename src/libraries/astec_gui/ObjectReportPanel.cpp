// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gui_pch.h"
#include "ObjectReportPanel.h"
#include "astec_gaming/Controllers.h"
#include "FireResultListView.h"

#include "astec_gaming/Object_ABC.h"
#include "astec_gaming/Explosions.h"
#include "astec_gaming/Equipment.h"

// -----------------------------------------------------------------------------
// Name: ObjectReportPanel constructor
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
ObjectReportPanel::ObjectReportPanel( InfoPanels* pParent, Controllers& controllers, ItemFactory_ABC& factory )
    : InfoPanel_ABC( pParent, "Rapports" )
    , controllers_ ( controllers )
    , selected_    ( controllers )
{
    reports_ = new FireResultListView( this, factory );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectReportPanel destructor
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
ObjectReportPanel::~ObjectReportPanel()
{
    controllers_.Remove( *this );
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
