// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __BmlPlugin_h_
#define __BmlPlugin_h_

#include "dispatcher/Plugin_ABC.h"

namespace MsgsSimToClient
{
    class MsgSimToClient;
}

namespace xml
{
    class xistream;
}

namespace dispatcher
{
    class Model_ABC;
    class SimulationPublisher_ABC;
}

namespace kernel
{
    class StaticModel;
}

namespace plugins
{
namespace bml
{
    class ExtensionFactory;
    class PublisherActor;
    class Serializer;
    class UpdateListener;
    class ReportFactory;
    class Simulation;

// =============================================================================
/** @class  BmlPlugin
    @brief  BmlPlugin
*/
// Created: SBO 2008-02-29
// =============================================================================
class BmlPlugin : public dispatcher::Plugin_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             BmlPlugin( dispatcher::Model_ABC& model, const kernel::StaticModel& staticModel, xml::xistream& xis, dispatcher::SimulationPublisher_ABC& simulation );
    virtual ~BmlPlugin();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const MsgsSimToClient::MsgSimToClient& message );

    virtual void NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& client, dispatcher::Profile_ABC& profile );
    virtual void NotifyClientLeft( dispatcher::ClientPublisher_ABC& client );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    BmlPlugin( const BmlPlugin& );            //!< Copy constructor
    BmlPlugin& operator=( const BmlPlugin& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::Model_ABC& model_;
    std::auto_ptr< PublisherActor > publisher_;
    std::auto_ptr< Simulation > simulation_;
    std::auto_ptr< ReportFactory > reportFactory_;
    std::auto_ptr< ExtensionFactory > extensionFactory_;
    std::auto_ptr< UpdateListener > listener_;
    //@}
};

}
}

#endif // __BmlPlugin_h_
