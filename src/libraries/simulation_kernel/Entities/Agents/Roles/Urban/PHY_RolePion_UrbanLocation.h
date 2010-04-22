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
#include "UrbanBlockCollisionNotificationHandler_ABC.h"
#include "MT_Tools/AlgorithmModifier_ABC.h"
#include "MT_Tools/MT_Random.h"

namespace posture
{
    class PostureComputer_ABC;
}

// =============================================================================
/** @class  PHY_RolePion_UrbanLocation
    @brief  PHY_RolePion_UrbanLocation
*/
// Created: SLG 2010-04-08
// =============================================================================
class PHY_RolePion_UrbanLocation : public PHY_RoleInterface_UrbanLocation
                                 , public terrain::UrbanBlockCollisionNotificationHandler_ABC
                                 , public tools::AlgorithmModifier_ABC< posture::PostureComputer_ABC >
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
    void                        MagicMove( MT_Vector2D vPosition );
    virtual MT_Float            ComputeUrbanProtection( const PHY_DotationCategory& dotationCategory ) const;
    virtual geometry::Point2f   GetFirerPosition( MIL_Agent_ABC& target ) const;
    virtual geometry::Point2f   GetTargetPosition( MIL_Agent_ABC& target ) const;
    virtual float               ComputeDistanceInsideSameUrbanBlock( MIL_Agent_ABC& target ) const;
    void                        Execute( posture::PostureComputer_ABC& algorithm ) const;

    //@}

    //! @name Event handlers
    //@{
    virtual void NotifyMovingInsideUrbanBlock( const urban::TerrainObject_ABC& urbanObject );
    virtual void NotifyMovingOutsideUrbanBlock( const urban::TerrainObject_ABC& urbanObject );
    //@}

    //! @name Accessors
    //@{
    virtual const urban::TerrainObject_ABC* GetCurrentUrbanBlock() const;
    //@}

    //! @name Network
    //@{
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PHY_RolePion_UrbanLocation( const PHY_RolePion_UrbanLocation& );            //!< Copy constructor
    PHY_RolePion_UrbanLocation& operator=( const PHY_RolePion_UrbanLocation& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    geometry::Point2f GetNearestUrbanBlockPoint( const geometry::Point2f firerPosition, const std::vector< geometry::Point2f > points ) const;
    geometry::Point2f GetFurthestUrbanBlockPoint( const geometry::Point2f firerPosition, const std::vector< geometry::Point2f > points ) const;
    //@}

private:
    //! @name Member data
    //@{
    MIL_Agent_ABC& pion_;
    const urban::TerrainObject_ABC* urbanObject_;
    //@}

    template< typename Archive > friend  void save_construct_data( Archive& archive, const PHY_RolePion_UrbanLocation* role, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, PHY_RolePion_UrbanLocation* role, const unsigned int /*version*/ );

private:
    static MT_Random randomGenerator_;

};

#endif // __PHY_RolePion_UrbanLocation_h_
