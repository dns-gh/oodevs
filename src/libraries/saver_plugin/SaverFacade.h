// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SaverFacade_h_
#define __SaverFacade_h_

#include "dispatcher/MessageHandler_ABC.h"
#include "MessageFilter.h"
#include <memory>
#include <string>

namespace sword
{
    class ControlBeginTick;
    class SimToClient;
}

namespace dispatcher
{
    class Config;
    class ClientPublisher_ABC;
    class Model_ABC;
}

namespace plugins
{
namespace saver
{
    class Saver;

// =============================================================================
/** @class  SaverFacade
    @brief  Saver facade
*/
// Created: AGE 2007-04-10
// =============================================================================
class SaverFacade : public dispatcher::MessageHandler_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             SaverFacade( dispatcher::ClientPublisher_ABC& client, dispatcher::Model_ABC& model, const dispatcher::Config& config );
    virtual ~SaverFacade();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const sword::SimToClient& message );
    //@}

private:
    //! @name Helpers
    //@{
    void StartFrame( const sword::SimToClient& message, const sword::ControlBeginTick& controlBeginTickMsg );
    void EndFrame( const sword::SimToClient& message );
    //@}

private:
    //! @name Member data
    //@{
    const dispatcher::Config& config_;
    dispatcher::Model_ABC& model_;
    std::unique_ptr< Saver > saver_;
    std::string encodingBuffer_;
    MessageFilter filter_;
    unsigned int keyFrameIndex_;
    unsigned int fragmentIndex_;
    bool firstTick_;
    bool infoInitialized_;
    //@}
};

}
}

#endif // __SaverFacade_h_
