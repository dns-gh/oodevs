// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef PHY_PerceptionDisaster_h
#define PHY_PerceptionDisaster_h

#include "PHY_Perception_ABC.h"

// =============================================================================
/** @class  PHY_PerceptionDisaster
    @brief  PHY perception disaster
*/
// Created: LGY 2012-12-06
// =============================================================================
class PHY_PerceptionDisaster : public PHY_Perception_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit PHY_PerceptionDisaster( PHY_RoleInterface_Perceiver& perceiver );
    virtual ~PHY_PerceptionDisaster();
    //@}

    //! @name Operations
    //@{
    virtual void Execute( const TER_Object_ABC::T_ObjectVector& perceivableObjects );
    virtual const PHY_PerceptionLevel& Compute( const DEC_Knowledge_Object& knowledge ) const;
    virtual void FinalizePerception();
    //@}

private:
    //! @name Helpers
    //@{
    const PHY_PerceptionLevel& Compute( const MIL_Object_ABC& object, const MT_Vector2D& position ) const;
    //@}
};

#endif // PHY_PerceptionDisaster_h
