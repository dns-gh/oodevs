// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __AltitudeModifierCapacity_h_
#define __AltitudeModifierCapacity_h_

#include "ObjectCapacity_ABC.h"

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  AltitudeModifierCapacity
    @brief  AltitudeModifierCapacity
*/
// Created: JSR 2011-05-17
// =============================================================================
class AltitudeModifierCapacity : public ObjectCapacity_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AltitudeModifierCapacity();
             AltitudeModifierCapacity( const AltitudeModifierCapacity& );
    explicit AltitudeModifierCapacity( xml::xistream& xis );
    virtual ~AltitudeModifierCapacity();
    //@}

    //! @name Operations
    //@{
    template< typename Archive >
    void serialize( Archive&, const unsigned int );
    virtual void Instanciate( MIL_Object_ABC& object ) const;
    virtual void Register( MIL_Object_ABC& object );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AltitudeModifierCapacity& operator=( const AltitudeModifierCapacity& ); //!< Assignment operator
    //@}
};

BOOST_CLASS_EXPORT_KEY( AltitudeModifierCapacity )

#endif // __AltitudeModifierCapacity_h_
