// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TicPluginFactory_h_
#define __TicPluginFactory_h_

#include "dispatcher/PluginFactory_ABC.h"

namespace plugins
{
namespace tic
{

// =============================================================================
/** @class  TicPluginFactory
    @brief  TicPluginFactory
*/
// Created: AGE 2008-04-01
// =============================================================================
class TicPluginFactory : public dispatcher::PluginFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             TicPluginFactory();
    virtual ~TicPluginFactory();
    //@}

    //! @name Operations
    //@{
    virtual std::auto_ptr< dispatcher::Plugin_ABC > Create( const std::string& name, xml::xistream& xis, const dispatcher::Config& config, dispatcher::Model& model, dispatcher::SimulationPublisher_ABC& simulation, dispatcher::ClientPublisher_ABC& clients, tools::MessageDispatcher_ABC& client, dispatcher::LinkResolver_ABC& resolver, dispatcher::CompositeRegistrable& registrables ) const ;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TicPluginFactory( const TicPluginFactory& );            //!< Copy constructor
    TicPluginFactory& operator=( const TicPluginFactory& ); //!< Assignment operator
    //@}
};

}
}

#endif // __TicPluginFactory_h_
