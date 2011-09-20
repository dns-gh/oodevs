// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __TrafficabilityCapacity_h_
#define __TrafficabilityCapacity_h_

#include "ObjectCapacity_ABC.h"

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  TrafficabilityCapacity
    @brief  Trafficability capacity
*/
// Created: LGY 2011-09-20
// =============================================================================
class TrafficabilityCapacity : public ObjectCapacity_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             TrafficabilityCapacity();
    explicit TrafficabilityCapacity( xml::xistream& xis );
    virtual ~TrafficabilityCapacity();
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
    TrafficabilityCapacity( const TrafficabilityCapacity& );            //!< Copy constructor
    TrafficabilityCapacity& operator=( const TrafficabilityCapacity& ); //!< Assignment operator
    //@}

private:
    //! @name Member Data
    //@{
    bool default_;
    float maxWeight_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( TrafficabilityCapacity )

#endif // __TrafficabilityCapacity_h_
