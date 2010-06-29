// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AttitudeModifierCapacity_h_
#define __AttitudeModifierCapacity_h_

#include "ObjectCapacity_ABC.h"
#include "MIL_InteractiveContainer_ABC.h"

class MIL_PopulationAttitude;


// =============================================================================
/** @class  AttitudeModifierCapacity
    @brief  AttitudeModifierCapacity
*/
// Created: MGD 2010-03-12
// =============================================================================
class AttitudeModifierCapacity : public ObjectCapacity_ABC
                           , public MIL_InteractiveContainer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             AttitudeModifierCapacity();
    explicit AttitudeModifierCapacity( xml::xistream& xis );
    virtual ~AttitudeModifierCapacity();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    virtual void Instanciate( MIL_Object_ABC& object ) const;
    virtual void Register( MIL_Object_ABC& object );
    virtual void ProcessPopulationInside ( MIL_Object_ABC& object, MIL_PopulationElement_ABC& population );
    //@}

private:
    //! @name Copy
    //@{
    AttitudeModifierCapacity( const AttitudeModifierCapacity& from );
    //@}

    //! @name Attribute
    const MIL_PopulationAttitude* attitude_;
};

BOOST_CLASS_EXPORT_KEY( AttitudeModifierCapacity )

#endif