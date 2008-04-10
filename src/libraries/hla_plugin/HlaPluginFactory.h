// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __HlaPluginFactory_h_
#define __HlaPluginFactory_h_

#include "dispatcher/PluginFactory_ABC.h"

namespace hla
{

// =============================================================================
/** @class  HlaPluginFactory
    @brief  HlaPluginFactory
*/
// Created: SBO 2008-02-28
// =============================================================================
class HlaPluginFactory : public dispatcher::PluginFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             HlaPluginFactory();
    virtual ~HlaPluginFactory();
    //@}

    //! @name Operations
    //@{
    virtual std::auto_ptr< dispatcher::Plugin_ABC > Create( const std::string& name, xml::xistream& xis, const dispatcher::Config& config, dispatcher::Model& model, dispatcher::SimulationPublisher_ABC& simulation, dispatcher::ClientPublisher_ABC& clients, tools::MessageDispatcher_ABC& client, dispatcher::LinkResolver_ABC& resolver) const ;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    HlaPluginFactory( const HlaPluginFactory& );            //!< Copy constructor
    HlaPluginFactory& operator=( const HlaPluginFactory& ); //!< Assignment operator
    //@}
};

}

#endif // __HlaPluginFactory_h_
