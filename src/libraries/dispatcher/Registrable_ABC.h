// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Registrable_ABC_h_
#define __Registrable_ABC_h_

#include <boost/noncopyable.hpp>

namespace directia
{
namespace brain
{
    class Brain;
}
}

namespace dispatcher
{
// =============================================================================
/** @class  Registrable_ABC
    @brief  Registrable interface
*/
// Created: AGE 2008-06-12
// =============================================================================
class Registrable_ABC : boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Registrable_ABC() {}
    virtual ~Registrable_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void RegisterIn( directia::brain::Brain& brain ) = 0;

    virtual void UsedByDIA    () {}
    virtual void ReleasedByDIA() {}
    //@}
};

}

#endif // __Registrable_ABC_h_
