// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __InteractWithEnemyCapacity_h_
#define __InteractWithEnemyCapacity_h_

#include "ObjectCapacity_ABC.h"
#include "MIL_InteractiveContainer_ABC.h"

class Object;
class MIL_Agent_ABC;

// =============================================================================
/** @class  InteractWithEnemyCapacity
    @brief  InteractWithEnemyCapacity
*/
// Created: LDC 2009-03-06
// =============================================================================
class InteractWithEnemyCapacity : public ObjectCapacity_ABC
                                , public MIL_InteractiveContainer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             InteractWithEnemyCapacity();
    explicit InteractWithEnemyCapacity( xml::xistream& xis );
    virtual ~InteractWithEnemyCapacity();
    //@}

    //! @name Operations
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    virtual void Instanciate( Object& object ) const;
    virtual void Register( Object& object );
    virtual void CanInteractWith( const Object& object, const MIL_Agent_ABC& agent, bool& canInteract );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    InteractWithEnemyCapacity( const InteractWithEnemyCapacity& );            //!< Copy constructor
    InteractWithEnemyCapacity& operator=( const InteractWithEnemyCapacity& ); //!< Assignment operator
    //@}
};

#endif // __InteractWithEnemyCapacity_h_
