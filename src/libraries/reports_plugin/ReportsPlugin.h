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

namespace dispatcher
{
    class Model_ABC;
}

namespace plugins
{
namespace reports
{

class ReportsPlugin : public dispatcher::Plugin_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ReportsPlugin( const dispatcher::Model_ABC& model );
    virtual ~ReportsPlugin();
    //@}

    //! @name Operations
    //@{
    virtual bool HandleClientToSim( const sword::ClientToSim& msg,
        dispatcher::RewritingPublisher_ABC& unicaster, dispatcher::ClientPublisher_ABC& );
    //@}

private:
    //! @name Member data
    //@{
    const dispatcher::Model_ABC& model_;
    //@}
};
}
}

#endif // __ReportsPlugin_h_
