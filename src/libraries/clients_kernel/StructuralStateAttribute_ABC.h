// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef StructuralStateAttribute_ABC_h
#define StructuralStateAttribute_ABC_h

#include "Extension_ABC.h"
#include "Displayable_ABC.h"
#include "Updatable_ABC.h"
#include <boost/noncopyable.hpp>

namespace sword
{
    class UrbanCreation;
    class UrbanUpdate;
}

namespace kernel
{
// =============================================================================
/** @class  StructuralStateAttribute_ABC
    @brief  Structural state attribute definition
*/
// Created: LGY 2012-06-15
// =============================================================================
class StructuralStateAttribute_ABC : public Extension_ABC
                                   , public Displayable_ABC
                                   , public Updatable_ABC< sword::UrbanUpdate >
                                   , public Updatable_ABC< sword::UrbanCreation >
                                   , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             StructuralStateAttribute_ABC() {}
    virtual ~StructuralStateAttribute_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void DoUpdate( const sword::UrbanCreation& ) {}
    virtual void DoUpdate( const sword::UrbanUpdate& ) {}
    virtual unsigned int GetValue() const = 0;
    //@}
};

}

#endif // StructuralStateAttribute_ABC_h
