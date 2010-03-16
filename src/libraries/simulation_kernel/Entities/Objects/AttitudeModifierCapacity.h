// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Math�matiques Appliqu�es SA (MASA)
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
    virtual void Instanciate( Object& object ) const;
    virtual void Register( Object& object );
    virtual void ProcessPopulationInside ( Object& object, MIL_PopulationElement_ABC& population ); 
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