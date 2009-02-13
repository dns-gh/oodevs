// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CrossbowPluginFactory_h_
#define __CrossbowPluginFactory_h_

#include "dispatcher/PluginFactory_ABC.h"

namespace plugins
{
namespace crossbow
{

// =============================================================================
/** @class  CrossbowPluginFactory
    @brief  CrossbowPluginFactory
*/
// Created: SBO 2008-02-28
// =============================================================================
class CrossbowPluginFactory : public dispatcher::PluginFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             CrossbowPluginFactory();
    virtual ~CrossbowPluginFactory();
    //@}

    //! @name Operations
    //@{
    virtual std::auto_ptr< dispatcher::Plugin_ABC > Create( const std::string& name, xml::xistream& xis, const dispatcher::Config& config,
            dispatcher::Model& model, dispatcher::SimulationPublisher_ABC& simulation, dispatcher::ClientPublisher_ABC& clients,
            tools::MessageDispatcher_ABC& client, dispatcher::LinkResolver_ABC& resolver, dispatcher::CompositeRegistrable& registrables ) const ;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    CrossbowPluginFactory( const CrossbowPluginFactory& );            //!< Copy constructor
    CrossbowPluginFactory& operator=( const CrossbowPluginFactory& ); //!< Assignment operator
    //@}
};

}
}

#endif // __CrossbowPluginFactory_h_
