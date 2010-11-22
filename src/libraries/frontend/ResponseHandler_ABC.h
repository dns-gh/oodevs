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

namespace MsgsLauncherToAdmin
{
    class MsgControlStartAck;
    class MsgControlStopAck;
    class MsgExercicesListResponse;
}

namespace MsgsAuthenticationToClient
{
    class MsgProfileDescriptionList;
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
    virtual void Handle( const MsgsLauncherToAdmin::MsgExercicesListResponse& message ) = 0;
    virtual void Handle( const MsgsLauncherToAdmin::MsgControlStartAck& message ) = 0;
    virtual void Handle( const MsgsLauncherToAdmin::MsgControlStopAck& message ) = 0;
    virtual void Handle( const MsgsAuthenticationToClient::MsgProfileDescriptionList& message ) = 0;
    //@}
};

}

#endif // __ResponseHandler_ABC_h_
