// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_app_pch.h"
#include "Actions.h"
#include "resources.h"
#include "clients_gui/Tools.h"

#pragma warning( disable : 4355 )

// -----------------------------------------------------------------------------
// Name: Actions constructor
// Created: AGE 2007-10-10
// -----------------------------------------------------------------------------
Actions::Actions( QObject* parent )
    : QObject( parent )
    , group_( new Q3ActionGroup( this, "Action group", true ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Actions destructor
// Created: AGE 2007-10-10
// -----------------------------------------------------------------------------
Actions::~Actions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Actions::CreateAction
// Created: AGE 2007-10-10
// -----------------------------------------------------------------------------
Q3Action& Actions::CreateAction( const QString& name, const char* cname )
{
    Q3Action* result = new Q3Action( parent(), cname );
    result->setToggleAction( true );
    result->setMenuText( name );
    result->setText( name );
    group_->add( result );
    return *result;
}

// -----------------------------------------------------------------------------
// Name: Actions::CreateTerrain
// Created: AGE 2007-10-10
// -----------------------------------------------------------------------------
Q3Action& Actions::CreateTerrain()
{
    Q3Action& result = CreateAction( tools::translate( "Actions", "Create terrain" ), "Create terrain" );
    result.setIconSet( MAKE_PIXMAP( terrain_create ) );
    result.setToolTip( tools::translate( "Actions", "Import new terrain from VMap, DTED and USRP bases" ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: Actions::CreateExercise
// Created: AGE 2007-10-10
// -----------------------------------------------------------------------------
Q3Action& Actions::CreateExercise()
{
    Q3Action& result = CreateAction( tools::translate( "Actions", "Create exercise" ), "Create exercise" );
    result.setIconSet( MAKE_PIXMAP( exercise_create ) );
    result.setToolTip( tools::translate( "Actions", "Create new exercise using an existing terrain and an existing physical/decisional data set" ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: Actions::EditExercise
// Created: AGE 2007-10-10
// -----------------------------------------------------------------------------
Q3Action& Actions::EditExercise()
{
    Q3Action& result = CreateAction( tools::translate( "Actions", "Edit exercise" ), "Edit exercise" );
    result.setIconSet( MAKE_PIXMAP( exercise_edit ) );
    result.setToolTip( tools::translate( "Actions", "Change exercise Order of Battle, User Profiles and Weather settings" ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: Actions::StartExercise
// Created: AGE 2007-10-10
// -----------------------------------------------------------------------------
Q3Action& Actions::StartExercise()
{
    Q3Action& result = CreateAction( tools::translate( "Actions", "Start exercise" ), "Start exercise" );
    result.setIconSet( MAKE_PIXMAP( exercise_start ) );
    result.setToolTip( tools::translate( "Actions", "Start a new simulation session using an existing exercise" ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: Actions::RestartExercise
// Created: AGE 2007-10-10
// -----------------------------------------------------------------------------
Q3Action& Actions::RestartExercise()
{
    Q3Action& result = CreateAction( tools::translate( "Actions", "Restart exercise" ), "Restart exercise" );
    result.setIconSet( MAKE_PIXMAP( exercise_restart ) );
    result.setToolTip( tools::translate( "Actions", "Restart a simulation session using an existing checkpoint" ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: Actions::JoinExercise
// Created: AGE 2007-10-10
// -----------------------------------------------------------------------------
Q3Action& Actions::JoinExercise()
{
    Q3Action& result = CreateAction( tools::translate( "Actions", "Join exercise" ), "Join exercise" );
    result.setIconSet( MAKE_PIXMAP( exercise_join ) );
    result.setToolTip( tools::translate( "Actions", "Join a simulation session" ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: Actions::StartAnalysis
// Created: AGE 2007-10-10
// -----------------------------------------------------------------------------
Q3Action& Actions::StartAnalysis()
{
    Q3Action& result = CreateAction( tools::translate( "Actions", "Start analysis" ), "Start analysis" );
    result.setIconSet( MAKE_PIXMAP( analyse_create ) );
    result.setToolTip( tools::translate( "Actions", "Start the analysis of a previous simulation session" ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: Actions::JoinAnalysis
// Created: AGE 2007-10-10
// -----------------------------------------------------------------------------
Q3Action& Actions::JoinAnalysis()
{
    Q3Action& result = CreateAction( tools::translate( "Actions", "Join analysis" ), "Join analysis" );
    result.setIconSet( MAKE_PIXMAP( analyse_join ) );
    result.setToolTip( tools::translate( "Actions", "Join an analysis session" ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: Actions::InstallPackage
// Created: SBO 2008-03-14
// -----------------------------------------------------------------------------
Q3Action& Actions::InstallPackage()
{
    Q3Action& result = CreateAction( tools::translate( "Actions", "Install package" ), "Install package" );
    result.setIconSet( MAKE_PIXMAP( exercise_create ) );
    result.setToolTip( tools::translate( "Actions", "Install a data package" ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: Actions::CreatePackage
// Created: JCR 2009-11-09
// -----------------------------------------------------------------------------
Q3Action& Actions::CreatePackage()
{
    Q3Action& result = CreateAction( tools::translate( "Actions", "Create package" ), "Create package" );
    result.setIconSet( MAKE_PIXMAP( exercise_create ) );
    result.setToolTip( tools::translate( "Actions", "Generate a data package (.otpack) easily exportable" ) );
    return result;
}
