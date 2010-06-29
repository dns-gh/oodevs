// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ScatteringCapacity_h_
#define __ScatteringCapacity_h_

#include "ObjectCapacity_ABC.h"
#include "MIL_InteractiveContainer_ABC.h"

// =============================================================================
/** @class  ScatteringCapacity
    @brief  ScatteringCapacity
*/
// Created: MGD 2010-03-12
// =============================================================================
class ScatteringCapacity : public ObjectCapacity_ABC
                         , public MIL_InteractiveContainer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ScatteringCapacity();
    explicit ScatteringCapacity( xml::xistream& xis );
    virtual ~ScatteringCapacity();
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
    ScatteringCapacity( const ScatteringCapacity& from );
    //@}

    //! @name Attribute
    //@
    float humanByTimeStep_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( ScatteringCapacity )

#endif
