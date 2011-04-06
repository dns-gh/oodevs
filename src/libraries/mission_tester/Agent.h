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

#include "Schedulable_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "tools/Resolver_ABC.h"
#pragma warning( push, 0 )
#include <qstring.h>
#pragma warning( pop )

namespace sword
{
    class UnitCreation;
}

namespace kernel
{
    class Mission;
}

namespace mission_tester
{
    class Exercise;
// =============================================================================
/** @class  Agent
    @brief  Agent
*/
// Created: PHC 2011-03-28
// =============================================================================
class Agent : public Schedulable_ABC
            , public kernel::Agent_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Agent( const sword::UnitCreation& message, const tools::Resolver_ABC< kernel::AgentType >& resolver );
    virtual ~Agent();
    //@}

    //! @name Accessors
    //@{
    virtual QString GetName() const;
    virtual unsigned long GetId() const;
    virtual const kernel::AgentType& GetType() const;
    virtual bool IsCommandPost() const;
    virtual bool Matches( const Filter_ABC& filter ) const;
    virtual bool Trigger( State_ABC& state );
    virtual void Select( kernel::ActionController& controller ) const;
    virtual void ContextMenu( kernel::ActionController& controller, const QPoint& where ) const;
    virtual void Activate( kernel::ActionController& controller ) const;

    virtual void StartMission( Exercise& exercise );
    //@}

private:
    //! @name Member data
    //@{
    const unsigned long id_;
    const QString name_;
    const kernel::AgentType& type_;
    const bool commandPost_;
    std::auto_ptr< tools::Iterator_ABC< const kernel::Mission& > > current_;
    //@}
};
}

#endif // __Agent_h_
