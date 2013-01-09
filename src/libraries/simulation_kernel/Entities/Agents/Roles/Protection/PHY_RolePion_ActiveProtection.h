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
             PHY_RolePion_ActiveProtection();
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
    //! @name Copy/Assignment
    //@{
    PHY_RolePion_ActiveProtection( const PHY_RolePion_ActiveProtection& );            //!< Copy constructor
    PHY_RolePion_ActiveProtection& operator=( const PHY_RolePion_ActiveProtection& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    PHY_RolePion_Composantes* pion_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( PHY_RolePion_ActiveProtection )

#endif // __PHY_RolePion_ActiveProtection_h_
