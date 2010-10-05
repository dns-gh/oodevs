// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __LauncherService_h_
#define __LauncherService_h_

#include <boost/ptr_container/ptr_map.hpp>
#include "tools/ServerNetworker.h"

namespace MsgsAdminToLauncher
{
    class MsgConnectionRequest;
}

namespace frontend
{

class LauncherPublisher;

// =============================================================================
/** @class  LauncherService
    @brief  LauncherService
*/
// Created: SBO 2010-09-29
// =============================================================================
class LauncherService : public tools::ServerNetworker
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit LauncherService( unsigned short port );
    virtual ~LauncherService();
    //@}

    //! @name Operations
    //@{
    LauncherPublisher& ResolveClient( const std::string& endpoint ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LauncherService( const LauncherService& );            //!< Copy constructor
    LauncherService& operator=( const LauncherService& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void ConnectionSucceeded( const std::string& endpoint );
    virtual void ConnectionError( const std::string& address, const std::string& error );
    //@}

private:
    //! @name Member data
    //@{
    boost::ptr_map< std::string, LauncherPublisher* > clients_;
    //@}
};

}

#endif // __LauncherService_h_
