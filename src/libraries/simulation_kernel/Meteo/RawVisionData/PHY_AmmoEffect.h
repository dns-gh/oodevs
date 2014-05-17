// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 MASA Group
//
// *****************************************************************************
#ifndef __PHY_AmmoEffect_h_
#define __PHY_AmmoEffect_h_

namespace weather
{
    class PHY_Lighting;
    class PHY_Precipitation;
}

class PHY_IndirectFireDotationClass;

//*****************************************************************************
// Created: JVT 04-03-24
//*****************************************************************************
class PHY_AmmoEffect : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             PHY_AmmoEffect( const PHY_IndirectFireDotationClass& ammoClass, PHY_AmmoEffect* pNextEffect );
    virtual ~PHY_AmmoEffect();
    //@}

    //! @name Accessors
    //@{
    const weather::PHY_Lighting& GetLighting( const weather::PHY_Lighting& mainLighting ) const;
    const weather::PHY_Precipitation& GetPrecipitation( const weather::PHY_Precipitation& mainPrecipitation ) const;

    bool HandleAmmo( const PHY_IndirectFireDotationClass& ammoClass ) const;
    PHY_AmmoEffect* GetNextEffect() const;
    void SetNextEffect( PHY_AmmoEffect* pNextEffect );
    //@}

private:
    const PHY_IndirectFireDotationClass& ammoClass_;
    PHY_AmmoEffect* pNextEffect_;
};

#endif // __PHY_AmmoEffect_h_
