// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __HierarchyPlugin_h_
#define __HierarchyPlugin_h_

#include "dispatcher/Plugin_ABC.h"
#include <memory>

namespace dispatcher
{
    class Model_ABC;
    class ClientPublisher_ABC;
}

namespace tools
{
    class SessionConfig;
}

namespace plugins
{
class Hierarchies;
}

namespace plugins
{
class HierarchyPlugin : public dispatcher::Plugin_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             HierarchyPlugin( const tools::SessionConfig& config, bool replay );
    virtual ~HierarchyPlugin();
    //@}

    //! @name MessageHandler_ABC
    //@{
    virtual void Receive( const sword::SimToClient& msg );
    //@{

    //! @name Plugin_ABC
    //@{
    virtual void NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC&, const std::string&, dispatcher::Profile_ABC&, unsigned int, bool );
    virtual void NotifyClientLeft( dispatcher::ClientPublisher_ABC&, const std::string&, bool );
    //@{

    //! @name Plugin_ABC
    //@{
    virtual void SendState( dispatcher::ClientPublisher_ABC& );
    //@}

private:
    //! @name Member data
    //@{
    const tools::SessionConfig& config_;
    const bool replay_;
    std::unique_ptr< Hierarchies > data_;
    //@}
};
}

#endif // __HierarchyPlugin_h_
