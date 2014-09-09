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
#include "clients_kernel/tools.h"
#include "gaming/Explosions.h"
#include "gaming/Equipment.h"

// -----------------------------------------------------------------------------
// Name: ObjectReportPanel constructor
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
ObjectReportPanel::ObjectReportPanel( QWidget* parent, gui::PanelStack_ABC& panel,
    kernel::Controllers& controllers, gui::DisplayExtractor& extractor,
    const kernel::KnowledgeConverter_ABC& converter )
    : InfoPanel_ABC( parent, panel, tools::translate( "ObjectReportPanel", "Reports" ) )
    , controllers_ ( controllers )
    , selected_    ( controllers )
{
    setWidget( new FireResultListView( this, controllers, extractor, converter ) );
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
// Name: ObjectReportPanel::NotifyDeleted
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void ObjectReportPanel::NotifyDeleted( const kernel::Object_ABC& object )
{
    if( &object == selected_ )
        NotifySelected( 0 );
}

// -----------------------------------------------------------------------------
// Name: ObjectReportPanel::NotifySelected
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void ObjectReportPanel::NotifySelected( const kernel::Object_ABC* object )
{
    // $$$$ _RC_ JSR 2012-10-24: Useless??
    if( object != selected_ || !object )
    {
        selected_ = object;
        if( selected_ )
            Show();
        else
            Hide();
    }
}
