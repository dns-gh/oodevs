// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA GROUP
//
// *****************************************************************************

#ifndef __EdxlHavePluginFactory_h_
#define __EdxlHavePluginFactory_h_

#include "dispatcher/PluginFactory_ABC.h"

namespace plugins
{
namespace edxl
{

// =============================================================================
/** @class  EdxlHavePluginFactory
    @brief  EdxlHavePluginFactory
*/
// Created: JCR 2010-05-29
// =============================================================================
class EdxlHavePluginFactory : public dispatcher::PluginFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             EdxlHavePluginFactory();
    virtual ~EdxlHavePluginFactory();
    //@}

    //! @name Operations
    //@{
    virtual std::auto_ptr< dispatcher::Plugin_ABC > Create( const std::string& name,
                                                            xml::xistream& xis,
                                                            const dispatcher::Config& config,
                                                            dispatcher::Model_ABC& model,
                                                            const kernel::StaticModel& staticModel,
                                                            dispatcher::SimulationPublisher_ABC& simulationPublisher,
                                                            dispatcher::ClientPublisher_ABC& clients,
                                                            tools::MessageDispatcher_ABC& client,
                                                            dispatcher::LinkResolver_ABC& resolver,
                                                            dispatcher::CompositeRegistrable& registrables ) const ;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    EdxlHavePluginFactory( const EdxlHavePluginFactory& );            //!< Copy constructor
    EdxlHavePluginFactory& operator=( const EdxlHavePluginFactory& ); //!< Assignment operator
    //@}
};

}
}

#endif // __EdxlHavePluginFactory_h_
