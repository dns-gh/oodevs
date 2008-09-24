// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __BmlPluginFactory_h_
#define __BmlPluginFactory_h_

#include "dispatcher/PluginFactory_ABC.h"

namespace plugins
{
namespace bml
{

// =============================================================================
/** @class  BmlPluginFactory
    @brief  BmlPluginFactory
*/
// Created: SBO 2008-02-29
// =============================================================================
class BmlPluginFactory : public dispatcher::PluginFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             BmlPluginFactory();
    virtual ~BmlPluginFactory();
    //@}

    //! @name Operations
    //@{
    virtual std::auto_ptr< dispatcher::Plugin_ABC > Create( const std::string& name, xml::xistream& xis, const dispatcher::Config& config, dispatcher::Model& model, dispatcher::SimulationPublisher_ABC& simulation, dispatcher::ClientPublisher_ABC& clients, tools::MessageDispatcher_ABC& client, dispatcher::LinkResolver_ABC& resolver, dispatcher::CompositeRegistrable& registrables ) const ;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    BmlPluginFactory( const BmlPluginFactory& );            //!< Copy constructor
    BmlPluginFactory& operator=( const BmlPluginFactory& ); //!< Assignment operator
    //@}
};

}
}

#endif // __BmlPluginFactory_h_
