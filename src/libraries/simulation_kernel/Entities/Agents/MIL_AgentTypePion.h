// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_AgentTypePion_h_
#define __MIL_AgentTypePion_h_

#include "MIL.h"
#include "MIL_AgentType_ABC.h"
#include <spatialcontainer/TerrainData.h>

namespace xml
{
    class xistream;
}

namespace sword
{
    class Brain;
    class RoleExtender_ABC;
}

class DEC_Model_ABC;
class MIL_Automate;
class MIL_AgentPion;
class PHY_UnitType;
class MIL_HumanRepartition;
class MIL_Config;
class MissionController_ABC;

// =============================================================================
// @class  MIL_AgentTypePion
// Created: JVT 2004-08-03
// =============================================================================
class MIL_AgentTypePion : public MIL_AgentType_ABC
{

public:
             MIL_AgentTypePion( const std::string& strName, const std::string& strType, xml::xistream& xis );
    virtual ~MIL_AgentTypePion();

    //! @name Manager
    //@{
    static void Initialize( xml::xistream& xis );
    static void Terminate ();

    static const MIL_AgentTypePion* Create( const std::string& strName, const std::string& strType, xml::xistream& xis );
    static const MIL_AgentTypePion* Find  ( const std::string& strName );
    static const MIL_AgentTypePion* Find  ( unsigned int nID );
    //@}

    //! @name Instantiation
    //@{
    virtual MIL_AgentPion* InstanciatePion( MissionController_ABC& controller, MIL_Automate& automate, xml::xistream& xis ) const;

    virtual void RegisterRoles( MIL_AgentPion& pion, sword::RoleExtender_ABC* ext ) const;
    //@}

    //! @name Accessors
    //@{
    virtual const PHY_UnitType&               GetUnitType                      () const;
    const DEC_Model_ABC&                      GetModel                         () const;
          double                            GetDistanceAvantPoint            ( const TerrainData& nType ) const;
          double                            GetDistanceAvantLima             () const;
          double                            GetFeedbackTime() const;
          const MIL_HumanRepartition&       GetHumanRepartition              () const;
    void SetBrainFunctions( const std::vector< std::string >& names );
    //@}

    //! @name Operators
    //@{
    bool operator==( const MIL_AgentTypePion& rhs ) const;
    //@}

    //! @name Operations
    //@{
    virtual void RegisterFunctions( sword::Brain& brain, MIL_Agent_ABC& agent ) const;
    //@}

protected:
    //! @name // $$$$ _RC_ SBO 2011-05-20: only for test purpose...
    //@{
    explicit MIL_AgentTypePion( const DEC_Model_ABC* pModel );
    template< typename T >
    MIL_AgentTypePion( const std::string& strName, xml::xistream& xis, T* pUnitTypeType /*Used only to determine T type*/ );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, const MIL_AgentTypePion* > T_PionTypeMap;
    typedef T_PionTypeMap::const_iterator                   CIT_PionTypeMap;

    typedef std::map< TerrainData, double >             T_DistanceAvantPointMap;
    typedef T_DistanceAvantPointMap::const_iterator     CIT_DistanceAvantPointMap;
    //@}

private:
    //! @name Init
    //@{
    void InitializeRapFor              ( xml::xistream& xis );
    void InitializeDistancesAvantPoints( xml::xistream& xis );
    void InitializeModel               ( xml::xistream& xis );
    //@}
    //! @name Helpers
    //@{
    static void ReadUnit ( xml::xistream& xis );
    void ReadPoint       ( xml::xistream& xis );
    void ReadFeedback    ( xml::xistream& xis );
    //@}

private:
    const DEC_Model_ABC*    pModel_;
    const PHY_UnitType*     pUnitType_;
    T_DistanceAvantPointMap distancesAvantPoints_;
    double                  rDistanceAvantLimas_;
    double                  rFeedbackTime_;
    MIL_HumanRepartition*   pHumanRepartition_;
    std::vector< std::string > functions_;

private:
    static T_PionTypeMap           pionTypes_;
};

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion constructor
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
template< typename T >
MIL_AgentTypePion::MIL_AgentTypePion( const std::string& strName, xml::xistream& xis, T* )
    : MIL_AgentType_ABC( strName, xis )
    , pModel_( 0 )
    , pUnitType_( new T( xis ) )
    , rDistanceAvantLimas_( 0. )
    , rFeedbackTime_( 0. )
    , pHumanRepartition_( new MIL_HumanRepartition( xis ) )
    , isNBC_ ( false )
{
    InitializeRapFor              ( xis );
    InitializeDistancesAvantPoints( xis );
    InitializeModel               ( xis );
}

#endif // __MIL_AgentTypePion_h_
