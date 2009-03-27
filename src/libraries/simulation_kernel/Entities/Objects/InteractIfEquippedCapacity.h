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
class InteractIfEquippedCapacity
    : public ObjectCapacity_ABC
    , public MIL_InteractiveContainer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit InteractIfEquippedCapacity( xml::xistream& xis );
             InteractIfEquippedCapacity();
    virtual ~InteractIfEquippedCapacity();
    //@}

    //! @name Operations
    //@{
    virtual void CanInteractWith( const Object& object, const MIL_Agent_ABC& /*agent*/, bool& canInteract );
    
    template< typename Archive > void serialize( Archive&, const uint );
    virtual void Register( Object& /*object*/ );
    //@}

    //! @name Capacity creation
    //@{
    virtual void Instanciate( Object& /*object*/ ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    InteractIfEquippedCapacity( const InteractIfEquippedCapacity& );            //!< Copy constructor
    InteractIfEquippedCapacity& operator=( const InteractIfEquippedCapacity& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __InteractIfEquippedCapacity_h_
