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
#include "MIL_Random.h"

namespace posture
{
    class PostureComputer_ABC;
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
                                 , public tools::AlgorithmModifier_ABC< posture::PostureComputer_ABC >
                                 , public tools::AlgorithmModifier_ABC< moving::SpeedComputer_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit PHY_RolePion_UrbanLocation( MIL_Agent_ABC& pion );
    virtual ~PHY_RolePion_UrbanLocation();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    virtual void MagicMove( MT_Vector2D vPosition );
    virtual double ComputeUrbanProtection( const PHY_DotationCategory& dotationCategory ) const;
    virtual MT_Vector2D GetFirerPosition( MIL_Agent_ABC& target ) const;
    virtual MT_Vector2D GetTargetPosition( MIL_Agent_ABC& target ) const;
    virtual double ComputeDistanceInsideSameUrbanBlock( MIL_Agent_ABC& target ) const;
    virtual float ComputeRatioPionInside( const MT_Ellipse& attritionSurface ) const;
    virtual float ComputeRatioPionInside( const geometry::Polygon2f& polygon, float modificator ) const;
    void Execute( posture::PostureComputer_ABC& algorithm ) const;
    void Execute( moving::SpeedComputer_ABC& algorithm ) const;
    //@}

    //! @name Event handlers
    //@{
    virtual void NotifyMovingInsideObject( MIL_Object_ABC& urbanObject );
    virtual void NotifyMovingOutsideObject( MIL_Object_ABC& urbanObject );
    virtual void NotifyPutInsideObject( MIL_Object_ABC& /*object*/ ) {};
    virtual void NotifyPutOutsideObject( MIL_Object_ABC& /*object*/ ) {};
    //@}

    //! @name Accessors
    //@{
    virtual const UrbanObjectWrapper* GetCurrentUrbanBlock() const;
    virtual bool IsInCity() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PHY_RolePion_UrbanLocation( const PHY_RolePion_UrbanLocation& );            //!< Copy constructor
    PHY_RolePion_UrbanLocation& operator=( const PHY_RolePion_UrbanLocation& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    MIL_Agent_ABC& pion_;
    const UrbanObjectWrapper* urbanObject_;
    std::auto_ptr< UrbanBlockPosition_ABC > delegate_;
    bool isInCity_;
    //@}

    template< typename Archive > friend  void save_construct_data( Archive& archive, const PHY_RolePion_UrbanLocation* role, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, PHY_RolePion_UrbanLocation* role, const unsigned int /*version*/ );
};

#endif // __PHY_RolePion_UrbanLocation_h_
