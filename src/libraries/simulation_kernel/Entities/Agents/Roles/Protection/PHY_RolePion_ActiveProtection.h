// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __PHY_RolePion_ActiveProtection_h_
#define __PHY_RolePion_ActiveProtection_h_

#include "MIL.h"
#include "PHY_RoleInterface_ActiveProtection.h"

class MIL_Agent_ABC;

// =============================================================================
/** @class  PHY_RolePion_ActiveProtection
    @brief  PHY_RolePion_ActiveProtection
*/
// Created: LDC 2010-01-11
// =============================================================================
class PHY_RolePion_ActiveProtection : public PHY_RoleInterface_ActiveProtection
{
public:
    //! @name Constructors/Destructor
    //@{
             PHY_RolePion_ActiveProtection();
    explicit PHY_RolePion_ActiveProtection( MIL_Agent_ABC& pion );
    virtual ~PHY_RolePion_ActiveProtection();
    //@}

    //! @name Operations
    //@{
    virtual void UseAmmunition( const PHY_DotationCategory& );
    virtual double GetPHModifier( const PHY_DotationCategory& ) const;
    virtual bool CounterIndirectFire( const PHY_DotationCategory& ) const;
    virtual bool DestroyIndirectFire( const PHY_DotationCategory& ) const;
    //@}

    //! @name Checkpoints
    //@{
    template< typename Archive > void serialize( Archive& file, const unsigned int );
    //@}

private:
    //! @name Member data
    //@{
    MIL_Agent_ABC* owner_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( PHY_RolePion_ActiveProtection )

#endif // __PHY_RolePion_ActiveProtection_h_
