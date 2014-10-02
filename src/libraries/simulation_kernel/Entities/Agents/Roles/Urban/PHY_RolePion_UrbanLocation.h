// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __PHY_RolePion_UrbanLocation_h_
#define __PHY_RolePion_UrbanLocation_h_

#include "MIL.h"
#include "PHY_RoleInterface_UrbanLocation.h"
#include "ObjectCollisionNotificationHandler_ABC.h"
#include "MT_Tools/AlgorithmModifier_ABC.h"
#include "Entities/Agents/Actions/Flying/FlyListener_ABC.h"

namespace location
{
    class LocationComputer_ABC;
}

namespace moving
{
    class SpeedComputer_ABC;
}

class MIL_Object_ABC;
class UrbanBlockPosition_ABC;

// =============================================================================
/** @class  PHY_RolePion_UrbanLocation
    @brief  PHY_RolePion_UrbanLocation
*/
// Created: SLG 2010-04-08
// =============================================================================
class PHY_RolePion_UrbanLocation : public PHY_RoleInterface_UrbanLocation
                                 , public terrain::ObjectCollisionNotificationHandler_ABC
                                 , public tools::AlgorithmModifier_ABC< location::LocationComputer_ABC >
                                 , public tools::AlgorithmModifier_ABC< moving::SpeedComputer_ABC >
                                 , public FlyListener_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit PHY_RolePion_UrbanLocation( MIL_Agent_ABC& pion );
    virtual ~PHY_RolePion_UrbanLocation();
    //@}

    //! @name CheckPoints
    //@{
    template<class Archive>
    void serialize( Archive & ar, const unsigned int version )
    {
        boost::serialization::void_cast_register< PHY_RolePion_UrbanLocation, PHY_RoleInterface_UrbanLocation >();
        boost::serialization::split_member( ar, *this, version );
    }

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    virtual void MagicMove( MT_Vector2D vPosition );
    virtual double ComputeUrbanProtection( const PHY_DotationCategory& dotationCategory ) const;
    virtual const PHY_MaterialCompositionType* GetUrbanMaterial() const;
    virtual MT_Vector2D GetFirerPosition( const MT_Vector2D& target ) const;
    virtual MT_Vector2D GetTargetPosition( MIL_Agent_ABC& target ) const;
    virtual double ComputeDistanceInsideSameUrbanBlock( MIL_Agent_ABC& target ) const;
    virtual double ComputeRatioPionInside( const MT_Ellipse& attritionSurface ) const;
    virtual double ComputeRatioPionInside( const TER_Polygon& polygon, double modificator ) const;
    virtual void AddInhabitantCollision( unsigned int id );
    virtual void RemoveInhabitantCollision( unsigned int id );
    void Execute( moving::SpeedComputer_ABC& algorithm ) const;
    void Execute( location::LocationComputer_ABC& algorithm ) const;
    virtual bool CanMount() const;

    virtual void TakeOff();
    virtual void Land();
    //@}

    //! @name Event handlers
    //@{
    virtual void NotifyMovingInsideObject( MIL_Object_ABC& urbanObject, const MT_Vector2D& startPos, const MT_Vector2D& endPos );
    virtual void NotifyMovingOutsideObject( MIL_Object_ABC& urbanObject );
    virtual void NotifyPutInsideObject( MIL_Object_ABC& /*object*/ ) {}
    virtual void NotifyPutOutsideObject( MIL_Object_ABC& /*object*/ ) {}
    //@}

    //! @name Accessors
    //@{
    virtual const MIL_UrbanObject_ABC* GetCurrentUrbanBlock() const;
    virtual bool IsInCity() const;
    virtual bool HasInhabitantCollision() const;
    //@}

    //! @name Serialization
    //@{
    INTERNAL_BOOST_SAVE_LOAD_CONSTRUCT_DATA_HEADER( PHY_RolePion_UrbanLocation )
    //@}

private:
    //! @name Member data
    //@{
    MIL_Agent_ABC&                          owner_;
    const MIL_UrbanObject_ABC*               urbanObject_;
    std::unique_ptr< UrbanBlockPosition_ABC > delegate_;
    bool                                    isInCity_;
    bool                                    isFlying_;
    std::set< unsigned int >                collisions_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( PHY_RolePion_UrbanLocation )
INTERNAL_BOOST_SAVE_LOAD_CONSTRUCT_DATA( PHY_RolePion_UrbanLocation, MIL_Agent_ABC )

#endif // __PHY_RolePion_UrbanLocation_h_
