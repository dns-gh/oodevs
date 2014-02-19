// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __RemoteExercise_h_
#define __RemoteExercise_h_

#include "Exercise_ABC.h"

namespace sword
{
    class Exercise;
}

namespace kernel
{
    class Controller;
}

namespace frontend
{
    class ExerciseIdentifierFactory_ABC;
    class Host_ABC;

// =============================================================================
/** @class  RemoteExercise
    @brief  RemoteExercise
*/
// Created: SBO 2010-10-01
// =============================================================================
class RemoteExercise : public Exercise_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             RemoteExercise( const Host_ABC& host, const ExerciseIdentifierFactory_ABC& factory,
                             const tools::Path& exercise, kernel::Controller& controller );
    virtual ~RemoteExercise();
    //@}

    //! @name Accessors
    //@{
    virtual const std::string& GetId() const;
    virtual const tools::Path& GetName() const;
    virtual unsigned int GetPort() const;
    virtual bool IsRunning() const;
    virtual bool IsHostedBy( const std::string& host ) const;
    virtual void Start( const tools::Path& session ) const;
    virtual void StartDispatcher( const tools::Path& session, const T_Parameters& parameters ) const;
    virtual void Replay( const tools::Path& session ) const;
    virtual void Stop( const tools::Path& session ) const;
    //@}

    //! @name Operations
    //@{
    virtual void SetRunning( bool running );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const Host_ABC& host_;
    const tools::Path name_;
    const std::string id_;
    const unsigned int port_;
    bool running_;
    //@}
};

}

#endif // __RemoteExercise_h_
