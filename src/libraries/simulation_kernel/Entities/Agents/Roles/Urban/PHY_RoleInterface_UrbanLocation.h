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
#include "MT_Tools/MT_Ellipse.h"
#include <geometry/Types.h>

namespace posture
{
    class PostureComputer_ABC;
}

class UrbanObjectWrapper;
class PHY_DotationCategory;
class MIL_Agent_ABC;

// =============================================================================
/** @class  PHY_RoleInterface_UrbanLocation
    @brief  PHY_RoleInterface_UrbanLocation
*/
// Created: SLG 2010-04-08
// =============================================================================
class PHY_RoleInterface_UrbanLocation : public tools::Role_ABC
                                      , private boost::noncopyable
{
public:
    typedef PHY_RoleInterface_UrbanLocation RoleInterface;

public:
    //! @name Constructors/Destructor
    //@{
             PHY_RoleInterface_UrbanLocation(){}
    virtual ~PHY_RoleInterface_UrbanLocation(){}
    //@}

    //! @name Operations
    //@{
    virtual void MagicMove( MT_Vector2D vPosition ) = 0;
    virtual double ComputeUrbanProtection( const PHY_DotationCategory& dotationCategory ) const = 0;
    virtual MT_Vector2D GetFirerPosition( MIL_Agent_ABC& target ) const = 0;
    virtual MT_Vector2D GetTargetPosition( MIL_Agent_ABC& target ) const = 0;
    virtual double ComputeDistanceInsideSameUrbanBlock( MIL_Agent_ABC& target  ) const = 0;
    virtual float ComputeRatioPionInside( const MT_Ellipse& attritionSurface ) const = 0;
    virtual float ComputeRatioPionInside( const geometry::Polygon2f& polygon, float modificator ) const = 0;
    //@}

    //! @name Accessors
    //@{
    virtual const UrbanObjectWrapper* GetCurrentUrbanBlock() const = 0;
    virtual bool IsInCity() const = 0;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PHY_RoleInterface_UrbanLocation( const PHY_RoleInterface_UrbanLocation& );            //!< Copy constructor
    PHY_RoleInterface_UrbanLocation& operator=( const PHY_RoleInterface_UrbanLocation& ); //!< Assignment operator
    //@}
};

#endif // __PHY_RoleInterface_UrbanLocation_h_
