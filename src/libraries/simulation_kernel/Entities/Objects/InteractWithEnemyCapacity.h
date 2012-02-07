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

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  InteractWithEnemyCapacity
    @brief  InteractWithEnemyCapacity
*/
// Created: LDC 2009-03-06
// =============================================================================
class InteractWithEnemyCapacity : public ObjectCapacity_ABC
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
    template< typename Archive > void serialize( Archive&, const unsigned int );
    virtual void Instanciate( MIL_Object_ABC& object ) const;
    virtual void Register( MIL_Object_ABC& object );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    InteractWithEnemyCapacity& operator=( const InteractWithEnemyCapacity& ); //!< Assignment operator
    //@}
};

BOOST_CLASS_EXPORT_KEY( InteractWithEnemyCapacity )

#endif // __InteractWithEnemyCapacity_h_
