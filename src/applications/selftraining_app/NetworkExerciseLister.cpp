// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "NetworkExerciseLister.h"
#include "moc_NetworkExerciseLister.cpp"
#include "Config.h"
#include "ExerciseList.h"
#include "clients_kernel/Controllers.h"
#include "frontend/commands.h"
#include "frontend/Exercise.h"
#include "frontend/Exercises.h"
#include "frontend/LauncherClient.h"
#include <boost/lexical_cast.hpp>
#include <qtimer.h>

// -----------------------------------------------------------------------------
// Name: NetworkExerciseLister constructor
// Created: LDC 2008-10-24
// -----------------------------------------------------------------------------
NetworkExerciseLister::NetworkExerciseLister( QObject* parent, kernel::Controllers& controllers, const Config& config, const std::string& subDir )
    : controllers_( controllers )
    , config_( config )
    , subDir_( subDir )
    , launcher_( new frontend::LauncherClient( controllers.controller_ ) )
    , list_( 0 )
    , timer_( new QTimer( parent ) )
    , pendingQuery_( false )
{
    connect( timer_, SIGNAL( timeout() ), SLOT( Update() ) );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: NetworkExerciseLister destructor
// Created: LDC 2008-10-24
// -----------------------------------------------------------------------------
NetworkExerciseLister::~NetworkExerciseLister()
{
    timer_->stop();
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: NetworkExerciseLister::QueryExercises
// Created: SBO 2010-10-04
// -----------------------------------------------------------------------------
void NetworkExerciseLister::QueryExercises( const std::string& host, unsigned int port, ExerciseList& list )
{
    list_ = &list;
    pendingQuery_ = true;
    launcher_->Connect( host, port );
    timer_->start( 100 );
}

// -----------------------------------------------------------------------------
// Name: NetworkExerciseLister::Update
// Created: SBO 2010-10-04
// -----------------------------------------------------------------------------
void NetworkExerciseLister::Update()
{
    if( launcher_->Connected() && pendingQuery_ )
    {
        exercises_.clear();
        if( list_ )
            list_->Clear();
        launcher_->QueryExerciseList();
        pendingQuery_ = false;
    }
    launcher_->Update();
}

// -----------------------------------------------------------------------------
// Name: NetworkExerciseLister::NotifyUpdated
// Created: SBO 2010-10-01
// -----------------------------------------------------------------------------
void NetworkExerciseLister::NotifyUpdated( const frontend::Exercises& exercises )
{
    if( launcher_->Connected() )
        launcher_->Disconnect();
    tools::Iterator< const frontend::Exercise& > it( exercises.CreateIterator() );
    while( it.HasMoreElements() )
    {
        const frontend::Exercise& exercise = it.NextElement();
        if( exercise.IsRunning() )
            exercises_[ exercise.GetName() ] = exercise.GetPort();
    }
    list_->Update();
}

// -----------------------------------------------------------------------------
// Name: NetworkExerciseLister::ListExercises
// Created: LDC 2008-10-24
// -----------------------------------------------------------------------------
void NetworkExerciseLister::ListExercises( QStringList& list ) const
{
    list.clear();
    for( T_PortDictionary::const_iterator it = exercises_.begin(); it != exercises_.end(); ++it )
        list.append( it->first.c_str() );
}

// -----------------------------------------------------------------------------
// Name: NetworkExerciseLister::GetPort
// Created: LDC 2008-10-24
// -----------------------------------------------------------------------------
unsigned short NetworkExerciseLister::GetPort( const QString& exercise ) const
{
    const std::string exerciseName = exercise.ascii();
    T_PortDictionary::const_iterator it = exercises_.find( exerciseName );
    if( it != exercises_.end() )
        return unsigned short( it->second );
    return 0;
}
