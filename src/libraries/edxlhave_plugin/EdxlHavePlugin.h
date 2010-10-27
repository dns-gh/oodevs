// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA GROUP
//
// *****************************************************************************

#ifndef __dispatcher_plugin_EdxlHavePlugin_h_
#define __dispatcher_plugin_EdxlHavePlugin_h_

#include "dispatcher/Plugin_ABC.h"
// #include "protocol/protocol.h"

namespace xml
{
    class xistream;
}

namespace kernel
{
    class StaticModel;
}

namespace dispatcher
{
    class Model_ABC;
    class SimulationPublisher_ABC;
}

namespace plugins
{
namespace edxl
{
    class ExtensionFactory;
    class PublisherActor;
    class ReportManager;
    class Simulation;
    class UpdateListener;
    class ReportBuilder_ABC;

// =============================================================================
/** @class  EdxlHavePlugin
    @brief  EdxlHavePlugin
*/
// Created: JCR 2010-05-29
// =============================================================================
class EdxlHavePlugin : public dispatcher::Plugin_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             EdxlHavePlugin( dispatcher::Model_ABC& model, const kernel::StaticModel& staticModel, xml::xistream& xis, dispatcher::SimulationPublisher_ABC& simulationPublisher );
    virtual ~EdxlHavePlugin();
    //@}

    //! @name Operations
    //@{
    virtual void Receive                  ( const MsgsSimToClient::MsgSimToClient& message );
    virtual void NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& client, dispatcher::Profile_ABC& profile );
    virtual void NotifyClientLeft         ( dispatcher::ClientPublisher_ABC& client );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    EdxlHavePlugin( const EdxlHavePlugin& );            //!< Copy constructor
    EdxlHavePlugin& operator=( const EdxlHavePlugin& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::Model_ABC& model_;
    dispatcher::SimulationPublisher_ABC& simulationPublisher_;
    std::auto_ptr< ReportBuilder_ABC > reports_;
    std::auto_ptr< Simulation > simulation_;
    std::auto_ptr< PublisherActor > publisher_;
    std::auto_ptr< ExtensionFactory > extensionFactory_;
    std::auto_ptr< UpdateListener > listener_;
    //@}
};

}
}

#endif // __dispatcher_plugin_EdxlHavePlugin_h_
