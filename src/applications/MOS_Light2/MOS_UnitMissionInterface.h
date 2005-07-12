// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-04-20 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_UnitMissionInterface.h $
// $Author: Ape $
// $Modtime: 25/10/04 10:14 $
// $Revision: 3 $
// $Workfile: MOS_UnitMissionInterface.h $
//
// *****************************************************************************

#ifndef __MOS_UnitMissionInterface_h_
#define __MOS_UnitMissionInterface_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_MissionInterface_ABC.h"

class MOS_Mission;
class MOS_ASN_MsgPionOrder;

// =============================================================================
/** @class  MOS_UnitMissionInterface
    @brief  MOS_UnitMissionInterface
    @par    Using example
    @code
    MOS_UnitMissionInterface;
    @endcode
*/
// Created: APE 2004-04-20
// =============================================================================
class MOS_UnitMissionInterface : public MOS_MissionInterface_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MOS_UnitMissionInterface );

public:
    //! @name Constructors/Destructor
    //@{
    MOS_UnitMissionInterface( MOS_Agent& agent, uint nMissionId, MOS_MissionPanel& parentPanel );
    ~MOS_UnitMissionInterface();
    //@}

private:
    //! @name Helpers
    //@{
    void CreateDefaultParameters();
    void CreateInterface();
    //@}


    //! @name Generated helpers
    //@{
    #include "MOS_UnitMissionInterface_Gen.h" 
    //@}

private slots:
    void OnOk();

private:
    //! @name Member data
    //@{
    uint nMissionId_;
    MOS_ASN_MsgPionOrder* pASNMsgOrder_;
    //@}
};


#ifdef MOS_USE_INLINE
#   include "MOS_UnitMissionInterface.inl"
#endif

#endif // __MOS_UnitMissionInterface_h_
