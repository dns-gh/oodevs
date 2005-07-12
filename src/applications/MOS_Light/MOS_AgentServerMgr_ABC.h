//*****************************************************************************
//
// This file is part of MASA DIN library.
//
// Refer to the included end-user license agreement (License.rtf) for
// restrictions.
//
// Copyright (c) 2002 Mathématiques Appliquées SA (MASA)
// All Rights Reserved. DIN is a trademark of MASA Corporation.
//
//*****************************************************************************
//
// $Created: NLD 2002-07-12 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_AgentServerMgr_ABC.h $
// $Author: Nld $
// $Modtime: 20/02/03 15:56 $
// $Revision: 3 $
// $Workfile: MOS_AgentServerMgr_ABC.h $
//
//*****************************************************************************

#ifndef __MOS_AgentServerMgr_ABC_h_
#define __MOS_AgentServerMgr_ABC_h_

#ifdef __GNUG__
#    pragma interface
#endif

#include "MOS_Types.h"

class MOS_AgentServerController;
class MOS_AgentServer;
class MOS_MOSServer;
class MOS_Config;

//=============================================================================
/**
*   @class  MOS_AgentServerMgr_ABC
*   @brief  PUT THE COMMENTS ON THE CLASS HERE
*   @par    Using example
*   @code
*     PUT YOUR EXAMPLE CODE HERE
*   @endcode
*/
// Created: NLD 2002-07-12
//=============================================================================
class MOS_AgentServerMgr_ABC
{
    MT_COPYNOTALLOWED( MOS_AgentServerMgr_ABC );

public:
    MOS_AgentServerMgr_ABC( MOS_AgentServerController& controller );
    virtual ~MOS_AgentServerMgr_ABC();

protected:
    //-------------------------------------------------------------------------
    /** @name Tools */
    //-------------------------------------------------------------------------
    //@{
    MOS_AgentServer& GetAgentServerFromSession( DIN::DIN_Link& link );
    //@}

protected:
    MOS_AgentServerController&  controller_;
    MOS_MOSServer&              mosServer_;
};

#ifdef MOS_USE_INLINE
#   include "MOS_AgentServerMgr_ABC.inl"
#endif

#endif // __MOS_AgentServerMgr_ABC_h_
