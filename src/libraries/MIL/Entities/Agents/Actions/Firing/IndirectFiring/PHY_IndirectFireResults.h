// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/IndirectFiring/PHY_IndirectFireResults.h $
// $Author: Nld $
// $Modtime: 12/04/05 17:19 $
// $Revision: 4 $
// $Workfile: PHY_IndirectFireResults.h $
//
// *****************************************************************************

#ifndef __PHY_IndirectFireResults_h_
#define __PHY_IndirectFireResults_h_

#include "MIL.h"

#include "Tools/MIL_MOSIDManager.h"
#include "Entities/Agents/Actions/Firing/PHY_FireResults_ABC.h"

class MIL_Agent_ABC;
class MIL_AgentPion;
class PHY_DotationCategory;

// =============================================================================
// @class  PHY_IndirectFireResults
// Created: JVT 2004-08-03
// =============================================================================
class PHY_IndirectFireResults : public PHY_FireResults_ABC
{
    MT_COPYNOTALLOWED( PHY_IndirectFireResults )

public:
    static MIL_MOSIDManager idManager_;

public:
    PHY_IndirectFireResults( const MIL_AgentPion& firer, const MT_Vector2D& vTargetPosition, const PHY_DotationCategory& dotationCategory );
    virtual ~PHY_IndirectFireResults();

    //! @name Accessors
    //@{
    uint GetID() const;
    //@}

private:
    const uint           nID_;
    const MIL_AgentPion& firer_;
    const MT_Vector2D&   vTargetPosition_;
};

#include "PHY_IndirectFireResults.inl"

#endif // __PHY_IndirectFireResults_h_
