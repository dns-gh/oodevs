// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentManipulator_h_
#define __AgentManipulator_h_

#include "dispatcher/Registrable_ABC.h"
#include "dispatcher/Position.h"
#include "clients_kernel/Extension_ABC.h"

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace dispatcher
{
    class Agent;
}

namespace script
{

// =============================================================================
/** @class  AgentManipulator
    @brief  AgentManipulator
*/
// Created: AGE 2008-06-17
// =============================================================================
class AgentManipulator : public kernel::Extension_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentManipulator( const kernel::CoordinateConverter_ABC& converter, const dispatcher::Agent& agent );
    virtual ~AgentManipulator();
    //@}

    //! @name Operations
    //@{
    struct Registrar : public dispatcher::Registrable_ABC
    {
        virtual void RegisterIn( directia::Brain& brain );
    };
    void UsedByDIA    () {};
    void ReleasedByDIA() {};
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AgentManipulator( const AgentManipulator& );            //!< Copy constructor
    AgentManipulator& operator=( const AgentManipulator& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    unsigned int GetIdentifier() const;
    std::string GetName() const;
    std::string GetTeam() const;
    dispatcher::Position GetPosition() const;
    unsigned int GetOperationalState() const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    const dispatcher::Agent& agent_;
    //@}
};

}

#endif // __AgentManipulator_h_
