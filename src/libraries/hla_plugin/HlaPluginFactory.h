// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef plugins_hla_HlaPluginFactory_h
#define plugins_hla_HlaPluginFactory_h

#include "dispatcher/PluginFactory_ABC.h"
#include <boost/noncopyable.hpp>

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  HlaPluginFactory
    @brief  Hla plugin factory
*/
// Created: SBO 2008-02-28
// =============================================================================
class HlaPluginFactory : public dispatcher::PluginFactory_ABC, private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             HlaPluginFactory();
    virtual ~HlaPluginFactory();
    //@}

    //! @name Operations
    //@{
    virtual std::auto_ptr< dispatcher::Plugin_ABC > Create( const std::string& name, xml::xistream& xis, const dispatcher::Config& config,
                                                            dispatcher::Model_ABC& model, const kernel::StaticModel& staticModel, dispatcher::SimulationPublisher_ABC& simulation,
                                                            dispatcher::ClientPublisher_ABC& clients, tools::MessageDispatcher_ABC& client, dispatcher::LinkResolver_ABC& resolver,
                                                            dispatcher::CompositeRegistrable& registrables ) const ;
    //@}
};

}
}

#endif // plugins_hla_HlaPluginFactory_h
