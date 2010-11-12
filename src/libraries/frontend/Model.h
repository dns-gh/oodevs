// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Model_h_
#define __Model_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <map>

namespace kernel
{
    class Controller;
}

namespace MsgsLauncherToAdmin
{
    class MsgControlStartAck;
    class MsgControlStopAck;
    class MsgExercicesListResponse;
}

namespace tools
{
    class GeneralConfig;
}

namespace frontend
{
    class Host_ABC;
    class LauncherPublisher;
    class RemoteHost;

// =============================================================================
/** @class  Model
    @brief  Model
*/
// Created: SBO 2010-10-21
// =============================================================================
class Model : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             Model( const tools::GeneralConfig& config, kernel::Controller& controller );
    virtual ~Model();
    //@}

    //! @name Operations
    //@{
    Host_ABC& CreateHost( LauncherPublisher& publisher, const std::string& host );
    void RemoveHost( Host_ABC& host );
    void Update( const std::string& host, const MsgsLauncherToAdmin::MsgExercicesListResponse& message );
    void Update( const std::string& host, const MsgsLauncherToAdmin::MsgControlStartAck& message );
    void Update( const std::string& host, const MsgsLauncherToAdmin::MsgControlStopAck& message );
    //@}

private:
    //! @name Helpers
    //@{
    template< typename Message >
    void UpdateHost( const std::string& host, const Message& message );
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    kernel::Controller& controller_;
    std::map< std::string, boost::shared_ptr< RemoteHost > > hosts_;
    //@}
};

}

#endif // __Model_h_
