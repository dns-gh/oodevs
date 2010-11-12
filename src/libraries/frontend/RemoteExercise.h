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

namespace Common
{
    class MsgExercise;
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
             RemoteExercise( const Host_ABC& host, const ExerciseIdentifierFactory_ABC& factory, const Common::MsgExercise& message, kernel::Controller& controller );
    virtual ~RemoteExercise();
    //@}

    //! @name Accessors
    //@{
    virtual std::string GetId() const;
    virtual std::string GetName() const;
    virtual unsigned int GetPort() const;
    virtual bool IsRunning() const;
    virtual bool IsHostedBy( const std::string& host ) const;
    virtual void StartSession( const std::string& session ) const;
    virtual void StopSession( const std::string& session ) const;
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
    const std::string name_;
    const std::string id_;
    const unsigned int port_;
    bool running_;
    //@}
};

}

#endif // __RemoteExercise_h_
