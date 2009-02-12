// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __EsriPluginFactory_h_
#define __EsriPluginFactory_h_

#include "dispatcher/PluginFactory_ABC.h"

namespace plugins
{
namespace esri
{

// =============================================================================
/** @class  EsriPluginFactory
    @brief  EsriPluginFactory
*/
// Created: SBO 2008-02-28
// =============================================================================
class EsriPluginFactory : public dispatcher::PluginFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             EsriPluginFactory();
    virtual ~EsriPluginFactory();
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
    EsriPluginFactory( const EsriPluginFactory& );            //!< Copy constructor
    EsriPluginFactory& operator=( const EsriPluginFactory& ); //!< Assignment operator
    //@}
};

}
}

#endif // __EsriPluginFactory_h_
