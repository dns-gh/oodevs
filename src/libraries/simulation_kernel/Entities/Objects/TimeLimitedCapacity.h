// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TimeLimitedCapacity_h_
#define __TimeLimitedCapacity_h_

#include "simulation_kernel/MIL.h"
#include "ObjectCapacity_ABC.h"
#include "MIL_InteractiveContainer_ABC.h"

class Object;

class TimeLimitedCapacity
    : public ObjectCapacity_ABC
{
public:
	//! @name Constructors/Destructor
    //@{
    explicit TimeLimitedCapacity( xml::xistream& xis );
             TimeLimitedCapacity();
    virtual ~TimeLimitedCapacity();
	//@}

    //! @name CheckPoints
    //@{    
    template< typename Archive > void serialize( Archive&, const uint );
    virtual void Register( Object& /*object*/ );
    //@}

	//! @name virtual
    //@{
    virtual void Instanciate( Object& object ) const;
	virtual void Update( Object& object, uint time );
	//@}

private:
    //! @name Copy
    //@{
    TimeLimitedCapacity( const TimeLimitedCapacity& from );
    //@}
};

#endif