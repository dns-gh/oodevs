// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PerceptionCapacity_h_
#define __PerceptionCapacity_h_

#include "ObjectCapacity_ABC.h"
#include "MIL_InteractiveContainer_ABC.h"

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  PerceptionCapacity
    @brief  PerceptionCapacity
*/
// Created: MGD 2010-03-12
// =============================================================================
class PerceptionCapacity : public ObjectCapacity_ABC
                         , public MIL_InteractiveContainer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             PerceptionCapacity();
    explicit PerceptionCapacity( xml::xistream& xis );
    virtual ~PerceptionCapacity();
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
    PerceptionCapacity( const PerceptionCapacity& from );
    //@}

    //! @name Attribute
    //@
    bool modifier_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( PerceptionCapacity )

#endif
