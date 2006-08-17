// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ReportFilterOptions.h"
#include "moc_ReportFilterOptions.cpp"

// -----------------------------------------------------------------------------
// Name: ReportFilterOptions constructor
// Created: AGE 2005-09-21
// -----------------------------------------------------------------------------
ReportFilterOptions::ReportFilterOptions( QWidget* pParent )
    : QVBox( pParent )
{
    QCheckBox* pDisplayRCs = new QCheckBox( tr( "Afficher les compte-rendus" ), this );
    QCheckBox* pDisplayTraces = new QCheckBox( tr( "Afficher les traces" ), this );
    QCheckBox* pDisplayEvents = new QCheckBox( tr( "Afficher les évènements" ), this );
    QCheckBox* pDisplayMessages = new QCheckBox( tr( "Afficher les messages" ), this );
    QCheckBox* pDisplayWarnings = new QCheckBox( tr( "Afficher les alertes" ), this );
    pDisplayRCs->setChecked( true ); pDisplayTraces->setChecked( true ); pDisplayEvents->setChecked( true ); pDisplayMessages->setChecked( true ); pDisplayWarnings->setChecked( true );
    toDisplay_.insert( Report_ABC::eRC ); toDisplay_.insert( Report_ABC::eTrace ); toDisplay_.insert( Report_ABC::eEvent ); toDisplay_.insert( Report_ABC::eMessage ); toDisplay_.insert( Report_ABC::eWarning );
    connect( pDisplayRCs,       SIGNAL( toggled( bool ) ), this, SLOT( OnToggleRCs( bool ) ) );
    connect( pDisplayTraces,    SIGNAL( toggled( bool ) ), this, SLOT( OnToggleTraces( bool ) ) );
    connect( pDisplayEvents,    SIGNAL( toggled( bool ) ), this, SLOT( OnToggleEvents( bool ) ) );
    connect( pDisplayMessages,  SIGNAL( toggled( bool ) ), this, SLOT( OnToggleMessages( bool ) ) );
    connect( pDisplayWarnings,  SIGNAL( toggled( bool ) ), this, SLOT( OnToggleWarnings( bool ) ) );
}

// -----------------------------------------------------------------------------
// Name: ReportFilterOptions destructor
// Created: AGE 2005-09-21
// -----------------------------------------------------------------------------
ReportFilterOptions::~ReportFilterOptions()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: ReportFilterOptions::ShouldDisplay
// Created: AGE 2005-09-21
// -----------------------------------------------------------------------------
bool ReportFilterOptions::ShouldDisplay( const Report_ABC& report ) const
{
    return toDisplay_.find( report.GetType() ) != toDisplay_.end();
}

// -----------------------------------------------------------------------------
// Name: ReportFilterOptions::Toggle
// Created: AGE 2005-09-21
// -----------------------------------------------------------------------------
void ReportFilterOptions::Toggle( bool bOn, Report_ABC::E_Type type )
{
    if( bOn )
        toDisplay_.insert( type );
    else
        toDisplay_.erase( type );
    emit OptionsChanged();
}

// -----------------------------------------------------------------------------
// Name: ReportFilterOptions::OnToggleRCs
// Created: AGE 2005-09-21
// -----------------------------------------------------------------------------
void ReportFilterOptions::OnToggleRCs( bool bOn )
{
    Toggle( bOn, Report_ABC::eRC );
}

// -----------------------------------------------------------------------------
// Name: ReportFilterOptions::OnToggleTraces
// Created: AGE 2005-09-21
// -----------------------------------------------------------------------------
void ReportFilterOptions::OnToggleTraces( bool bOn )
{
    Toggle( bOn, Report_ABC::eTrace );
}

// -----------------------------------------------------------------------------
// Name: ReportFilterOptions::OnToggleEvents
// Created: AGE 2005-09-21
// -----------------------------------------------------------------------------
void ReportFilterOptions::OnToggleEvents( bool bOn )
{
    Toggle( bOn, Report_ABC::eEvent );
}

// -----------------------------------------------------------------------------
// Name: ReportFilterOptions::OnToggleMessages
// Created: AGE 2005-09-21
// -----------------------------------------------------------------------------
void ReportFilterOptions::OnToggleMessages( bool bOn )
{
    Toggle( bOn, Report_ABC::eMessage );
}

// -----------------------------------------------------------------------------
// Name: ReportFilterOptions::OnToggleWarnings
// Created: AGE 2005-09-21
// -----------------------------------------------------------------------------
void ReportFilterOptions::OnToggleWarnings( bool bOn )
{
    Toggle( bOn, Report_ABC::eWarning );
}
