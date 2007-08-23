// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_ObjectManager.h $
// $Author: Nld $
// $Modtime: 10/06/05 10:18 $
// $Revision: 6 $
// $Workfile: MIL_ObjectManager.h $
//
// *****************************************************************************

#ifndef __MIL_ObjectManager_h_
#define __MIL_ObjectManager_h_

#include "MIL.h"

#include "game_asn/Asn.h"

namespace xml
{
    class xistream;
}

class MIL_Army;
class MIL_RealObjectType;
class TER_Localisation;
class MIL_VirtualObject_ABC;
class MIL_RealObject_ABC;
class MIL_NbcAgentType;
class MIL_ObstacleType;

class MIL_ControlZone;
class MIL_NuageNBC;
class MIL_ZoneMineeParDispersion;

// =============================================================================
// @class  MIL_ObjectManager
// Created: JVT 2004-08-03
// =============================================================================
class MIL_ObjectManager
{
    MT_COPYNOTALLOWED( MIL_ObjectManager )

public:
     MIL_ObjectManager();
    ~MIL_ObjectManager();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}
    
    //! @name Operations
    //@{
    void ProcessEvents();
    void UpdateStates ();

    void                        CreateObject                       ( xml::xistream& xis, MIL_Army& army ); 
    MIL_RealObject_ABC*         CreateObject                       ( MIL_Army& army, const MIL_ObstacleType& obstacleType, DIA_Parameters& diaParameters, uint nCurrentParamIdx );
    MIL_RealObject_ABC*         CreateObject                       ( const MIL_RealObjectType& type, MIL_Army& army, const TER_Localisation& localisation, const std::string& strOption, const std::string& strExtra, double rCompletion, double rMining, double rBypass );
    MIL_NuageNBC&               CreateObjectNuageNBC               ( MIL_Army& army, const TER_Localisation& localisation, const MIL_NbcAgentType& nbcAgentType );
    MIL_ControlZone&            CreateObjectControlZone            ( MIL_Army& army, const TER_Localisation& localisation, MT_Float rRadius );
    MIL_ZoneMineeParDispersion& CreateObjectZoneeMineeParDispersion( MIL_Army& army, const TER_Localisation& localisation, uint nNbrMines );
    MIL_RealObject_ABC*         FindRealObject                      ( uint nID ) const;
    //@}

    //! @name Network
    //@{
    void OnReceiveMsgObjectMagicAction( const ASN1T_MsgObjectMagicAction& asnMsg, uint nCtx );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< uint, MIL_RealObject_ABC* > T_RealObjectMap;
    typedef T_RealObjectMap::iterator             IT_RealObjectMap;
    typedef T_RealObjectMap::const_iterator       CIT_RealObjectMap;

    typedef std::vector< MIL_VirtualObject_ABC* > T_VirtualObjectVector;
    typedef T_VirtualObjectVector::iterator       IT_VirtualObjectVector;
    typedef T_VirtualObjectVector::const_iterator CIT_VirtualObjectVector;
    //@}

private:
    //! @name Tools
    //@{
    ASN1T_EnumObjectErrorCode CreateObject  ( const ASN1T_MagicActionCreateObject& asn );
    void                      RegisterObject( MIL_VirtualObject_ABC& object );
    void                      RegisterObject( MIL_RealObject_ABC& object    );
    //@}

private:
    T_RealObjectMap       realObjects_;
    T_VirtualObjectVector virtualObjects_;
};

#include "MIL_ObjectManager.inl"

#endif // __MIL_ObjectManager_h_
