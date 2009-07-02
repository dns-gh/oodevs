//*****************************************************************************
//
// $Created: JVT 04-03-24 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Meteo/RawVisionData/PHY_AmmoEffect.h $
// $Author: Nld $
// $Modtime: 17/03/05 14:55 $
// $Revision: 2 $
// $Workfile: PHY_AmmoEffect.h $
//
//*****************************************************************************

#ifndef __PHY_AmmoEffect_h_
#define __PHY_AmmoEffect_h_

#include "MIL.h"

class PHY_Lighting;
class PHY_Precipitation;
class PHY_IndirectFireDotationClass;

//*****************************************************************************
// Created: JVT 04-03-24
//*****************************************************************************
class PHY_AmmoEffect : private boost::noncopyable
{

public:
     PHY_AmmoEffect( const PHY_IndirectFireDotationClass& ammoClass, PHY_AmmoEffect* pNextEffect );
    ~PHY_AmmoEffect();

    //! @name Accessors
    //@{
    const PHY_Lighting&      GetLighting     ( const PHY_Lighting&      mainLighting      ) const;
    const PHY_Precipitation& GetPrecipitation( const PHY_Precipitation& mainPrecipitation ) const;

    bool            HandleAmmo      ( const PHY_IndirectFireDotationClass& ammoClass ) const;
    PHY_AmmoEffect* GetNextEffect   () const;
    void            SetNextEffect   ( PHY_AmmoEffect* pNextEffect );
    //@}

private:
    const PHY_IndirectFireDotationClass& ammoClass_;
          PHY_AmmoEffect*                pNextEffect_;
};

#include "PHY_AmmoEffect.inl"

#endif // __PHY_AmmoEffect_h_