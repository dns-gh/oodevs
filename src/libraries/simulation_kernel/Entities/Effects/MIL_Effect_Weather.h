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

#include "MIL_Effect_Fire_ABC.h"

class PHY_DotationCategory_IndirectWeatherFire;

// =============================================================================
// @class  MIL_Effect_Weather
// Created: JVT 2004-08-03
// =============================================================================
class MIL_Effect_Weather : public MIL_Effect_Fire_ABC
{
public:
             MIL_Effect_Weather( const MT_Ellipse& surface, const PHY_DotationCategory_IndirectWeatherFire& dotationCategory, double rLifeDuration, double rDeploymentDuration );
    virtual ~MIL_Effect_Weather();

    //! @name Operations
    //@{
    virtual bool Execute();
    //@}

private:
    const PHY_DotationCategory_IndirectWeatherFire& dotationCategory_;
    const unsigned int nDeploymentTimeStep_;
    const unsigned int nLifeLastTimeStep_;
    bool bIsDeployed_;

    static MIL_IDManager idManager_;
};

#endif // __MIL_Effect_Weather_h_
