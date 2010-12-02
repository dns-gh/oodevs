// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_AgentTypePion_h_
#define __MIL_AgentTypePion_h_

#include "MIL.h"
#include "MIL_AgentType_ABC.h"
#include "MT_Tools/MT_Stl.h"
#include <pathfind/TerrainData.h>

namespace xml
{
    class xistream;
}

namespace directia
{
namespace brain
{
    class Brain;
}
}

class AlgorithmsFactories;
class DEC_Model_ABC;
class MIL_Automate;
class MIL_AgentPion;
class PHY_UnitType;


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
    virtual MIL_AgentPion* InstanciatePion( MIL_Automate& automate, const AlgorithmsFactories& algorithmFactories, xml::xistream& xis ) const;
    virtual MIL_AgentPion* InstanciatePion( MIL_Automate& automate, const AlgorithmsFactories& algorithmFactories ) const;
    virtual void RegisterRoles( MIL_AgentPion& pion, unsigned int gcPause, unsigned int gcMult ) const;
    //@}

    //! @name Accessors
    //@{
    virtual const PHY_UnitType&               GetUnitType                      () const;
    const DEC_Model_ABC&                      GetModel                         () const;
          double                            GetDistanceAvantPoint            ( const TerrainData& nType ) const;
          double                            GetDistanceAvantLima             () const;
          double                            GetRapForIncreasePerTimeStepValue() const;
    //@}

    //! @name Operators
    //@{
    bool operator==( const MIL_AgentTypePion& rhs ) const;
    //@}

    //! @name Operations
    //@{
    virtual void RegisterFunctions( directia::brain::Brain& brain, MIL_Agent_ABC& agent ) const;
    //@}

protected:
    explicit MIL_AgentTypePion( const DEC_Model_ABC* pModel );
    template< typename T >
        MIL_AgentTypePion( const std::string& strName, xml::xistream& xis, T* pUnitTypeType /*Used only to determine T type*/ );

private:
    //! @name Types
    //@{
    typedef std::map< std::string, const MIL_AgentTypePion*, sCaseInsensitiveLess > T_PionTypeMap;
    typedef T_PionTypeMap::const_iterator                                           CIT_PionTypeMap;

    typedef const MIL_AgentTypePion* (*T_PionTypeAllocator)( const std::string& strName, const std::string& strType, xml::xistream& xis );

    typedef std::map< std::string, T_PionTypeAllocator, sCaseInsensitiveLess > T_PionTypeAllocatorMap;
    typedef T_PionTypeAllocatorMap::const_iterator                             CIT_PionTypeAllocatorMap;

    typedef std::map< TerrainData, double >             T_DistanceAvantPointMap;
    typedef T_DistanceAvantPointMap::const_iterator     CIT_DistanceAvantPointMap;
    //@}

private:
    //! @name Init
    //@{
    void InitializeRapFor              ( xml::xistream& xis );
    void InitializeDistancesAvantPoints( xml::xistream& xis );
    void InitializeModel               ( xml::xistream& xis );
    void InitializeDiaFunctions        ();
    //@}
    //! @name Helpers
    //@{
    struct LoadingWrapper;
    static void ReadUnit ( xml::xistream& xis );
    void ReadPoint       ( xml::xistream& xis );
    void ReadFeedback    ( xml::xistream& xis );
    //@}

private:
    const DEC_Model_ABC*    pModel_;
    const PHY_UnitType*     pUnitType_;
    T_DistanceAvantPointMap distancesAvantPoints_;
    double                rDistanceAvantLimas_;
    double                rRapForIncreasePerTimeStepValue_;

private:
    static T_PionTypeAllocatorMap  pionTypeAllocators_;
    static T_PionTypeMap           pionTypes_;
};

#include "MIL_AgentTypePion.inl"

#endif // __MIL_AgentTypePion_h_
