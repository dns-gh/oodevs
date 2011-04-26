// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ModelFunction_ABC_h_
#define __ModelFunction_ABC_h_

#include <boost/noncopyable.hpp>

namespace sword
{
    class SimToClient;
}

// =============================================================================
/** @class  ModelFunction_ABC
    @brief  Model function definition
*/
// Created: AGE 2007-08-29
// =============================================================================
class ModelFunction_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ModelFunction_ABC() {}
    virtual ~ModelFunction_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void BeginTick() = 0;
    virtual void Receive( const sword::SimToClient& message ) = 0;
    virtual void EndTick() = 0;
    //@}
};

#endif // __ModelFunction_ABC_h_
