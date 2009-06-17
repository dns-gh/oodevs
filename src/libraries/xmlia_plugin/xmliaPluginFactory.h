// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __XmliaPluginFactory_h_
#define __XmliaPluginFactory_h_

#include "dispatcher/PluginFactory_ABC.h"

namespace plugins
{
namespace xmlia
{

// =============================================================================
/** @class  XmliaPluginFactory
    @brief  XmliaPluginFactory
*/
// Created: SBO 2008-02-29
// =============================================================================
class XmliaPluginFactory : public dispatcher::PluginFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             XmliaPluginFactory();
    virtual ~XmliaPluginFactory();
    //@}

    //! @name Operations
    //@{
    virtual std::auto_ptr< dispatcher::Plugin_ABC > Create( const std::string& name,
															xml::xistream& xis,
															const dispatcher::Config& config,
															dispatcher::Model& model,
															dispatcher::SimulationPublisher_ABC& simulationPublisher,
															dispatcher::ClientPublisher_ABC& clients,
															tools::MessageDispatcher_ABC& client,
															dispatcher::LinkResolver_ABC& resolver,
															dispatcher::CompositeRegistrable& registrables ) const ;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    XmliaPluginFactory( const XmliaPluginFactory& );            //!< Copy constructor
    XmliaPluginFactory& operator=( const XmliaPluginFactory& ); //!< Assignment operator
    //@}
};

}
}

#endif // __XmliaPluginFactory_h_
