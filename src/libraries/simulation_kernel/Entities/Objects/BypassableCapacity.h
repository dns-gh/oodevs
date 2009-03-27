// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __BypassableCapacity_h_
#define __BypassableCapacity_h_

#include "ObjectCapacity_ABC.h"

class BypassableCapacity : public ObjectCapacity_ABC
{
public:
	//! @name Constructors/Destructor
    //@{
	explicit BypassableCapacity( xml::xistream& xis );
             BypassableCapacity();
    virtual ~BypassableCapacity();
	//@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    virtual void Register( Object& /*object*/ );
    //@}


	//! @name virtual
    //@{
    virtual void Instanciate( Object& object ) const;	
    //@}

    //! @name 
    //@{
    void Bypass( Object& object, MT_Float rDeltaPercentage );
    //@}

private:
    //! @name Copy
    //@{
    BypassableCapacity( const BypassableCapacity& );
    //@}

private:
    //! @name Type
    //@{
    uint bypassSpeed_;
    //@}
};

#endif // __BypassableCapacity_h_