// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __GearthPluginFactory_h_
#define __GearthPluginFactory_h_

#include "dispatcher/PluginFactory_ABC.h"

namespace gearth
{

// =============================================================================
/** @class  GearthPluginFactory
    @brief  GearthPluginFactory
*/
// Created: SBO 2008-02-28
// =============================================================================
class GearthPluginFactory : public dispatcher::PluginFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             GearthPluginFactory();
    virtual ~GearthPluginFactory();
    //@}

    //! @name Operations
    //@{
    virtual std::auto_ptr< dispatcher::Plugin_ABC > Create( const std::string& name, xml::xistream& xis, const dispatcher::Config& config, dispatcher::Model& model, dispatcher::SimulationPublisher_ABC& simulation, dispatcher::ClientPublisher_ABC& clients, tools::MessageDispatcher_ABC& clientCommands ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    GearthPluginFactory( const GearthPluginFactory& );            //!< Copy constructor
    GearthPluginFactory& operator=( const GearthPluginFactory& ); //!< Assignment operator
    //@}
};

}

#endif // __GearthPluginFactory_h_
