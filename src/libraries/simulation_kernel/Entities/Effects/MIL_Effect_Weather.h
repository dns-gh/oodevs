// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Effects/MIL_Effect_Weather.h $
// $Author: Nld $
// $Modtime: 17/03/05 14:53 $
// $Revision: 4 $
// $Workfile: MIL_Effect_Weather.h $
//
// *****************************************************************************

#ifndef __MIL_Effect_Weather_h_
#define __MIL_Effect_Weather_h_

#include "MIL.h"

#include "MIL_Effect_ABC.h"
#include "MT_Tools/MT_Ellipse.h"

class PHY_IndirectFireDotationClass;

// =============================================================================
// @class  MIL_Effect_Weather
// Created: JVT 2004-08-03
// =============================================================================
class MIL_Effect_Weather : public MIL_Effect_ABC
{

public:
             MIL_Effect_Weather( const MT_Ellipse& surface, const PHY_IndirectFireDotationClass& ammoCategory, MT_Float rLifeDuration, MT_Float rDeploymentDuration );
    virtual ~MIL_Effect_Weather();

    //! @name Operations
    //@{
    virtual bool Execute();
    //@}

private:
    //! @name Network
    //@{
    void SendMsgStartEffect() const;
    void SendMsgStopEffect () const;
    //@}

private:
    const uint                           nID_;
    const MT_Ellipse                     surface_;
    const PHY_IndirectFireDotationClass& ammoCategory_;
    const uint                           nDeploymentTimeStep_;
    const uint                           nLifeLastTimeStep_;
          bool                           bIsDeployed_;
};

#include "MIL_Effect_Weather.inl"

#endif // __MIL_Effect_Weather_h_
