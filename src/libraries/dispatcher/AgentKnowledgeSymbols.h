// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentKnowledgeSymbols_h_
#define __AgentKnowledgeSymbols_h_

#include "EntitySymbols_ABC.h"

namespace dispatcher
{
    class AgentKnowledge;

// =============================================================================
/** @class  AgentKnowledgeSymbols
    @brief  AgentKnowledgeSymbols
*/
// Created: SBO 2008-02-14
// =============================================================================
class AgentKnowledgeSymbols : public EntitySymbols_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit AgentKnowledgeSymbols( const AgentKnowledge& holder );
    virtual ~AgentKnowledgeSymbols();
    //@}

    //! @name Operations
    //@{
    virtual std::string BuildSymbol( bool up = true ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AgentKnowledgeSymbols( const AgentKnowledgeSymbols& );            //!< Copy constructor
    AgentKnowledgeSymbols& operator=( const AgentKnowledgeSymbols& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const AgentKnowledge& holder_;
    //@}
};

}

#endif // __AgentKnowledgeSymbols_h_
