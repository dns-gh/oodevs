// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
#ifndef __MIL_PopulationFlow_h_
#define __MIL_PopulationFlow_h_

#include "MIL.h"
#include "MIL_PopulationElement_ABC.h"
#include "Entities/Actions/PHY_MovingEntity_ABC.h"
#include "simulation_terrain/TER_Localisation.h"
#include "simulation_terrain/TER_PopulationFlow_ABC.h"
#include "Tools/MIL_IDManager.h"
#include <boost/shared_ptr.hpp>

class MIL_PopulationConcentration;
class DEC_Population_Path;

// =============================================================================
// Created: NLD 2005-09-28
// =============================================================================
class MIL_PopulationFlow : public PHY_MovingEntity_ABC
                         , public TER_PopulationFlow_ABC
                         , public MIL_PopulationElement_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_PopulationFlow( MIL_Population& population, const MIL_PopulationFlow& source, const MT_Vector2D& splitPoint );
             MIL_PopulationFlow( MIL_Population& population, MIL_PopulationConcentration& sourceConcentration );
    virtual ~MIL_PopulationFlow();
    //@}

    //! @name Operations
    //@{
    bool Update();
    void Clean ();
    virtual bool IsValid() const; // false = will be deleted
    virtual bool CanBePerceived() const;
    //@}

    //! @name Actions
    //@{
    void MagicMove( const MT_Vector2D& destination );
    void Move( const MT_Vector2D& destination );
    void MoveAlong( const std::vector< boost::shared_ptr< MT_Vector2D > >& destination );
    void CancelMove();
    virtual bool IsReady() const;
    MIL_PopulationFlow* Split( const MT_Vector2D& splittingPoint, std::size_t segmentIndex );
    //@}

    //! @name Accessors
    //@{
    virtual const MT_Vector2D& GetPosition() const;
    virtual const MT_Vector2D& GetDirection() const;
    virtual const TER_Localisation& GetLocation() const;
    double GetSpeed() const;
    bool CanCollideWithFlow() const;
    bool CanCollideWith( MIL_PopulationFlow* flow ) const;
    virtual boost::shared_ptr< MT_Vector2D > GetSafetyPosition( const MIL_AgentPion& agent, double rMinDistance, double rSeed ) const;
    virtual double GetDefaultDensity( const MIL_PopulationType& type ) const;
    virtual bool Intersect2DWithCircle( const MT_Vector2D& vCircleCenter, double rRadius, std::vector< MT_Vector2D >& shape ) const;
    const T_PointList& GetFlowShape() const;
    void ApplyOnShape( const boost::function< bool( const MT_Line& ) >& f ) const;
    //@}

    //! @name Concentration management
    //@{
    void UnregisterSourceConcentration();
    //@}

    //! @name Network
    //@{
    void SendCreation( unsigned int context = 0 ) const;
    void SendFullState() const;
    void SendChangedState() const;
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

protected:
    MIL_PopulationFlow( MIL_Population& population, unsigned int nID );

private:
    //! @name Types
    //@{
    typedef std::vector< std::pair< MIL_PopulationFlow*, MT_Vector2D > > T_FlowCollisions;
    typedef std::list< std::pair< MT_Vector2D, std::size_t > > T_FlowShape;
    //@}

    //! @name Helpers
    //@{
    virtual double GetMaxSpeed() const;
    virtual double GetSpeed( const TerrainData& environment ) const;
    virtual double GetSpeed( const TerrainData& environment, const MIL_Object_ABC& object ) const;
    virtual double GetSlopeDeceleration() const;
    virtual double GetMaxSlope() const;
    //@}

    //! @name  Position operations
    //@{
    virtual void ApplyMove( const MT_Vector2D& position, const MT_Vector2D& direction, double rSpeed, double rWalkedDistance );
    void UpdateTailPosition( const double rWalkedDistance );
    const MT_Vector2D& GetHeadPosition() const;
    const MT_Vector2D& GetTailPosition() const;
    void SetHeadPosition( const MT_Vector2D& position );
    void SetTailPosition( const MT_Vector2D& position );
    void SetDirection( const MT_Vector2D& direction );
    void SetSpeed( const double rSpeed );
    void UpdateLocation();
    void UpdateCrowdCollisions();
    bool ComputeFlowCollisions( const MT_Line& line, T_FlowCollisions& collisions );
    bool AddFlowCollision( const MT_Line& line, const MT_Line& flowSegment, T_FlowCollisions& collisions );
    void ComputeSpeedLimit();
    //@}

    //! @name Notifications
    //@{
    virtual void NotifyMovingOnPathPoint( const MT_Vector2D& point );
    virtual void NotifyMovingOnSpecialPoint( boost::shared_ptr< DEC_PathPoint > point );
    virtual void NotifyMovingInsideObject( MIL_Object_ABC& object );
    virtual void NotifyMovingOutsideObject( MIL_Object_ABC& object );
    virtual void NotifyEnvironmentChanged();
    virtual void NotifyCurrentPathChanged();
    virtual void NotifyCollision( MIL_Agent_ABC& agent );
    //@}

    //! @name Helpers
    //@{
    virtual bool CanMove() const;
    virtual bool CanObjectInteractWith( const MIL_Object_ABC& object ) const;
    virtual bool HasKnowledgeObject( const MIL_Object_ABC& object ) const;
    virtual double GetObjectCost( const MIL_ObjectType_ABC& objectType, const DEC_PathType& pathType ) const;
    virtual bool HasResources();
    virtual void SendRC( const MIL_DecisionalReport& reportId ) const;
    virtual void SendRC( const MIL_DecisionalReport& reportId, const std::string& name ) const;
    bool ManageObjectSplit();
    bool ManageSplit();
    void MoveToAlternateDestination( const MT_Vector2D& destination );
    void ComputePath( const MT_Vector2D& destination );
    void ComputePathAlong( const std::vector< boost::shared_ptr< MT_Vector2D > >& headDestination, const std::vector< boost::shared_ptr< MT_Vector2D > >& tailDestination );
    void DetachFromDestConcentration();
    T_FlowShape::const_iterator FindPointInShape( const MT_Vector2D& v ) const;
    //@}

    //! @name Serialization
    //@{
    template< typename Archive > friend  void save_construct_data( Archive& archive, const MIL_PopulationFlow* flow, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, MIL_PopulationFlow* flow, const unsigned int /*version*/ );
    //@}

    //! @name Network
    //@{
    void SendDestruction() const;
    bool HasChanged     () const;
    //@}

private:
    //! @name Member data
    //@{
    MIL_PopulationConcentration* pSourceConcentration_;
    MIL_PopulationConcentration* pDestConcentration_;
    MT_Vector2D primaryDestination_;
    MT_Vector2D alternateDestination_; // Used when the flow is splitted
    boost::shared_ptr< DEC_Population_Path > pHeadPath_;
    boost::shared_ptr< DEC_Population_Path > pTailPath_;
    MT_Vector2D direction_;
    double rSpeed_;
    T_FlowShape flowShape_; 
    mutable T_PointList computedFlowShape_;
    std::vector< MT_Vector2D > pointsToInsert_;
    TER_Localisation location_; // For terrain
    bool bHeadMoveFinished_;
    // Network
    bool bPathUpdated_;
    bool bFlowShapeUpdated_;
    bool bDirectionUpdated_;
    bool bSpeedUpdated_;
    bool bBlocked_;
    bool canCollideWithFlow_;
    bool hasDoneMagicMove_;
    // Split
    const MIL_Object_ABC* pSplittingObject_;
    const MIL_Object_ABC* pBlockingObject_;
    const MIL_Object_ABC* pFirstSplittingObject_;
    double armedIndividualsBeforeSplit_;
    unsigned int personsPassedThroughObject_;
    double speedLimit_;
    static MIL_IDManager idManager_;
    double objectDensity_;
    std::vector< boost::shared_ptr< MT_Vector2D > > moveAlongPath_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_PopulationFlow )

#endif // __MIL_PopulationFlow_h_
