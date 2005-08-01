// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: $TIME$ $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MOS_MissionPion_Skeleton.h $
// $Author: Nld $
// $Modtime: 18/10/04 13:04 $
// $Revision: 3 $
// $Workfile: AGR_MOS_MissionPion_Skeleton.h $
//
// *****************************************************************************

#ifndef __MOS_MissionPion_h_
#define __MOS_MissionPion_h_

#include "MOS_Types.h"

#include "MOS_Mission_ABC.h"

class MOS_ASN_MsgPionOrder;

//=============================================================================
// Created:  NLD $TIME$
//=============================================================================
class MOS_MissionPion : public MOS_Mission_ABC
{
    MT_COPYNOTALLOWED( MOS_MissionPion )

public:
     MOS_MissionPion( E_MissionID nTypeMission, QWidget* pParent = 0 );
    ~MOS_MissionPion();

    //-------------------------------------------------------------------------
    /** @name Init */
    //-------------------------------------------------------------------------
    //@{
    virtual void Initialize();
    virtual void Terminate ();

    virtual void SetAgent( MOS_Agent* pAgent );
    virtual bool IsAutomateMission() const;
    //@}

    //-------------------------------------------------------------------------
    /** @name Exit*/
    //-------------------------------------------------------------------------
    //@{
    void Cancel();
    void Validate();
    //@}

private:
    //-------------------------------------------------------------------------
    /** @name Init */
    //-------------------------------------------------------------------------
    //@{
    void CreateMissionTools();
    void FillRandomParameters();
    void ReloadParameters( MT_InputArchive_ABC& archive );
    void ResaveParameters( MT_OutputArchive_ABC& archive );
    //@}


    //-------------------------------------------------------------------------
    /** @name Missions*/
    //-------------------------------------------------------------------------
    //@{
 $MissionCreatorDeclarations$
    //@}

    //-------------------------------------------------------------------------
    /** @name Network */
    //-------------------------------------------------------------------------
    //@{
    virtual void SendMsg( std::string& sParam );
    //@}

private:
    MOS_ASN_MsgPionOrder*   pASNMsgOrder_;

    QTabWidget*     pLimaLimitWidget_;
    QVBoxLayout*    pLimaLimitLayout_;
    QTabWidget*     pCommonWidget_;
    QVBoxLayout*    pCommonLayout_;
};


#endif // __MOS_MissionPion_h_