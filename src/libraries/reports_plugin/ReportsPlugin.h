// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __ReportsPlugin_h_
#define __ReportsPlugin_h_

#include "dispatcher/Plugin_ABC.h"
#include <memory>

namespace dispatcher
{
    class Model_ABC;
}

namespace tools
{
    class SessionConfig;
}

namespace plugins
{
namespace reports
{
class Reports;

class ReportsPlugin : public dispatcher::Plugin_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ReportsPlugin( const tools::SessionConfig& config );
    virtual ~ReportsPlugin();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const sword::SimToClient& msg );
    virtual bool HandleClientToSim( const sword::ClientToSim& msg,
        dispatcher::RewritingPublisher_ABC& unicaster, dispatcher::ClientPublisher_ABC& );
    //@}

private:
    //! @name Member data
    //@{
    std::unique_ptr< Reports > reports_;
    const tools::SessionConfig& config_;
    //@}
};
}
}

#endif // __ReportsPlugin_h_
