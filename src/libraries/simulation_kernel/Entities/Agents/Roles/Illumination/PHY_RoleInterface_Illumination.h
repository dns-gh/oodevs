// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
// LTO
//
// *****************************************************************************

#ifndef __PHY_RoleInterface_Illumination_h_
#define __PHY_RoleInterface_Illumination_h_

#include "MT_Tools/Role_ABC.h"

class MIL_Entity_ABC;

// =============================================================================
/** @class  PHY_RoleInterface_Illumination
    @brief  PHY_RoleInterface_Illumination
*/
// Created: MGD 2010-02-15
// =============================================================================
class PHY_RoleInterface_Illumination : public tools::Role_ABC
{
public:
    //! @name Types
    //@{
    typedef PHY_RoleInterface_Illumination RoleInterface;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             PHY_RoleInterface_Illumination() {}
    virtual ~PHY_RoleInterface_Illumination() {}
    //@}

    //! @name Operations
    //@{
    virtual bool IsIlluminating() const = 0;
    virtual void NotifyStartIlluminate( const MIL_Entity_ABC& entity ) = 0;//action state can be sufficient
    virtual void NotifyStopIlluminate() = 0;

    virtual bool IsIlluminated() const = 0;
    virtual bool IsDefinitevelyIlluminated() const = 0;
    virtual void NotifyStartIlluminatedBy( const MIL_Entity_ABC& entity ) = 0;
    virtual void NotifyStopIlluminatedBy( const MIL_Entity_ABC& entity ) = 0;
    virtual void NotifyDefinitelyIlluminated() = 0;

    virtual bool IsUnderIndirectFire() const = 0;
    virtual void NotifyHitByIndirectFire() = 0;
    //@}

    //! @name Serialization
    //@{
    template< typename Archive > void serialize( Archive& ar, const unsigned int )
    {
        ar & boost::serialization::base_object< tools::Role_ABC >( *this );
    }
    //@}
};

#endif // __PHY_RoleInterface_Illumination_h_
