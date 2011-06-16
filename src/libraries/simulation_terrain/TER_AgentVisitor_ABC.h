// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TER_AgentVisitor_ABC_h_
#define __TER_AgentVisitor_ABC_h_

#include <boost/noncopyable.hpp>

// =============================================================================
/** @class  TER_AgentVisitor_ABC
    @brief  TER Agent Visitor
*/
// Created: MMC 2011-06-14
// =============================================================================
class TER_AgentVisitor_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             TER_AgentVisitor_ABC() {}
    virtual ~TER_AgentVisitor_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Visit( TER_Agent_ABC& object ) = 0;
    //@}
};

#endif // __TER_AgentVisitor_ABC_h_
