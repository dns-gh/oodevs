// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Orders/Automate/MIL_PopulationMissionType.h $
// $Author: Nld $
// $Modtime: 10/03/05 16:10 $
// $Revision: 3 $
// $Workfile: MIL_PopulationMissionType.h $
//
// *****************************************************************************

#ifndef __MIL_PopulationMissionType_h_
#define __MIL_PopulationMissionType_h_

#include "MIL.h"
#include "Network/NET_ASN_Types.h"

class MIL_PopulationMission_ABC;
class MIL_Population;

// =============================================================================
// @class  MIL_PopulationMissionType
// Created: JVT 2004-08-03
// =============================================================================
class MIL_PopulationMissionType
{
    MT_COPYNOTALLOWED( MIL_PopulationMissionType )

public:
    //! @name Manager
    //@{
    template< typename Mission > static 
    void RegisterMission( const std::string& strName, uint nID, uint nAsnID, const std::string& strDIATypeName, const std::string& strBehavior );

    static void InitializeDIA();
    static void Terminate    ();

    static const MIL_PopulationMissionType* FindPopulationMissionType( const std::string& strName );
    static const MIL_PopulationMissionType* FindPopulationMissionType( uint nID );
    static const MIL_PopulationMissionType* FindPopulationMissionType( const ASN1T_Mission_Population& asnMission );
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
    MIL_PopulationMission_ABC& InstanciateMission( MIL_Population& population ) const;
    bool operator==( const MIL_PopulationMissionType& rhs ) const;
    //@}

private:
    typedef std::vector< const MIL_PopulationMissionType* > T_MissionFromIDVector;
    typedef T_MissionFromIDVector::const_iterator         CIT_MissionFromIDVector;

    typedef std::map< uint /*asn*/, const MIL_PopulationMissionType* > T_MissionFromAsnIDMap;
    typedef T_MissionFromAsnIDMap::const_iterator                    CIT_MissionFromAsnIDMap;

    typedef std::map< std::string, const MIL_PopulationMissionType* > T_MissionFromNameMap;
    typedef T_MissionFromNameMap::const_iterator                    CIT_MissionFromNameMap;

    typedef MIL_PopulationMission_ABC&(* T_MissionAllocator )( MIL_Population& population, const MIL_PopulationMissionType& type );
    typedef void(* T_DiaInitializer )( const MIL_PopulationMissionType& type );
    //@}

private:
     MIL_PopulationMissionType( const std::string& strName, uint nID, uint nAsnID, const std::string& strDIATypeName, const std::string& strBehavior, T_MissionAllocator missionAllocator, T_DiaInitializer diaInitializer );
    ~MIL_PopulationMissionType();

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

#include "MIL_PopulationMissionType.inl"

#endif // __MIL_PopulationMissionType_h_
