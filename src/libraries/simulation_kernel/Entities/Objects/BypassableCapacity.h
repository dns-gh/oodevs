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

// =============================================================================
/** @class  BypassableCapacity
    @brief  BypassableCapacity
*/
// Created: JCR 2008-05-30
// =============================================================================
class BypassableCapacity : public ObjectCapacity_ABC
{
public:
	//! @name Constructors/Destructor
    //@{
             BypassableCapacity();
    explicit BypassableCapacity( xml::xistream& xis );
    virtual ~BypassableCapacity();
	//@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}

	//! @name Operations
    //@{
    virtual void Instanciate( Object& object ) const;
    virtual void Register( Object& object );
    void Bypass( Object& object, MT_Float rDeltaPercentage );
    //@}

private:
    //! @name Member data
    //@{
    uint bypassSpeed_;
    //@}
};

#endif // __BypassableCapacity_h_