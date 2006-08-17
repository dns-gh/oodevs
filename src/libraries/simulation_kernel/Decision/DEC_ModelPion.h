// *****************************************************************************
//
// $Created: NLD 2003-11-24 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/DEC_ModelPion.h $
// $Author: Mco $
// $Modtime: 11/02/05 16:14 $
// $Revision: 2 $
// $Workfile: DEC_ModelPion.h $
//
// *****************************************************************************

#ifndef __DEC_ModelPion_h_
#define __DEC_ModelPion_h_

#include "MIL.h"

#include "DEC_Model_ABC.h"

class MIL_OrderConduiteType;
class MIL_PionMissionType;

// =============================================================================
// Created: NLD 2003-11-24
// =============================================================================
class DEC_ModelPion : public DEC_Model_ABC
{
    MT_COPYNOTALLOWED( DEC_ModelPion );

public:
    DEC_ModelPion( const DEC_Workspace& decWorkspace, const std::string& strName, MIL_InputArchive& input, bool bNeedParsing, const std::string& strArchivePath, const std::string& strSourcePath );
    virtual ~DEC_ModelPion();

    //! @name Missions management
    //@{
    bool IsMissionAvailable                ( const MIL_PionMissionType& missionType ) const;
    bool IsOrderConduiteAvailableForMission( const MIL_PionMissionType& missionType, const MIL_OrderConduiteType& orderConduiteType ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< bool >                                       T_IDBitVector;
    typedef std::map< const MIL_PionMissionType*, T_IDBitVector >     T_OrderConduitePerMissionMap;
    typedef T_OrderConduitePerMissionMap::const_iterator              CIT_OrderConduitePerMissionMap;
    //@}

private:;
    //! @name Init
    //@{
    void InitializeMissions             ( MIL_InputArchive& archive );
    void InitializeMissionOrdresConduite( MIL_InputArchive& archive, const MIL_PionMissionType& missionType );
    //@}

private:
    T_IDBitVector                 missionBitset_;
    T_OrderConduitePerMissionMap  orderConduitePerMissionMap_;
};

#include "DEC_ModelPion.inl"

#endif // __DEC_ModelPion_h_
