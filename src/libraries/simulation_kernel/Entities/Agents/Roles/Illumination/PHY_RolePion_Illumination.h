// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __PHY_RolePion_Illumination_h_
#define __PHY_RolePion_Illumination_h_

#include "PHY_RoleInterface_Illumination.h"

// =============================================================================
/** @class  PHY_RolePion_Illumination
    @brief  PHY_RolePion_Illumination
*/
// Created: MGD 2010-02-15
// =============================================================================
class PHY_RolePion_Illumination : public PHY_RoleInterface_Illumination
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit PHY_RolePion_Illumination();
    virtual ~PHY_RolePion_Illumination();
    //@}

    //! @name Operations
    //@{
    virtual bool IsIlluminating() const;
    virtual void NotifyStartIlluminate( const MIL_Entity_ABC& entity );
    virtual void NotifyStopIlluminate();

    virtual bool IsIlluminated() const;
    virtual bool IsDefinitevelyIlluminated() const;
    virtual void NotifyStartIlluminatedBy( const MIL_Entity_ABC& entity );
    virtual void NotifyStopIlluminatedBy( const MIL_Entity_ABC& entity );
    virtual void NotifyDefinitelyIlluminated();
    
    virtual bool IsUnderIndirectFire() const;
    virtual void NotifyHitByIndirectFire();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PHY_RolePion_Illumination( const PHY_RolePion_Illumination& );            //!< Copy constructor
    PHY_RolePion_Illumination& operator=( const PHY_RolePion_Illumination& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::set< const MIL_Entity_ABC* > illuminators_;
    bool bIlluminatedDefinitely_;
    bool bHit_;
    const MIL_Entity_ABC* target_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( PHY_RolePion_Illumination )

#endif // __PHY_RolePion_Illumination_h_
