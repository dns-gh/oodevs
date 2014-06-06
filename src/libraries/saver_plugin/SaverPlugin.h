// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SaverPlugin_h_
#define __SaverPlugin_h_

#include "dispatcher/Plugin_ABC.h"
#include <memory>

namespace dispatcher
{
    class Model_ABC;
    class Config;
}

namespace plugins
{
namespace saver
{
    class SaverFacade;

// =============================================================================
/** @class  SaverPlugin
    @brief  Saver plugin
*/
// Created: AGE 2007-08-24
// =============================================================================
class SaverPlugin : public dispatcher::Plugin_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             SaverPlugin( dispatcher::ClientPublisher_ABC& client, dispatcher::Model_ABC& model, const dispatcher::Config& config );
    virtual ~SaverPlugin();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const sword::SimToClient& message );
    //@}

private:
    //! @name Member data
    //@{
    std::unique_ptr< SaverFacade > saver_;
    //@}
};

}
}

#endif // __SaverPlugin_h_
