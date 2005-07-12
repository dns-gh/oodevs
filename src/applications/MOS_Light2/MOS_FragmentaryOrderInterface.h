// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-05-12 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_FragmentaryOrderInterface.h $
// $Author: Ape $
// $Modtime: 25/10/04 16:26 $
// $Revision: 2 $
// $Workfile: MOS_FragmentaryOrderInterface.h $
//
// *****************************************************************************

#ifndef __MOS_FragmentaryOrderInterface_h_
#define __MOS_FragmentaryOrderInterface_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_MissionInterface_ABC.h"

class MOS_ASN_MsgOrderConduite;


// =============================================================================
/** @class  MOS_FragmentaryOrderInterface
    @brief  MOS_FragmentaryOrderInterface
    @par    Using example
    @code
    MOS_FragmentaryOrderInterface;
    @endcode
*/
// Created: APE 2004-05-12
// =============================================================================
class MOS_FragmentaryOrderInterface : public MOS_MissionInterface_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MOS_FragmentaryOrderInterface );

public:
    //! @name Constructors/Destructor
    //@{
     MOS_FragmentaryOrderInterface( MOS_Agent& agent, uint nMissionId, MOS_MissionPanel& parentPanel );
    ~MOS_FragmentaryOrderInterface();
    //@}

private:
    //! @name Helpers
    //@{
    void CreateInterface();

#include "MOS_FragmentaryOrderInterface_Gen.h"
    //@}

private slots:
    void OnOk();

private:
    //! @name Member data
    //@{
    MOS_ASN_MsgOrderConduite*   pASNMsgOrder_;
    uint                        nMissionId_;
    //@}
};


#ifdef MOS_USE_INLINE
#   include "MOS_FragmentaryOrderInterface.inl"
#endif

#endif // __MOS_FragmentaryOrderInterface_h_
