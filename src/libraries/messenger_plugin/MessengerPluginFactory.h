// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MessengerPluginFactory_h_
#define __MessengerPluginFactory_h_

#include "dispatcher/PluginFactory_ABC.h"

namespace messenger
{

// =============================================================================
/** @class  MessengerPluginFactory
       @brief  MessengerPluginFactory
*/
// Created: AGE 2008-04-01
// =============================================================================
class MessengerPluginFactory : public dispatcher::PluginFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MessengerPluginFactory();
    virtual ~MessengerPluginFactory();
    //@}

    //! @name Operations
    //@{
    virtual std::auto_ptr< dispatcher::Plugin_ABC > Create( const std::string& name, xml::xistream& xis, const dispatcher::Config& config, dispatcher::Model& model, dispatcher::SimulationPublisher_ABC& simulation, dispatcher::ClientPublisher_ABC& clients, tools::MessageDispatcher_ABC& client, dispatcher::LinkResolver_ABC& links) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MessengerPluginFactory( const MessengerPluginFactory& );            //!< Copy constructor
    MessengerPluginFactory& operator=( const MessengerPluginFactory& ); //!< Assignment operator
    //@}
};

}

#endif // __MessengerPluginFactory_h_
