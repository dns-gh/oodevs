// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __PHY_RoleInterface_ActiveProtection_h_
#define __PHY_RoleInterface_ActiveProtection_h_

#include "MT_Tools/Role_ABC.h"
#include <boost/serialization/access.hpp>

class PHY_DotationCategory;

// =============================================================================
/** @class  PHY_RoleInterface_ActiveProtection
    @brief  PHY_RoleInterface_ActiveProtection
*/
// Created: LDC 2010-01-07
// =============================================================================
class PHY_RoleInterface_ActiveProtection : public tools::Role_ABC
{
public:
    //! @name Types
    //@{
    typedef PHY_RoleInterface_ActiveProtection RoleInterface;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             PHY_RoleInterface_ActiveProtection() {}
    virtual ~PHY_RoleInterface_ActiveProtection() {}
    //@}

    //! @name Operations
    //@{
    /** Apply protection effects: Acquire knowledge, use dotations, etc. */
    virtual void UseAmmunition( const PHY_DotationCategory& ) = 0;
    virtual double GetPHModifier( const PHY_DotationCategory& ) const = 0;
    /** Avoid indirect fire for self only */
    virtual bool CounterIndirectFire( const PHY_DotationCategory& ) const = 0;
    /** Destroy incoming indirect fire */
    virtual bool DestroyIndirectFire( const PHY_DotationCategory& ) const = 0;
    //@}

private:
    //! @name Serialization
    //@{
    friend class boost::serialization::access;
    template< typename Archive > void serialize( Archive& ar, const unsigned int )
    {
        ar & boost::serialization::base_object< tools::Role_ABC >( *this );
    }
    //@}
};

#endif // __PHY_RoleInterface_ActiveProtection_h_
