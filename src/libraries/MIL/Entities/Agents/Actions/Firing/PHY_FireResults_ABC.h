// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/PHY_FireResults_ABC.h $
// $Author: Nld $
// $Modtime: 12/04/05 17:19 $
// $Revision: 3 $
// $Workfile: PHY_FireResults_ABC.h $
//
// *****************************************************************************

#ifndef __PHY_FireResults_ABC_h_
#define __PHY_FireResults_ABC_h_

#include "MIL.h"

#include "PHY_AgentFireResult.h"

class MIL_Agent_ABC;

// =============================================================================
// @class  PHY_FireResults_ABC
// Created: JVT 2004-08-03
// =============================================================================
class PHY_FireResults_ABC
{
    MT_COPYNOTALLOWED( PHY_FireResults_ABC )

public:
    static MIL_MOSIDManager idManager_;

public:
    PHY_FireResults_ABC();
    virtual ~PHY_FireResults_ABC();

    //! @name Accessors
    //@{
    PHY_AgentFireResult& GetAgentFireResult( const MIL_Agent_ABC& target );
    //@}

    //! @name Refs
    //@{
    void IncRef();
    void DecRef();
    //@}

protected:
    //! @name Types
    //@{
    typedef std::map< const MIL_Agent_ABC*, PHY_AgentFireResult > T_ResultMap;
    typedef T_ResultMap::const_iterator                           CIT_ResultMap;
    //@}

protected:
    T_ResultMap results_;
    uint        nNbrRefs_;
};

#include "PHY_FireResults_ABC.inl"

#endif // __PHY_FireResults_ABC_h_
