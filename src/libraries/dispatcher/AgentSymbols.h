// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentSymbols_h_
#define __AgentSymbols_h_

#include "EntitySymbols_ABC.h"

namespace dispatcher
{
    class Agent;

// =============================================================================
/** @class  AgentSymbols
    @brief  AgentSymbols
*/
// Created: SBO 2008-02-13
// =============================================================================
class AgentSymbols : public EntitySymbols_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit AgentSymbols( const Agent& holder );
    virtual ~AgentSymbols();
    //@}

    //! @name Operations
    //@{
    virtual std::string BuildSymbol( bool up = true ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AgentSymbols( const AgentSymbols& );            //!< Copy constructor
    AgentSymbols& operator=( const AgentSymbols& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const Agent& holder_;
    //@}
};

}

#endif // __AgentSymbols_h_
