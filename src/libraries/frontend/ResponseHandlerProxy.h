// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ResponseHandlerProxy_h_
#define __ResponseHandlerProxy_h_

#include "ResponseHandler_ABC.h"
#include <boost/shared_ptr.hpp>
#include <vector>

namespace frontend
{

// =============================================================================
/** @class  ResponseHandlerProxy
    @brief  ResponseHandlerProxy
*/
// Created: SBO 2010-11-22
// =============================================================================
class ResponseHandlerProxy : public ResponseHandler_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ResponseHandlerProxy();
    virtual ~ResponseHandlerProxy();
    //@}

    //! @name Operations
    //@{
    void SetMainHandler( boost::shared_ptr< ResponseHandler_ABC > handler );
    void ResetMainHandler();
    void Register( boost::shared_ptr< ResponseHandler_ABC > handler );
    //@}

    //! @name Proxy
    //@{
    virtual void Handle( const sword::ExerciseListResponse& message );
    virtual void Handle( const sword::SessionStartResponse& message );
    virtual void Handle( const sword::SessionStopResponse& message );
    virtual void Handle( const sword::ProfileListResponse& message );
    virtual void Handle( const sword::SessionCommandExecutionResponse& message );
    virtual void Handle( const sword::SessionNotification& message );
    virtual void Handle( const sword::SessionParameterChangeResponse& message );
    virtual void Handle( const sword::SessionStatus& message );
    virtual void Handle( const sword::ConnectedProfileListResponse& message );
    virtual void Handle( const sword::SessionListResponse& message );
    virtual void Handle( const sword::CheckpointListResponse& message );
    virtual void Handle( const sword::CheckpointDeleteResponse& message );
    //@}

private:
    //! @name Helpers
    //@{
    template< typename Message >
    void UpdateHandlers( const Message& message );
    //@}

private:
    //! @name Member data
    //@{
    boost::shared_ptr< ResponseHandler_ABC > mainHandler_;
    std::vector< boost::shared_ptr< ResponseHandler_ABC > > handlers_;
    //@}
};

}

#endif // __ResponseHandlerProxy_h_
