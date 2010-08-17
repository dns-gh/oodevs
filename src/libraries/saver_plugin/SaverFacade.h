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

namespace MsgsSimToClient
{
    class MsgSimToClient;
}

namespace dispatcher
{
    class Config;
    class Model;
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
             SaverFacade( dispatcher::Model& model, const dispatcher::Config& config );
    virtual ~SaverFacade();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const MsgsSimToClient::MsgSimToClient& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SaverFacade( const SaverFacade& );            //!< Copy constructor
    SaverFacade& operator=( const SaverFacade& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void SaveUpdate( const MsgsSimToClient::MsgSimToClient& message );
    void StartFrame( const MsgsSimToClient::MsgSimToClient& message );
    void EndFrame( const MsgsSimToClient::MsgSimToClient& message );
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::Model& model_;
    std::auto_ptr< Saver > saver_;
    std::string encodingBuffer_;
    unsigned frameCount_;
    MessageFilter filter_;
    //@}
};

}
}

#endif // __SaverFacade_h_
