// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __LivingAreaVisitor_ABC_h_
#define __LivingAreaVisitor_ABC_h_

#include <boost/noncopyable.hpp>
#include <string>

namespace kernel
{

// =============================================================================
/** @class  LivingAreaVisitor_ABC
    @brief  Living area visitor declaration
*/
// Created: LGY 2011-05-02
// =============================================================================
class LivingAreaVisitor_ABC : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             LivingAreaVisitor_ABC() {}
    virtual ~LivingAreaVisitor_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Visit( unsigned int id, unsigned int resident, bool alerted, float angriness, bool confined, bool evacuated ) = 0;
    //@}
};

}

#endif // __LivingAreaVisitor_ABC_h_
