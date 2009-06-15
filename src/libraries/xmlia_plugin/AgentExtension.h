// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentExtension_h_
#define __AgentExtension_h_

#include "xmliaExtension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "game_asn/Simulation.h"

namespace dispatcher
{
    class Agent;
    class Model;
}

namespace plugins
{
namespace xmlia
{
    class Publisher_ABC;
    class RapportManager;
    class Simulation;

// =============================================================================
/** @class  AgentExtension
    @brief  AgentExtension
*/
// Created: SBO 2008-02-29
// =============================================================================
class AgentExtension : public XmliaExtension_ABC
                     , public kernel::Updatable_ABC< ASN1T_MsgUnitAttributes >
                     , public kernel::Updatable_ABC< ASN1T_MsgUnitDetection >
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentExtension( dispatcher::Agent& holder, Publisher_ABC& publisher, RapportManager& factory, const Simulation& simulation, const dispatcher::Model& model );
    virtual ~AgentExtension();
    //@}

    //! @name Operations
    //@{
    virtual void DoUpdate( const ASN1T_MsgUnitAttributes& attributes );
    virtual void DoUpdate( const ASN1T_MsgUnitDetection& message );
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
    RapportManager& rapportManager_;
    const Simulation& simulation_;
    const dispatcher::Model& model_;
    unsigned long lastUpdate_;
    //@}
};

}
}

#endif // __AgentExtension_h_
