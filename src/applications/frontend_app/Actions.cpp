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

// -----------------------------------------------------------------------------
// Name: Actions constructor
// Created: AGE 2007-10-10
// -----------------------------------------------------------------------------
Actions::Actions( QObject* parent )
    : QObject( parent )
    , group_( new QActionGroup( this, "Action group", true ) )
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
QAction& Actions::CreateAction( const QString& name, const char* cname )
{
    QAction* result = new QAction( parent(), cname );
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
QAction& Actions::CreateTerrain()
{
    QAction& result = CreateAction( tr( "Create terrain" ), "Create terrain" );
    result.setIconSet( MAKE_PIXMAP( terrain_create ) );
    result.setToolTip( tr( "Import new terrain from VMap, DTED and USRP bases" ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: Actions::CreateExercise
// Created: AGE 2007-10-10
// -----------------------------------------------------------------------------
QAction& Actions::CreateExercise()
{
    QAction& result = CreateAction( tr( "Create exercise" ), "Create exercise" );
    result.setIconSet( MAKE_PIXMAP( exercise_create ) );
    result.setToolTip( tr( "Create new exercise using an existing terrain and an existing physical/decisional data set" ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: Actions::EditExercise
// Created: AGE 2007-10-10
// -----------------------------------------------------------------------------
QAction& Actions::EditExercise()
{
    QAction& result = CreateAction( tr( "Edit exercise" ), "Edit exercise" );
    result.setIconSet( MAKE_PIXMAP( exercise_edit ) );
    result.setToolTip( tr( "Change exercise Order of Battle, User Profiles and Weather settings" ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: Actions::StartExercise
// Created: AGE 2007-10-10
// -----------------------------------------------------------------------------
QAction& Actions::StartExercise()
{
    QAction& result = CreateAction( tr( "Start exercise" ), "Start exercise" );
    result.setIconSet( MAKE_PIXMAP( data_create ) ); // $$$$ AGE 2007-10-10: 
    result.setToolTip( tr( "Start a new simulation session using an existing exercise" ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: Actions::RestartExercise
// Created: AGE 2007-10-10
// -----------------------------------------------------------------------------
QAction& Actions::RestartExercise()
{
    QAction& result = CreateAction( tr( "Restart exercise" ), "Restart exercise" );
    result.setIconSet( MAKE_PIXMAP( data_create ) ); // $$$$ AGE 2007-10-10: 
    result.setToolTip( tr( "Restart a simulation session using an existing checkpoint" ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: Actions::JoinExercise
// Created: AGE 2007-10-10
// -----------------------------------------------------------------------------
QAction& Actions::JoinExercise()
{
    QAction& result = CreateAction( tr( "Join exercise" ), "Join exercise" );
    result.setIconSet( MAKE_PIXMAP( data_create ) ); // $$$$ AGE 2007-10-10: 
    result.setToolTip( tr( "Join a simulation session" ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: Actions::StartAnalysis
// Created: AGE 2007-10-10
// -----------------------------------------------------------------------------
QAction& Actions::StartAnalysis()
{
    QAction& result = CreateAction( tr( "Start analysis" ), "Start analysis" );
    result.setIconSet( MAKE_PIXMAP( data_create ) ); // $$$$ AGE 2007-10-10: 
    result.setToolTip( tr( "Start the analysis of a previous simulation session" ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: Actions::JoinAnalysis
// Created: AGE 2007-10-10
// -----------------------------------------------------------------------------
QAction& Actions::JoinAnalysis()
{
    QAction& result = CreateAction( tr( "Join analysis" ), "Join analysis" );
    result.setIconSet( MAKE_PIXMAP( data_edit ) ); // $$$$ AGE 2007-10-10: 
    result.setToolTip( tr( "Join an analysis session" ) );
    return result;
}
