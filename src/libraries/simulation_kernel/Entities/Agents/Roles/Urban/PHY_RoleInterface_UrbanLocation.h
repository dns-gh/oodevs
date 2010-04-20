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
#include <geometry/Types.h>

namespace posture
{
    class PostureComputer_ABC;
}
namespace urban
{
    class TerrainObject_ABC;
}

class PHY_DotationCategory;

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
    virtual MT_Float            ComputeUrbanProtection( const PHY_DotationCategory& dotationCategory ) const = 0;
    virtual geometry::Point2f   GetFirerPosition( const geometry::Point2f firerPosition, const geometry::Point2f targetPosition ) const = 0;
    virtual geometry::Point2f   GetTargetPosition( const geometry::Point2f firerPosition, const geometry::Point2f targetPosition ) const = 0;
    virtual float               ComputeDistanceInsideSameUrbanBlock( const geometry::Point2f firerPosition, const geometry::Point2f targetPosition, float targetDeployment  ) const = 0;
    //@}


    //! @name Accessors
    //@{
    virtual const urban::TerrainObject_ABC* GetCurrentUrbanBlock() const = 0;
    virtual float GetDeployment() const = 0;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PHY_RoleInterface_UrbanLocation( const PHY_RoleInterface_UrbanLocation& );            //!< Copy constructor
    PHY_RoleInterface_UrbanLocation& operator=( const PHY_RoleInterface_UrbanLocation& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __PHY_RoleInterface_UrbanLocation_h_
