// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentHierarchies_h_
#define __AgentHierarchies_h_

#include "TacticalHierarchies.h"

namespace kernel {
    class Agent_ABC;
}

// =============================================================================
/** @class  AgentHierarchies
    @brief  AgentHierarchies
*/
// Created: AGE 2006-09-20
// =============================================================================
class AgentHierarchies : public TacticalHierarchies
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentHierarchies( kernel::Controller& controller, kernel::Agent_ABC& holder, kernel::Entity_ABC* superior );
    virtual ~AgentHierarchies();
    //@}

    //! @name Operations
    //@{
    virtual std::string GetLevel() const;
    virtual std::string GetSymbol() const;
    virtual void UpdateSymbol( bool up = true );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AgentHierarchies( const AgentHierarchies& );            //!< Copy constructor
    AgentHierarchies& operator=( const AgentHierarchies& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const kernel::InstanciationComplete& );
    void UpdateKarma( const kernel::Entity_ABC& superior );
    //@}

private:
    //! @name Member data
    //@{
    std::string level_;
    std::string baseSymbol_;
    std::string symbol_;
    kernel::Entity_ABC* superior_;
    //@}
};

#endif // __AgentHierarchies_h_
