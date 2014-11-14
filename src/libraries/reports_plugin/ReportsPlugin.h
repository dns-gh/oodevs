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
#include <boost/shared_ptr.hpp>
#include <memory>

namespace dispatcher
{
    class Config;
    class Model_ABC;
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
             ReportsPlugin( const dispatcher::Config& config, bool replay );
    virtual ~ReportsPlugin();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const sword::SimToClient& msg );
    virtual bool HandleClientToSim( const sword::ClientToSim& msg,
        dispatcher::RewritingPublisher_ABC& unicaster, dispatcher::ClientPublisher_ABC& );
    virtual bool HandleClientToReplay( const sword::ClientToReplay& msg,
        dispatcher::RewritingPublisher_ABC& unicaster, dispatcher::ClientPublisher_ABC& );
    virtual void SendState( dispatcher::ClientPublisher_ABC& );
    //@}

private:
    template< typename R, typename M >
    bool HandleClientTo( const M& msg, dispatcher::RewritingPublisher_ABC& unicaster );

private:
    //! @name Member data
    //@{
    std::unique_ptr< Reports > reports_;
    const dispatcher::Config& config_;
    int currentTick_;
    const unsigned int frequency_;
    const bool replay_;
    //@}
};
}
}

#endif // __ReportsPlugin_h_
