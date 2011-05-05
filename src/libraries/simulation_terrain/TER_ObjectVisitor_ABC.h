// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TER_ObjectVisitor_ABC_h_
#define __TER_ObjectVisitor_ABC_h_

#include <boost/noncopyable.hpp>

// =============================================================================
/** @class  TER_ObjectVisitor_ABC
    @brief  TER Object Visitor
*/
// Created: JSR 2011-05-05
// =============================================================================
class TER_ObjectVisitor_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             TER_ObjectVisitor_ABC() {}
    virtual ~TER_ObjectVisitor_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Visit( TER_Object_ABC& object ) = 0;
    //@}
};

#endif // __TER_ObjectVisitor_ABC_h_
