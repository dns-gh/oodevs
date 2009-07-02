// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/MIL_AgentTypePion.h $
// $Author: Jvt $
// $Modtime: 10/05/05 16:36 $
// $Revision: 10 $
// $Workfile: MIL_AgentTypePion.h $
//
// *****************************************************************************

#ifndef __MIL_AgentTypePion_h_
#define __MIL_AgentTypePion_h_

#include "MIL.h"

#include "MIL_AgentType_ABC.h"

namespace xml
{
    class xistream;
}

class PHY_UnitType;
class DEC_Model_ABC;
class MIL_Automate;
class MIL_AgentPion;
class MIL_RealObjectType;

// =============================================================================
// @class  MIL_AgentTypePion
// Created: JVT 2004-08-03
// =============================================================================
class MIL_AgentTypePion : public MIL_AgentType_ABC
{

public:
             MIL_AgentTypePion( const std::string& strName, xml::xistream& xis );
    virtual ~MIL_AgentTypePion();

    //! @name Manager
    //@{
    static void Initialize( xml::xistream& xis );
    static void Terminate ();

    static const MIL_AgentTypePion* Create( const std::string& strName, xml::xistream& xis );
    static const MIL_AgentTypePion* Find  ( const std::string& strName );
    static const MIL_AgentTypePion* Find  ( uint nID );
    //@}

    //! @name Instanciation
    //@{
    virtual MIL_AgentPion& InstanciatePion( uint nID, MIL_Automate& automate, xml::xistream& xis ) const;
    virtual MIL_AgentPion& InstanciatePion( uint nID, MIL_Automate& automate, const MT_Vector2D& vPosition ) const; 
    //@}

    //! @name Accessors
    //@{
    const PHY_UnitType&                       GetUnitType                      () const;
    const DEC_Model_ABC&                      GetModel                         () const;
          DIA_FunctionTable< MIL_AgentPion >& GetFunctionTable                 () const;
          MT_Float                            GetDistanceAvantPoint            ( const TerrainData& nType ) const;
          MT_Float                            GetDistanceAvantLima             () const;
          MT_Float                            GetRapForIncreasePerTimeStepValue() const;
    //@}

    //! @name Operators
    //@{
    bool operator==( const MIL_AgentTypePion& rhs ) const;
    //@}

protected:
    template< typename T >
        MIL_AgentTypePion( const std::string& strName, xml::xistream& xis, T* pUnitTypeType /*Used only to determine T type*/ );

private:
    //! @name Types
    //@{
    typedef std::map< std::string, const MIL_AgentTypePion*, sCaseInsensitiveLess > T_PionTypeMap;
    typedef T_PionTypeMap::const_iterator                                           CIT_PionTypeMap;

    typedef const MIL_AgentTypePion* (*T_PionTypeAllocator)( const std::string& strName, xml::xistream& xis );

    typedef std::map< std::string, T_PionTypeAllocator, sCaseInsensitiveLess > T_PionTypeAllocatorMap;
    typedef T_PionTypeAllocatorMap::const_iterator                             CIT_PionTypeAllocatorMap;

    typedef std::map< TerrainData, MT_Float >             T_DistanceAvantPointMap;
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
    MT_Float                rDistanceAvantLimas_;
    MT_Float                rRapForIncreasePerTimeStepValue_;

    DIA_FunctionTable< MIL_AgentPion >* pDIAFunctionTable_;

private:
    static T_PionTypeAllocatorMap  pionTypeAllocators_;
    static T_PionTypeMap           pionTypes_;
};

#include "MIL_AgentTypePion.inl"

#endif // __MIL_AgentTypePion_h_
