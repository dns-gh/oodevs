// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Agent_h_
#define __Agent_h_

#include "Entity.h"
#include "Model.h"
#include "clients_kernel/Agent_ABC.h"
#include "tools/Resolver_ABC.h"
#pragma warning( push, 0 )
#include <QtCore/qstring.h>
#pragma warning( pop )

namespace kernel
{
    class AgentType;
    class ActionController;
}

namespace sword
{
    class UnitCreation;
}

namespace mission_tester
{
// =============================================================================
/** @class  Agent
    @brief  Agent
*/
// Created: PHC 2011-03-28
// =============================================================================
class Agent : public Entity
            , public kernel::Agent_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Agent( const sword::UnitCreation& message, const tools::Resolver_ABC< kernel::AgentType >& resolver );
    virtual ~Agent();
    //@}

    //! @name Operations
    //@{
    virtual const kernel::AgentType& GetType() const;
    virtual bool IsCommandPost() const;

    virtual bool Matches( const Filter_ABC& filter ) const;
    virtual QString GetTypeName() const;
    //@}

    //! @name Accessors
    //@{
    virtual QString GetName() const;
    virtual unsigned long GetId() const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::AgentType& type_;
    const bool isPc_;
    //@}
};

}

#endif // __Agent_h_
