// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __UniversalCapacity_h_
#define __UniversalCapacity_h_

#include "ObjectCapacity_ABC.h"

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  UniversalCapacity
    @brief  UniversalCapacity
*/
// Created: JSR 2011-01-06
// =============================================================================
class UniversalCapacity : public ObjectCapacity_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             UniversalCapacity();
    explicit UniversalCapacity( xml::xistream& xis );
    virtual ~UniversalCapacity();
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
    UniversalCapacity( const UniversalCapacity& );            //!< Copy constructor
    UniversalCapacity& operator=( const UniversalCapacity& ); //!< Assignment operator
    //@}
};

BOOST_CLASS_EXPORT_KEY( UniversalCapacity )

#endif // __UniversalCapacity_h_
