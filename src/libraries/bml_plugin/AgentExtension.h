// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __AgentExtension_h_
#define __AgentExtension_h_

#include "BmlExtension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "protocol/protocol.h"

using namespace Common;
using namespace MsgsSimToClient;

namespace dispatcher
{
    class Agent;
    class Model;
}

namespace plugins
{
namespace bml
{
    class Publisher_ABC;
    class ReportFactory;
    class Simulation;

// =============================================================================
/** @class  AgentExtension
    @brief  AgentExtension
*/
// Created: SBO 2008-02-29
// =============================================================================
class AgentExtension : public BmlExtension_ABC
                     , public kernel::Updatable_ABC< MsgUnitAttributes >
                     , public kernel::Updatable_ABC< MsgUnitOrder >
                     , public kernel::Updatable_ABC< MsgUnitDetection >
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentExtension( dispatcher::Agent& holder, Publisher_ABC& publisher, const ReportFactory& factory, const Simulation& simulation, const dispatcher::Model& model );
    virtual ~AgentExtension();
    //@}

    //! @name Operations
    //@{
    virtual void DoUpdate( const MsgUnitAttributes& attributes );
    virtual void DoUpdate( const MsgUnitOrder& message );
    virtual void DoUpdate( const MsgUnitDetection& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AgentExtension( const AgentExtension& );            //!< Copy constructor
    AgentExtension& operator=( const AgentExtension& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::Agent& holder_;
    Publisher_ABC& publisher_;
    const ReportFactory& factory_;
    const Simulation& simulation_;
    const dispatcher::Model& model_;
    unsigned long lastUpdate_;
    //@}
};

}
}

#endif // __AgentExtension_h_
