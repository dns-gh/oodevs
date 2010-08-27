// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __SealOffCapacity_h_
#define __SealOffCapacity_h_

#include "ObjectCapacity_ABC.h"

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  SealOffCapacity
    @brief  SealOffCapacity
*/
// Created: MGD 2010-08-24
// =============================================================================
class SealOffCapacity : public ObjectCapacity_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             SealOffCapacity();
             SealOffCapacity( const SealOffCapacity& );
    explicit SealOffCapacity( xml::xistream& xis );
    virtual ~SealOffCapacity();
    //@}

     //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Operations
    //@{
    virtual void Instanciate( MIL_Object_ABC& object ) const;
    virtual void Register( MIL_Object_ABC& object );
    virtual void Update( MIL_Object_ABC& object, unsigned int time );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SealOffCapacity& operator=( const SealOffCapacity& ); //!< Assignment operator
    //@}
};

BOOST_CLASS_EXPORT_KEY( SealOffCapacity )

#endif // __SealOffCapacity_h_
