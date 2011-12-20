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

class PHY_RolePion_Composantes;

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
    explicit PHY_RolePion_ActiveProtection( PHY_RolePion_Composantes& pion );
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
    //! @name Serialization
    //@{
    INTERNAL_BOOST_SAVE_LOAD_CONSTRUCT_DATA_HEADER( PHY_RolePion_ActiveProtection )
    //@}

private:
    //! @name Member data
    //@{
    PHY_RolePion_Composantes& owner_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( PHY_RolePion_ActiveProtection )
INTERNAL_BOOST_SAVE_LOAD_CONSTRUCT_DATA( PHY_RolePion_ActiveProtection, PHY_RolePion_Composantes )

#endif // __PHY_RolePion_ActiveProtection_h_
