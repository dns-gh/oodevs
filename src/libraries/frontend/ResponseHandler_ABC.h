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
    class SessionListResponse;
    class SessionNotification;
    class SessionParameterChangeResponse;
    class SessionStatus;
    class CheckpointListResponse;
    class CheckpointDeleteResponse;
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
    virtual void Handle( const sword::SessionListResponse& message ) = 0;
    virtual void Handle( const sword::SessionNotification& message ) = 0;
    virtual void Handle( const sword::SessionStatus& message ) = 0;
    virtual void Handle( const sword::CheckpointListResponse& message ) = 0;
    virtual void Handle( const sword::CheckpointDeleteResponse& message ) = 0;
    //@}
};

}

#endif // __ResponseHandler_ABC_h_
