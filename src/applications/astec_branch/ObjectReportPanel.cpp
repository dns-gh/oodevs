// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-09-08 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ObjectReportPanel.cpp $
// $Author: Age $
// $Modtime: 5/04/05 17:12 $
// $Revision: 7 $
// $Workfile: ObjectReportPanel.cpp $
//
// *****************************************************************************

#include "astec_pch.h"
#include "ObjectReportPanel.h"
#include "Controller.h"
#include "ActionController.h"
#include "FireResultListView.h"

#include "Object_ABC.h"
#include "Explosions.h"
#include "Equipment.h"

// -----------------------------------------------------------------------------
// Name: ObjectReportPanel constructor
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
ObjectReportPanel::ObjectReportPanel( InfoPanels* pParent, Controller& controller, ActionController& actionController )
    : InfoPanel_ABC( pParent, "Rapports" )
    , selected_( 0 )
{
    reports_ = new FireResultListView( this );

    controller.Register( *this );
    actionController.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectReportPanel destructor
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
ObjectReportPanel::~ObjectReportPanel()
{
    // NOTHING
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
            NotifyUpdated( selected_->Get< Explosions >() );
        }
        else
            Hide();
    }
}
