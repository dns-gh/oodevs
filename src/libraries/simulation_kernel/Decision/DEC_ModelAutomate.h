// *****************************************************************************
//
// $Created: NLD 2003-11-24 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/DEC_ModelAutomate.h $
// $Author: Mco $
// $Modtime: 11/02/05 16:11 $
// $Revision: 2 $
// $Workfile: DEC_ModelAutomate.h $
//
// *****************************************************************************

#ifndef __DEC_ModelAutomate_h_
#define __DEC_ModelAutomate_h_

#include "MIL.h"

#include "DEC_Model_ABC.h"

class MIL_AutomateMissionType;
class MIL_OrderConduiteType;

// =============================================================================
// Created: NLD 2003-11-24
// =============================================================================
class DEC_ModelAutomate : public DEC_Model_ABC
{
    MT_COPYNOTALLOWED( DEC_ModelAutomate );

public:
    DEC_ModelAutomate( const DEC_Workspace& decWorkspace, const std::string& strName, MIL_InputArchive& input, bool bNeedParsing, const std::string& strArchivePath, const std::string& strSourcePath );
    virtual ~DEC_ModelAutomate();

    //! @name Missions management
    //@{
    bool IsMissionAvailable                ( const MIL_AutomateMissionType& missionType ) const;
    bool IsOrderConduiteAvailableForMission( const MIL_AutomateMissionType& missionType, const MIL_OrderConduiteType& orderConduiteType ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< bool >                                       T_IDBitVector;
    typedef std::map< const MIL_AutomateMissionType*, T_IDBitVector > T_OrderConduitePerMissionMap;
    typedef T_OrderConduitePerMissionMap::const_iterator              CIT_OrderConduitePerMissionMap;
    //@}

private:;
    //! @name Init
    //@{
    void InitializeMissions             ( MIL_InputArchive& archive );
    void InitializeMissionOrdresConduite( MIL_InputArchive& archive, const MIL_AutomateMissionType& missionType );
    //@}

private:
    T_IDBitVector                 missionBitset_;
    T_OrderConduitePerMissionMap  orderConduitePerMissionMap_;
};

#include "DEC_ModelAutomate.inl"

#endif // __DEC_ModelAutomate_h_
