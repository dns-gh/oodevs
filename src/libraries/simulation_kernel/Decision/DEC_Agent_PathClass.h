// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DEC_Agent_PathClass_h_
#define __DEC_Agent_PathClass_h_

#include <spatialcontainer/TerrainData.h>
#include "DEC_PathClass.h"
#include "DEC_Agent_PathClass_ABC.h"

namespace xml
{
    class xistream;
}

class DEC_PathType;
class MIL_Agent_ABC;
class MIL_ObjectType_ABC;
class MIL_PopulationAttitude;

// =============================================================================
/** @class  DEC_Agent_PathClass
    @brief  DEC Agent path class
*/
// Created: AGE 2005-08-04
// =============================================================================
class DEC_Agent_PathClass : public DEC_PathClass, public DEC_Agent_PathClass_ABC
{
public:
    //! @name Manager
    //@{
    static       void                 Initialize  ( xml::xistream& xis, const std::vector< unsigned int >& dangerousObjects );
    static       void                 Terminate   ();
    static const DEC_Agent_PathClass& GetPathClass( const DEC_PathType& pathType, const MIL_Agent_ABC& pion );
    static const DEC_Agent_PathClass& GetPathClass( const DEC_PathType& pathType, bool bCanFly, bool bAutonomous );
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             DEC_Agent_PathClass( xml::xistream& xis, const std::vector< unsigned int >& dangerousObjects, const DEC_Agent_PathClass* pCopyFrom = 0 );
    virtual ~DEC_Agent_PathClass();
    //@}

    //! @name Accessors
    //@{
          double       GetPreferedTerrainCost              () const;
    const TerrainData& GetPreferedTerrain                  () const;
          double       GetAvoidedTerrainCost               () const;
    const TerrainData& GetAvoidedTerrain                   () const;

          double       GetAltitudePreference               () const;

          double       GetComfortFuseauDistance            () const;
          double       GetFuseauCostPerMeterOut            () const;
          double       GetFuseauCostPerMeterIn             () const;
          double       GetMaximumFuseauDistance            () const;
          double       GetMaximumFuseauDistanceWithAutomata() const;

          double       GetAutomateFuseauCostPerMeterOut    () const;
          double       GetMaximumAutomataFuseauDistance    () const;

          double       GetDangerDirectionBaseCost          () const;
          double       GetDangerDirectionLinearCost        () const;

          bool         AvoidObjects                        () const;
          bool         AvoidEnemies                        () const;
          bool         HandlePopulations                   () const;

          double       GetEnemyCostOnContact               () const;
          double       GetEnemyCostAtSecurityRange         () const;
          double       GetEnemyMaximumCost                 () const;
          double       GetObjectCost                       ( const MIL_ObjectType_ABC& objectType ) const;
          double       GetPopulationMaximumCost            () const;
          double       GetThreshold                        () const;

          virtual double GetPopulationAttitudeCost ( unsigned int attitudeID ) const;
          virtual double GetPopulationSecurityRange() const;
          virtual double GetCostOutsideOfPopulation() const;

          bool         IsShort                             () const;
          bool         IsFlying                            () const;
    //@}

private:
    //! @name Initialization
    //@{
    void ReadFuseau           ( xml::xistream& xis );
    void ReadAutomataFuseau   ( xml::xistream& xis );
    void ReadDangerDirection  ( xml::xistream& xis );
    void ReadEnemiesCost      ( xml::xistream& xis );
    void ReadPopulationsCost  ( xml::xistream& xis );
    void ReadObjectsCost      ( xml::xistream& xis );
    //@}

private:
    //! @name Initialization
    //@{
    static void CheckRulesExistence();
    //@}

    //! @name Helpers
    //@{
    static void ReadUnitRule( xml::xistream& xis, const std::vector< unsigned int >& dangerousObjects );
    void ReadObject         ( xml::xistream& xis );
    void ReadTerrain        ( xml::xistream& xis, TerrainData& data );
    void ReadPopulation     ( xml::xistream& xis );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< double >                              T_ObjectCosts;

    typedef std::pair< bool /*bFlying*/, bool /*, bAutonomous*/> T_BooleanPair;
    typedef std::pair< std::string, T_BooleanPair >              T_RuleType;
    typedef std::map< T_RuleType, DEC_Agent_PathClass* >         T_Rules;

    typedef std::map< unsigned int, double >  T_PopulationAttitudeCosts;
    //@}

private:
    //! @name Member data
    //@{
    bool bShort_;

    bool bAvoidObjects_;
    bool bFlying_;
    double rObstructionThreshold_;
    T_ObjectCosts objectCosts_;

    double rAltitudePreference_;

    double rPreferedTerrainCost_;
    TerrainData preferedTerrain_;
    double rAvoidedTerrainCost_;
    TerrainData avoidedTerrain_;

    double rMaximumFuseauDistance_;
    double rMaximumFuseauDistanceWithAutomata_;
    double rComfortFuseauDistance_;
    double rFuseauCostPerMeterOut_;
    double rFuseauCostPerMeterIn_;

    double rMaximumAutomataFuseauDistance_;
    double rAutomataFuseauCostPerMeterOut_;

    double rDangerDirectionBaseCost_;
    double rDangerDirectionLinearCost_;

    double rEnemyCostOnContact_;
    double rEnemyCostAtSecurityRange_;
    double rEnemyMaximumCost_;

    double rPopulationSecurityRange_;
    double rPopulationMaximumCost_;
    double rCostOutsideOfPopulation_;

    T_PopulationAttitudeCosts populationAttitudeCosts_;
    //@}

private:
    static T_Rules rules_;
};

#endif // __DEC_Agent_PathClass_h_
