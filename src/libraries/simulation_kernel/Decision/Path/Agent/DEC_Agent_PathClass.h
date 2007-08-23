// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-08-04 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifndef __DEC_PathClass_h_
#define __DEC_PathClass_h_

#include "MIL.h"

namespace xml
{
    class xistream;
}

class DEC_PathType;
class MIL_AgentPion;
class MIL_RealObjectType;
class MIL_PopulationAttitude;

// =============================================================================
/** @class  DEC_Agent_PathClass
    @brief  DEC_Agent_PathClass
*/
// Created: AGE 2005-08-04
// =============================================================================
class DEC_Agent_PathClass
{
public:
    //! @name Manager
    //@{
    static       void                 Initialize  ( xml::xistream& xis );
    static       void                 Terminate   ();
    static const DEC_Agent_PathClass& GetPathClass( const DEC_PathType& pathType, const MIL_AgentPion& pion );
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    DEC_Agent_PathClass( xml::xistream& xis, const DEC_Agent_PathClass* pCopyFrom = 0 );
    virtual ~DEC_Agent_PathClass();
    //@}

    //! @name Accessors
    //@{
          MT_Float     GetPreferedTerrainCost              () const;
    const TerrainData& GetPreferedTerrain                  () const;
          MT_Float     GetAvoidedTerrainCost               () const;
    const TerrainData& GetAvoidedTerrain                   () const;

          MT_Float     GetAltitudePreference               () const;

          MT_Float     GetComfortFuseauDistance            () const;
          MT_Float     GetFuseauCostPerMeterOut            () const;
          MT_Float     GetFuseauCostPerMeterIn             () const;
          MT_Float     GetMaximumFuseauDistance            () const;
          MT_Float     GetMaximumFuseauDistanceWithAutomata() const;

          MT_Float     GetAutomateFuseauCostPerMeterOut    () const;
          MT_Float     GetMaximumAutomataFuseauDistance    () const;

          MT_Float     GetDangerDirectionBaseCost          () const;
          MT_Float     GetDangerDirectionLinearCost        () const;

          bool         AvoidObjects                        () const;
          bool         AvoidEnemies                        () const;
          bool         HandlePopulations                   () const;

          MT_Float     GetEnemyCostOnContact               () const;
          MT_Float     GetEnemyCostAtSecurityRange         () const;
          MT_Float     GetEnemyMaximumCost                 () const;
          MT_Float     GetObjectCost                       ( const MIL_RealObjectType& objectType ) const;
          MT_Float     GetPopulationAttitudeCost           ( const MIL_PopulationAttitude& attitude ) const;
          MT_Float     GetPopulationSecurityRange          () const;
          MT_Float     GetPopulationMaximumCost            () const;
          MT_Float     GetCostOutsideOfPopulation          () const;

          bool         IsShort                             () const;
    //@}

private:
    //! @name Initialization
    //@{
    void ReadFuseau           ( xml::xistream& xis );
    void ReadAutomataFuseau   ( xml::xistream& xis );
    void ReadDangerDirection  ( xml::xistream& xis );
    void ReadEnemiesCost      ( xml::xistream& xis );
    void ReadPrefferedTerrains( xml::xistream& xis, TerrainData& data );
    void ReadAvoidedTerrain   ( xml::xistream& xis, TerrainData& data );
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
    struct LoadingWrapper;
    static void ReadUnitRule( xml::xistream& xis );
    void ReadObject         ( xml::xistream& xis );
    void ReadTerrain        ( xml::xistream& xis, TerrainData& data );
    void ReadPopulation     ( xml::xistream& xis );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< MT_Float >                              T_ObjectCosts;

    typedef std::pair< bool /*bFlying*/, bool /*, bAutonomous*/> T_BooleanPair;
    typedef std::pair< std::string, T_BooleanPair >              T_RuleType;
    typedef std::map< T_RuleType, DEC_Agent_PathClass* >         T_Rules;

    typedef std::map< const MIL_PopulationAttitude*, MT_Float >  T_PopulationAttitudeCosts;
    //@}

private:
    //! @name Member data
    //@{
    bool          bShort_;

    bool          bAvoidObjects_;
    T_ObjectCosts objectCosts_;

    MT_Float    rAltitudePreference_;

    MT_Float    rPreferedTerrainCost_;
    TerrainData preferedTerrain_;
    MT_Float    rAvoidedTerrainCost_;
    TerrainData avoidedTerrain_;

    MT_Float rMaximumFuseauDistance_;
    MT_Float rMaximumFuseauDistanceWithAutomata_;
    MT_Float rComfortFuseauDistance_;
    MT_Float rFuseauCostPerMeterOut_;
    MT_Float rFuseauCostPerMeterIn_;

    MT_Float rMaximumAutomataFuseauDistance_;
    MT_Float rAutomataFuseauCostPerMeterOut_;

    MT_Float rDangerDirectionBaseCost_;
    MT_Float rDangerDirectionLinearCost_;

    MT_Float rEnemyCostOnContact_;
    MT_Float rEnemyCostAtSecurityRange_;
    MT_Float rEnemyMaximumCost_;

    MT_Float rPopulationSecurityRange_;
    MT_Float rPopulationMaximumCost_;
    MT_Float rCostOutsideOfPopulation_;

    T_PopulationAttitudeCosts populationAttitudeCosts_;
    //@}

private:
    static T_Rules rules_;
};

#include "DEC_Agent_PathClass.inl"

#endif // __DEC_PathClass_h_
