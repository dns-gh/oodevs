// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PROPAGATION_CAPACITY_h
#define SWORD_PROPAGATION_CAPACITY_h

#include "ObjectCapacity_ABC.h"

namespace sword
{
namespace capacity
{
// =============================================================================
/** @class  PropagationCapacity
    @brief  Propagation capacity
*/
// Created: LGY 2012-09-19
// =============================================================================
class PropagationCapacity : public ObjectCapacity_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit PropagationCapacity( xml::xistream& xis );
    virtual ~PropagationCapacity();
    //@}

    //! @name Operations
    //@{
    virtual void Register( MIL_Object_ABC& object );
    virtual void Instanciate( MIL_Object_ABC& object ) const;
    //@}
};

}

}

#endif // SWORD_PROPAGATION_CAPACITY_h
