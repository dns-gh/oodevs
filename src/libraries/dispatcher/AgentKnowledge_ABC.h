// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __dispatcher_AgentKnowledge_ABC_h_
#define __dispatcher_AgentKnowledge_ABC_h_

#include "Sendable.h"
#include "clients_kernel/AgentKnowledge_ABC.h"

namespace MsgsSimToClient
{
    enum EnumUnitIdentificationLevel;
}

namespace dispatcher
{

// =============================================================================
/** @class  AgentKnowledge_ABC
    @brief  AgentKnowledge_ABC
*/
// Created: SBO 2010-06-08
// =============================================================================
class AgentKnowledge_ABC : public Sendable< kernel::AgentKnowledge_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit AgentKnowledge_ABC( unsigned long id, const QString& name = "" )
        : Sendable< kernel::AgentKnowledge_ABC >( id, name ) {}
    virtual ~AgentKnowledge_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual MsgsSimToClient::EnumUnitIdentificationLevel GetMaxPerceptionLevel() const = 0;
    virtual geometry::Point2d GetPosition() const = 0;
    //@}
};

}

#endif // __dispatcher_AgentKnowledge_ABC_h_
