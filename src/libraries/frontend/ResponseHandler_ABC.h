// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ResponseHandler_ABC_h_
#define __ResponseHandler_ABC_h_

#include <boost/noncopyable.hpp>

namespace sword
{
    class ExerciseListResponse;
    class SessionStartResponse;
    class SessionStopResponse;
    class ProfileListResponse;
    class SessionCommandExecutionResponse;
    class SessionNotification;
}

namespace frontend
{
// =============================================================================
/** @class  ResponseHandler_ABC
    @brief  ResponseHandler_ABC
*/
// Created: SBO 2010-11-22
// =============================================================================
class ResponseHandler_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ResponseHandler_ABC() {}
    virtual ~ResponseHandler_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Handle( const sword::ExerciseListResponse& message ) = 0;
    virtual void Handle( const sword::SessionStartResponse& message ) = 0;
    virtual void Handle( const sword::SessionStopResponse& message ) = 0;
    virtual void Handle( const sword::ProfileListResponse& message ) = 0;
    virtual void Handle( const sword::SessionCommandExecutionResponse& message ) = 0;
    virtual void Handle( const sword::SessionNotification& message ) = 0;
    //@}
};

}

#endif // __ResponseHandler_ABC_h_
