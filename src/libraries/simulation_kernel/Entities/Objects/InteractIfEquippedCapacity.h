// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __InteractIfEquippedCapacity_h_
#define __InteractIfEquippedCapacity_h_

#include "ObjectCapacity_ABC.h"
#include "MIL_InteractiveContainer_ABC.h"

class Object;
class MIL_Agent_ABC;

// =============================================================================
/** @class  InteractIfEquippedCapacity
    @brief  InteractIfEquippedCapacity
*/
// Created: LDC 2009-03-03
// =============================================================================
class InteractIfEquippedCapacity : public ObjectCapacity_ABC
                                 , public MIL_InteractiveContainer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             InteractIfEquippedCapacity();
    explicit InteractIfEquippedCapacity( xml::xistream& xis );
    virtual ~InteractIfEquippedCapacity();
    //@}

    //! @name Operations
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    virtual void Instanciate( Object& object ) const;
    virtual void Register( Object& object );
    virtual void CanInteractWith( const Object& object, const MIL_Agent_ABC& /*agent*/, bool& canInteract );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    InteractIfEquippedCapacity( const InteractIfEquippedCapacity& );            //!< Copy constructor
    InteractIfEquippedCapacity& operator=( const InteractIfEquippedCapacity& ); //!< Assignment operator
    //@}
};

#endif // __InteractIfEquippedCapacity_h_
