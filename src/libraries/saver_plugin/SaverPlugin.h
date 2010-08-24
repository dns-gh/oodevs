// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __SaverPlugin_h_
#define __SaverPlugin_h_

#include "dispatcher/Plugin_ABC.h"
#include <memory>

namespace dispatcher
{
    class Model;
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
             SaverPlugin( dispatcher::Model& model, const dispatcher::Config& config );
    virtual ~SaverPlugin();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const MsgsSimToClient::MsgSimToClient& message );

    virtual void NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& client, dispatcher::Profile_ABC& profile );
    virtual void NotifyClientLeft( dispatcher::ClientPublisher_ABC& client );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SaverPlugin( const SaverPlugin& );            //!< Copy constructor
    SaverPlugin& operator=( const SaverPlugin& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< SaverFacade > saver_;
    //@}
};

}
}

#endif // __SaverPlugin_h_
