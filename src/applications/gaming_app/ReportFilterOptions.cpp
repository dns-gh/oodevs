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
#include "clients_kernel/ContextMenu.h"

// -----------------------------------------------------------------------------
// Name: ReportFilterOptions constructor
// Created: AGE 2005-09-21
// -----------------------------------------------------------------------------
ReportFilterOptions::ReportFilterOptions( QWidget* pParent )
    : Q3VBox( pParent )
{
    QCheckBox* pDisplayRCs = new QCheckBox( tr( "Display reports" ), this );
    QCheckBox* pDisplayTraces = new QCheckBox( tr( "Display traces" ), this );
    QCheckBox* pDisplayEvents = new QCheckBox( tr( "Display events" ), this );
    QCheckBox* pDisplayMessages = new QCheckBox( tr( "Display messages" ), this );
    QCheckBox* pDisplayWarnings = new QCheckBox( tr( "Display warnings" ), this );
    pDisplayRCs->setChecked( true ); pDisplayTraces->setChecked( true ); pDisplayEvents->setChecked( true ); pDisplayMessages->setChecked( true ); pDisplayWarnings->setChecked( true );
    toDisplay_.insert( Report::eRC ); toDisplay_.insert( Report::eTrace ); toDisplay_.insert( Report::eEvent ); toDisplay_.insert( Report::eMessage ); toDisplay_.insert( Report::eWarning );
    connect( pDisplayRCs,       SIGNAL( toggled( bool ) ), this, SLOT( OnToggleRCs() ) );
    connect( pDisplayTraces,    SIGNAL( toggled( bool ) ), this, SLOT( OnToggleTraces() ) );
    connect( pDisplayEvents,    SIGNAL( toggled( bool ) ), this, SLOT( OnToggleEvents() ) );
    connect( pDisplayMessages,  SIGNAL( toggled( bool ) ), this, SLOT( OnToggleMessages() ) );
    connect( pDisplayWarnings,  SIGNAL( toggled( bool ) ), this, SLOT( OnToggleWarnings() ) );
    Toggle( Report::eTrace );
}

// -----------------------------------------------------------------------------
// Name: ReportFilterOptions destructor
// Created: AGE 2005-09-21
// -----------------------------------------------------------------------------
ReportFilterOptions::~ReportFilterOptions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ReportFilterOptions::ShouldDisplay
// Created: AGE 2005-09-21
// -----------------------------------------------------------------------------
bool ReportFilterOptions::ShouldDisplay( const Report& report ) const
{
    return toDisplay_.find( report.GetType() ) != toDisplay_.end();
}

// -----------------------------------------------------------------------------
// Name: ReportFilterOptions::Toggle
// Created: AGE 2005-09-21
// -----------------------------------------------------------------------------
void ReportFilterOptions::Toggle( Report::E_Type type )
{
    if( toDisplay_.find( type ) == toDisplay_.end() )
        toDisplay_.insert( type );
    else
        toDisplay_.erase( type );
    emit OptionsChanged();
}

// -----------------------------------------------------------------------------
// Name: ReportFilterOptions::OnToggleRCs
// Created: AGE 2005-09-21
// -----------------------------------------------------------------------------
void ReportFilterOptions::OnToggleRCs()
{
    Toggle( Report::eRC );
}

// -----------------------------------------------------------------------------
// Name: ReportFilterOptions::OnToggleTraces
// Created: AGE 2005-09-21
// -----------------------------------------------------------------------------
void ReportFilterOptions::OnToggleTraces()
{
    Toggle( Report::eTrace );
}

// -----------------------------------------------------------------------------
// Name: ReportFilterOptions::OnToggleEvents
// Created: AGE 2005-09-21
// -----------------------------------------------------------------------------
void ReportFilterOptions::OnToggleEvents()
{
    Toggle( Report::eEvent );
}

// -----------------------------------------------------------------------------
// Name: ReportFilterOptions::OnToggleMessages
// Created: AGE 2005-09-21
// -----------------------------------------------------------------------------
void ReportFilterOptions::OnToggleMessages()
{
    Toggle( Report::eMessage );
}

// -----------------------------------------------------------------------------
// Name: ReportFilterOptions::OnToggleWarnings
// Created: AGE 2005-09-21
// -----------------------------------------------------------------------------
void ReportFilterOptions::OnToggleWarnings()
{
    Toggle( Report::eWarning );
}

// -----------------------------------------------------------------------------
// Name: ReportFilterOptions::AddMenuItem
// Created: SBO 2007-02-06
// -----------------------------------------------------------------------------
void ReportFilterOptions::AddMenuItem( kernel::ContextMenu* menu, const QString& name, Report::E_Type type, const char* slot ) const
{
    int id = menu->insertItem( name );
    menu->setItemChecked( id, toDisplay_.find( type ) != toDisplay_.end() );
    menu->connectItem( id, this, slot );
}

// -----------------------------------------------------------------------------
// Name: ReportFilterOptions::AddContextMenu
// Created: SBO 2007-02-06
// -----------------------------------------------------------------------------
void ReportFilterOptions::AddContextMenu( kernel::ContextMenu* menu ) const
{
    menu->insertSeparator();
    AddMenuItem( menu, tr( "Show reports" ) , Report::eRC      , SLOT( OnToggleRCs     () ) );
    AddMenuItem( menu, tr( "Show traces" )  , Report::eTrace   , SLOT( OnToggleTraces  () ) );
    AddMenuItem( menu, tr( "Show events" )  , Report::eEvent   , SLOT( OnToggleEvents  () ) );
    AddMenuItem( menu, tr( "Show messages" ), Report::eMessage , SLOT( OnToggleMessages() ) );
    AddMenuItem( menu, tr( "Show warnings" ), Report::eWarning , SLOT( OnToggleWarnings() ) );
}
