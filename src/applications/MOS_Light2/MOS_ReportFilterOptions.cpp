// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "MOS_Light2_pch.h"
#include "MOS_ReportFilterOptions.h"
#include "moc_MOS_ReportFilterOptions.cpp"

// -----------------------------------------------------------------------------
// Name: MOS_ReportFilterOptions constructor
// Created: AGE 2005-09-21
// -----------------------------------------------------------------------------
MOS_ReportFilterOptions::MOS_ReportFilterOptions( QWidget* pParent )
    : QVBox( pParent )
{
    QCheckBox* pDisplayRCs = new QCheckBox( tr( "Afficher les compte-rendus" ), this );
    QCheckBox* pDisplayTraces = new QCheckBox( tr( "Afficher les traces" ), this );
    QCheckBox* pDisplayEvents = new QCheckBox( tr( "Afficher les évènements" ), this );
    QCheckBox* pDisplayMessages = new QCheckBox( tr( "Afficher les messages" ), this );
    QCheckBox* pDisplayWarnings = new QCheckBox( tr( "Afficher les alertes" ), this );
    pDisplayRCs->setChecked( true ); pDisplayTraces->setChecked( true ); pDisplayEvents->setChecked( true ); pDisplayMessages->setChecked( true ); pDisplayWarnings->setChecked( true );
    toDisplay_.insert( MOS_Report_ABC::eRC ); toDisplay_.insert( MOS_Report_ABC::eTrace ); toDisplay_.insert( MOS_Report_ABC::eEvent ); toDisplay_.insert( MOS_Report_ABC::eMessage ); toDisplay_.insert( MOS_Report_ABC::eWarning );
    connect( pDisplayRCs,       SIGNAL( toggled( bool ) ), this, SLOT( OnToggleRCs( bool ) ) );
    connect( pDisplayTraces,    SIGNAL( toggled( bool ) ), this, SLOT( OnToggleTraces( bool ) ) );
    connect( pDisplayEvents,    SIGNAL( toggled( bool ) ), this, SLOT( OnToggleEvents( bool ) ) );
    connect( pDisplayMessages,  SIGNAL( toggled( bool ) ), this, SLOT( OnToggleMessages( bool ) ) );
    connect( pDisplayWarnings,  SIGNAL( toggled( bool ) ), this, SLOT( OnToggleWarnings( bool ) ) );
}

// -----------------------------------------------------------------------------
// Name: MOS_ReportFilterOptions destructor
// Created: AGE 2005-09-21
// -----------------------------------------------------------------------------
MOS_ReportFilterOptions::~MOS_ReportFilterOptions()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: MOS_ReportFilterOptions::ShouldDisplay
// Created: AGE 2005-09-21
// -----------------------------------------------------------------------------
bool MOS_ReportFilterOptions::ShouldDisplay( const MOS_Report_ABC& report ) const
{
    return toDisplay_.find( report.GetType() ) != toDisplay_.end();
}

// -----------------------------------------------------------------------------
// Name: MOS_ReportFilterOptions::Toggle
// Created: AGE 2005-09-21
// -----------------------------------------------------------------------------
void MOS_ReportFilterOptions::Toggle( bool bOn, MOS_Report_ABC::E_Type type )
{
    if( bOn )
        toDisplay_.insert( type );
    else
        toDisplay_.erase( type );
    emit OptionsChanged();
}

// -----------------------------------------------------------------------------
// Name: MOS_ReportFilterOptions::OnToggleRCs
// Created: AGE 2005-09-21
// -----------------------------------------------------------------------------
void MOS_ReportFilterOptions::OnToggleRCs( bool bOn )
{
    Toggle( bOn, MOS_Report_ABC::eRC );
}

// -----------------------------------------------------------------------------
// Name: MOS_ReportFilterOptions::OnToggleTraces
// Created: AGE 2005-09-21
// -----------------------------------------------------------------------------
void MOS_ReportFilterOptions::OnToggleTraces( bool bOn )
{
    Toggle( bOn, MOS_Report_ABC::eTrace );
}

// -----------------------------------------------------------------------------
// Name: MOS_ReportFilterOptions::OnToggleEvents
// Created: AGE 2005-09-21
// -----------------------------------------------------------------------------
void MOS_ReportFilterOptions::OnToggleEvents( bool bOn )
{
    Toggle( bOn, MOS_Report_ABC::eEvent );
}

// -----------------------------------------------------------------------------
// Name: MOS_ReportFilterOptions::OnToggleMessages
// Created: AGE 2005-09-21
// -----------------------------------------------------------------------------
void MOS_ReportFilterOptions::OnToggleMessages( bool bOn )
{
    Toggle( bOn, MOS_Report_ABC::eMessage );
}

// -----------------------------------------------------------------------------
// Name: MOS_ReportFilterOptions::OnToggleWarnings
// Created: AGE 2005-09-21
// -----------------------------------------------------------------------------
void MOS_ReportFilterOptions::OnToggleWarnings( bool bOn )
{
    Toggle( bOn, MOS_Report_ABC::eWarning );
}
