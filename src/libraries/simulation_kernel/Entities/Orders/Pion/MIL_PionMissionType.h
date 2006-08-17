// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/entities/orders/pion/MIL_PionMissionType.h $
// $Author: Nld $
// $Modtime: 4/03/05 17:34 $
// $Revision: 2 $
// $Workfile: MIL_PionMissionType.h $
//
// *****************************************************************************

#ifndef __MIL_PionMissionType_h_
#define __MIL_PionMissionType_h_

#include "MIL.h"

#include "Network/NET_ASN_Types.h"

class MIL_PionMission_ABC;
class MIL_AgentPion;

// =============================================================================
// @class  MIL_PionMissionType
// Created: JVT 2004-08-03
// =============================================================================
class MIL_PionMissionType
{
    MT_COPYNOTALLOWED( MIL_PionMissionType )

public:
    //! @name Manager
    //@{
    template< typename Mission > static 
    void RegisterMission( const std::string& strName, uint nID, uint nAsnID, const std::string& strDIATypeName, const std::string& strBehavior );

    static void InitializeDIA();
    static void Terminate ();

    static const MIL_PionMissionType* FindPionMissionType( const std::string& strName );
    static const MIL_PionMissionType* FindPionMissionType( uint nID );
    static const MIL_PionMissionType* FindPionMissionType( const ASN1T_Mission_Pion& asnMission );
    //@}

    //! @name Accessors
    //@{
          uint         GetID          () const;
    const std::string& GetDIATypeName () const;
    const std::string& GetBehaviorName() const;
    const std::string& GetName        () const;
    //@}

    //! @name Operations
    //@{
    MIL_PionMission_ABC& InstanciateMission( MIL_AgentPion& pion ) const;
    bool operator==( const MIL_PionMissionType& rhs ) const;
    //@}

private:

    //! @name Types
    //@{
    typedef std::vector< const MIL_PionMissionType* > T_MissionFromIDVector;
    typedef T_MissionFromIDVector::const_iterator      CIT_MissionFromIDVector;

    typedef std::map< uint /*asn*/, const MIL_PionMissionType* > T_MissionFromAsnIDMap;
    typedef T_MissionFromAsnIDMap::const_iterator                 CIT_MissionFromAsnIDMap;

    typedef std::map< std::string, const MIL_PionMissionType* > T_MissionFromNameMap;
    typedef T_MissionFromNameMap::const_iterator                CIT_MissionFromNameMap;

    typedef MIL_PionMission_ABC&(* T_MissionAllocator )( MIL_AgentPion& pion, const MIL_PionMissionType& type );
    typedef void(* T_DiaInitializer )( const MIL_PionMissionType& type );
    //@}

private:
     MIL_PionMissionType( const std::string& strName, uint nID, uint nAsnID, const std::string& strDIATypeName, const std::string& strBehavior, T_MissionAllocator missionAllocator, T_DiaInitializer diaInitializer );
    ~MIL_PionMissionType();

private:
    const std::string        strName_;
    const uint               nID_;
    const uint               nAsnID_;
    const std::string        strDIATypeName_;
    const std::string        strBehavior_;
    const T_MissionAllocator missionAllocator_;
    const T_DiaInitializer   missionDiaInitializer_;

private:
    static T_MissionFromIDVector missionsFromID_;
    static T_MissionFromAsnIDMap missionsFromAsnID_;
    static T_MissionFromNameMap  missionsFromName_;
};

#include "MIL_PionMissionType.inl"

#endif // __MIL_PionMissionType_h_
