// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __FireForbiddenCapacity_h_
#define __FireForbiddenCapacity_h_

#include "ObjectCapacity_ABC.h"

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  FireForbiddenCapacity
    @brief  FireForbiddenCapacity
*/
// Created: JSR 2011-08-31
// =============================================================================
class FireForbiddenCapacity : public ObjectCapacity_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             FireForbiddenCapacity();
    explicit FireForbiddenCapacity( xml::xistream& xis );
    virtual ~FireForbiddenCapacity();
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
    FireForbiddenCapacity( const FireForbiddenCapacity& );            //!< Copy constructor
    FireForbiddenCapacity& operator=( const FireForbiddenCapacity& ); //!< Assignment operator
    //@}
};

BOOST_CLASS_EXPORT_KEY( FireForbiddenCapacity )

#endif // __FireForbiddenCapacity_h_
