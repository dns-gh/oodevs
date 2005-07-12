// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Orders/Automate/MIL_AutomateMissionType.h $
// $Author: Nld $
// $Modtime: 10/03/05 16:10 $
// $Revision: 3 $
// $Workfile: MIL_AutomateMissionType.h $
//
// *****************************************************************************

#ifndef __MIL_AutomateMissionType_h_
#define __MIL_AutomateMissionType_h_

#include "MIL.h"
#include "Network/NET_ASN_Types.h"

class MIL_AutomateMission_ABC;
class MIL_Automate;

// =============================================================================
// @class  MIL_AutomateMissionType
// Created: JVT 2004-08-03
// =============================================================================
class MIL_AutomateMissionType
{
    MT_COPYNOTALLOWED( MIL_AutomateMissionType )

public:
    //! @name Manager
    //@{
    template< typename Mission > static 
    void RegisterMission( const std::string& strName, uint nID, uint nAsnID, const std::string& strDIATypeName, const std::string& strMrtBehavior, const std::string& strConduiteBehavior );

    static void Initialize   ( MIL_InputArchive& archive );
    static void InitializeDIA();
    static void Terminate    ();

    static const MIL_AutomateMissionType* FindAutomateMissionType        ( const std::string& strName );
    static const MIL_AutomateMissionType* FindAutomateMissionType        ( uint nID );
    static const MIL_AutomateMissionType* FindAutomateMissionType        ( const ASN1T_Mission_Automate& asnMission );
    static const MIL_AutomateMissionType& GetSurrenderingMissionType     ();
    static const MIL_AutomateMissionType& GetMoveToRefugeeCampMissionType();
    //@}

    //! @name Accessors
    //@{
          uint         GetID                  () const;
    const std::string& GetDIATypeName         () const;
    const std::string& GetMrtBehaviorName     () const;
    const std::string& GetConduiteBehaviorName() const;
    const std::string& GetName                () const;
    //@}

    //! @name Operations
    //@{
    MIL_AutomateMission_ABC& InstanciateMission( MIL_Automate& automate ) const;
    bool operator==( const MIL_AutomateMissionType& rhs ) const;
    //@}

private:
    typedef std::vector< const MIL_AutomateMissionType* > T_MissionFromIDVector;
    typedef T_MissionFromIDVector::const_iterator         CIT_MissionFromIDVector;

    typedef std::map< uint /*asn*/, const MIL_AutomateMissionType* > T_MissionFromAsnIDMap;
    typedef T_MissionFromAsnIDMap::const_iterator                    CIT_MissionFromAsnIDMap;

    typedef std::map< std::string, const MIL_AutomateMissionType* > T_MissionFromNameMap;
    typedef T_MissionFromNameMap::const_iterator                    CIT_MissionFromNameMap;

    typedef MIL_AutomateMission_ABC&(* T_MissionAllocator )( MIL_Automate& automate, const MIL_AutomateMissionType& type );
    typedef void(* T_DiaInitializer )( const MIL_AutomateMissionType& type );
    //@}

private:
     MIL_AutomateMissionType( const std::string& strName, uint nID, uint nAsnID, const std::string& strDIATypeName, const std::string& strMrtBehavior, const std::string& strConduiteBehavior, T_MissionAllocator missionAllocator, T_DiaInitializer diaInitializer );
    ~MIL_AutomateMissionType();

private:
    const std::string        strName_;
    const uint               nID_;
    const uint               nAsnID_;
    const std::string        strDIATypeName_;
    const std::string        strMrtBehavior_;
    const std::string        strConduiteBehavior_;
    const T_MissionAllocator missionAllocator_;
    const T_DiaInitializer   missionDiaInitializer_;

private:
    static       T_MissionFromIDVector     missionsFromID_;
    static       T_MissionFromAsnIDMap     missionsFromAsnID_;
    static       T_MissionFromNameMap      missionsFromName_;
    static const MIL_AutomateMissionType*  pSurrenderingMissionType_;
    static const MIL_AutomateMissionType*  pMoveToRefugeeCampMissionType_;
};

#include "MIL_AutomateMissionType.inl"

#endif // __MIL_AutomateMissionType_h_
