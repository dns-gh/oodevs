// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_PopulationConcentration_h_
#define __MIL_PopulationConcentration_h_

#include "MIL.h"
#include "MIL_PopulationElement_ABC.h"
#include "simulation_terrain/TER_Localisation.h"
#include "simulation_terrain/TER_PopulationConcentration_ABC.h"
#include "Tools/MIL_IDManager.h"

namespace xml
{
    class xistream;
}

class MIL_Population;
class MIL_PopulationFlow;
class MIL_Object_ABC;
class MIL_AttackController;

// =============================================================================
// Created: NLD 2005-09-28
// =============================================================================
class MIL_PopulationConcentration : public MIL_PopulationElement_ABC
                                  , public TER_PopulationConcentration_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_PopulationConcentration();
             MIL_PopulationConcentration( MIL_Population& population, unsigned int id );
             MIL_PopulationConcentration( MIL_Population& population, xml::xistream& xis );
             MIL_PopulationConcentration( MIL_Population& population, const MT_Vector2D& position, unsigned int nHumans = 0 );
             MIL_PopulationConcentration( MIL_Population& population, const MT_Vector2D& position, const MIL_PopulationHumans& humans );
    virtual ~MIL_PopulationConcentration();
    //@}

    //! @name Operations
    //@{
    bool Update(); // Return false if the concentration must be deleted
    virtual void Clean();
    bool IsValid() const; // false = will be deleted
    virtual bool CanBePerceived() const;
    //@}

    //! @name Actions
    //@{
    void MagicMove( const MT_Vector2D& destination );
    void Move( const MT_Vector2D& destination );
    void Attack();
    //@}

    //! @name Flows management
    //@{
    void RegisterPushingFlow( MIL_PopulationFlow& flow );
    void UnregisterPushingFlow( MIL_PopulationFlow& flow );
    double GetPullingFlowsDensity() const;
    void SetPullingFlowsDensity( const MIL_Object_ABC& splittingObject );
    //@}

    //! @name Accessors
    //@{
    const MIL_Object_ABC* GetSplittingObject() const;
    const MT_Vector2D& GetPosition() const;
    bool IsNearPosition( const MT_Vector2D& position ) const;
    virtual const TER_Localisation& GetLocation() const;
    virtual boost::shared_ptr< MT_Vector2D > GetSecuringPoint( const MIL_Agent_ABC& securingAgent ) const;
    virtual boost::shared_ptr< MT_Vector2D > GetSafetyPosition( const MIL_AgentPion& agent, double rMinDistance, double rSeed ) const;
    virtual double GetDefaultDensity( const MIL_PopulationType& type ) const;
    virtual bool Intersect2DWithCircle( const MT_Vector2D& vCircleCenter, double rRadius, std::vector< MT_Vector2D >& shape ) const;
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

private:
    //! @name Tools
    //@{
    void NotifyCollision( MIL_Agent_ABC& agent );
    void UpdateLocation();
    //@}

    //! @name Network
    //@{
    void SendDestruction() const;
    bool HasChanged() const;
    //@}

private:
    //! @name Member data
    //@{
    MT_Vector2D position_;
    TER_Localisation location_;
    MIL_PopulationFlow* pPullingFlow_;
    std::set< MIL_PopulationFlow* > pushingFlows_;
    const MIL_Object_ABC* pSplittingObject_;
    std::auto_ptr< MIL_AttackController > pAttackController_;
    double rPullingFlowsDensity_;
    bool hasDoneMagicMove_;
    static MIL_IDManager idManager_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_PopulationConcentration )

#endif // __MIL_PopulationConcentration_h_
