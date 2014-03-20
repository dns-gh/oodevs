// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 Mathematiques Appliquees SA (MASA)
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "ExerciseContainer.h"
#include "frontend/commands.h"
#include "frontend/Exercise_ABC.h"
#include "clients_kernel/Controller.h"

namespace
{

class Exercise : public frontend::Exercise_ABC
{
public:
    Exercise( const tools::Path& path, kernel::Controller& controller )
        : path_( path )
        , controller_( controller )
    {
        controller_.Create( static_cast< Exercise_ABC& >( *this ) );
    }

    ~Exercise()
    {
        controller_.Delete( static_cast< Exercise_ABC& >( *this ) );
    }

    virtual const tools::Path& GetName() const
    {
        return path_;
    }

private:
    const tools::Path path_;
    kernel::Controller& controller_;

};

}  // namespace

ExerciseContainer::ExerciseContainer( const tools::GeneralConfig& config,
        kernel::Controller& controller )
    : config_( config )
    , controller_( controller )
{
}

ExerciseContainer::~ExerciseContainer()
{
}

void ExerciseContainer::Refresh()
{
    exercises_.clear();
    const auto exercises = frontend::commands::ListExercises( config_ );
    for( auto it = exercises.cbegin(); it != exercises.end(); ++it )
        exercises_.push_back( std::make_shared< Exercise >( *it, controller_ ) );
    controller_.Update( *this );
}
