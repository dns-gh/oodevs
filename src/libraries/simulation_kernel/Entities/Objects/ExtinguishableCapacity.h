// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ExtinguishableCapacity_h_
#define __ExtinguishableCapacity_h_

#include "ObjectCapacity_ABC.h"

// =============================================================================
/** @class  ExtinguishableCapacity
    @brief  ExtinguishableCapacity
*/
// Created: JCR 2008-05-30
// =============================================================================
class ExtinguishableCapacity : public ObjectCapacity_ABC
{
public:
	//! @name Constructors/Destructor
    //@{
	         ExtinguishableCapacity();
    explicit ExtinguishableCapacity( xml::xistream& xis );
    virtual ~ExtinguishableCapacity();
	//@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}

	//! @name Operations
    //@{
    virtual void Instanciate( Object& object ) const;
    virtual void Register( Object& object );
    void Extinguish( Object& object, int extinguisherAgent, int numberOfFireHoses );
    //@}

private:
    //! @name Copy
    //@{
    ExtinguishableCapacity( const ExtinguishableCapacity& );
    //@}
};

#endif // __ExtinguishableCapacity_h_