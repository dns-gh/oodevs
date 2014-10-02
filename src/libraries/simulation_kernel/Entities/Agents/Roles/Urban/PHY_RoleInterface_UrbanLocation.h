// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __PHY_RoleInterface_UrbanLocation_h_
#define __PHY_RoleInterface_UrbanLocation_h_

#include "MT_Tools/Role_ABC.h"

class MIL_UrbanObject_ABC;
class MIL_Agent_ABC;
class MT_Ellipse;
class PHY_DotationCategory;
class PHY_MaterialCompositionType;
class TER_Polygon;

// =============================================================================
/** @class  PHY_RoleInterface_UrbanLocation
    @brief  PHY_RoleInterface_UrbanLocation
*/
// Created: SLG 2010-04-08
// =============================================================================
class PHY_RoleInterface_UrbanLocation : public tools::Role_ABC
{
public:
    typedef PHY_RoleInterface_UrbanLocation RoleInterface;

public:
    //! @name Constructors/Destructor
    //@{
             PHY_RoleInterface_UrbanLocation() {}
    virtual ~PHY_RoleInterface_UrbanLocation() {}
    //@}

    //! @name Operations
    //@{
    virtual void MagicMove( MT_Vector2D vPosition ) = 0;
    virtual double ComputeUrbanProtection( const PHY_DotationCategory& dotationCategory ) const = 0;
    virtual const PHY_MaterialCompositionType* GetUrbanMaterial() const = 0;
    virtual MT_Vector2D GetFirerPosition( const MT_Vector2D& target ) const = 0;
    virtual MT_Vector2D GetTargetPosition( MIL_Agent_ABC& target ) const = 0;
    virtual double ComputeDistanceInsideSameUrbanBlock( MIL_Agent_ABC& target  ) const = 0;
    virtual double ComputeRatioPionInside( const MT_Ellipse& attritionSurface ) const = 0;
    virtual double ComputeRatioPionInside( const TER_Polygon& polygon, double modificator ) const = 0;
    virtual void AddInhabitantCollision( unsigned int id ) = 0;
    virtual void RemoveInhabitantCollision( unsigned int id ) = 0;
    //@}

    //! @name Accessors
    //@{
    virtual const MIL_UrbanObject_ABC* GetCurrentUrbanBlock() const = 0;
    virtual bool IsInCity() const = 0;
    virtual bool CanMount() const = 0;
    virtual bool HasInhabitantCollision() const = 0;
    //@}
};

#endif // __PHY_RoleInterface_UrbanLocation_h_
