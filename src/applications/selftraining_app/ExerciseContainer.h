// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 Mathematiques Appliquees SA (MASA)
//
// *****************************************************************************

#ifndef SELFTRAINING_APP_EXERCISE_CONTAINER_H
#define SELFTRAINING_APP_EXERCISE_CONTAINER_H

#include <boost/noncopyable.hpp>
#include <memory>
#include <map>

namespace frontend
{
    class Exercise_ABC;
}

namespace kernel
{
    class Controller;
}

namespace tools
{
    class GeneralConfig;
}

// This class is silly but comes as a replacement for the launcher facility
// which behaved the same way, maintaining a set of exercises and pushing
// notification upon changes.
//
// We should probably avoid having a global structure for all selftraining pages
// but this refactoring is for another day.
class ExerciseContainer: private boost::noncopyable
{
public:
    ExerciseContainer( const tools::GeneralConfig& config, kernel::Controller& controller );
    virtual ~ExerciseContainer();

    void Refresh();

private:
    const tools::GeneralConfig& config_;
    kernel::Controller& controller_;
    std::vector< std::shared_ptr< frontend::Exercise_ABC > > exercises_;
};

#endif // SELFTRAINING_APP_EXERCISE_CONTAINER_H
