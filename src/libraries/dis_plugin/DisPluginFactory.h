// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DisPluginFactory_h_
#define __DisPluginFactory_h_

#include "dispatcher/PluginFactory_ABC.h"

namespace plugins
{
namespace dis
{

// =============================================================================
/** @class  DisPluginFactory
    @brief  DisPluginFactory
*/
// Created: AGE 2008-03-10
// =============================================================================
class DisPluginFactory : public dispatcher::PluginFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             DisPluginFactory();
    virtual ~DisPluginFactory();
    //@}

    //! @name Operations
    //@{
    virtual std::auto_ptr< dispatcher::Plugin_ABC > Create( const std::string& name, xml::xistream& xis, const dispatcher::Config& config, dispatcher::Model& model, dispatcher::SimulationPublisher_ABC& simulation, dispatcher::ClientPublisher_ABC& clients, tools::MessageDispatcher_ABC& client, dispatcher::LinkResolver_ABC& resolver, dispatcher::CompositeRegistrable& registrables ) const ;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DisPluginFactory( const DisPluginFactory& );            //!< Copy constructor
    DisPluginFactory& operator=( const DisPluginFactory& ); //!< Assignment operator
    //@}
};

}
}

#endif // __DisPluginFactory_h_
