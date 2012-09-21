// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "LogisticTreeView.h"
#include "preparation/LogisticBaseStates.h"
#include "preparation/LogisticLevelAttritube.h"
#include "clients_kernel/StrongType.h"

// -----------------------------------------------------------------------------
// Name: LogisticTreeView constructor
// Created: ABR 2012-09-19
// -----------------------------------------------------------------------------
LogisticTreeView::LogisticTreeView( kernel::Controllers& controllers, const kernel::Profile_ABC& profile, gui::ModelObserver_ABC& modelObserver, const gui::EntitySymbols& symbols, QWidget* parent /*= 0*/ )
    : gui::LogisticTreeView( controllers, profile, modelObserver, symbols, parent )
{
    controllers_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticTreeView destructor
// Created: ABR 2012-09-19
// -----------------------------------------------------------------------------
LogisticTreeView::~LogisticTreeView()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticTreeView::SetSuperior
// Created: ABR 2012-09-19
// -----------------------------------------------------------------------------
void LogisticTreeView::SetSuperior( const kernel::Entity_ABC& entity, const kernel::Entity_ABC* superior )
{
    LogisticHierarchiesBase* hierarchy = const_cast< kernel::Entity_ABC& >( entity ).Retrieve< LogisticHierarchiesBase >();
    assert( hierarchy );
    hierarchy->SetLogisticSuperior( ( superior ) ? superior : kernel::LogisticBaseSuperior() );
}

// -----------------------------------------------------------------------------
// Name: LogisticTreeView::RetrieveSuperior
// Created: ABR 2012-09-19
// -----------------------------------------------------------------------------
const kernel::Entity_ABC* LogisticTreeView::RetrieveSuperior( const kernel::Entity_ABC& entity ) const
{
    const LogisticBaseStates* hierarchy = static_cast< const LogisticBaseStates* >( entity.Retrieve< LogisticHierarchiesBase >() );
    return ( hierarchy ) ? hierarchy->GetSuperior() : 0;
}

// -----------------------------------------------------------------------------
// Name: LogisticTreeView::NotifyUpdated
// Created: ABR 2012-09-19
// -----------------------------------------------------------------------------
void LogisticTreeView::NotifyUpdated( const LogisticBaseStates& hierarchy )
{
    CreateOrReplace( hierarchy.GetEntity() );
}

// -----------------------------------------------------------------------------
// Name: LogisticTreeView::NotifyUpdated
// Created: ABR 2012-09-19
// -----------------------------------------------------------------------------
void LogisticTreeView::NotifyUpdated( const LogisticLevelAttritube& hierarchy )
{
    CreateOrReplace( hierarchy.GetEntity() );
}
