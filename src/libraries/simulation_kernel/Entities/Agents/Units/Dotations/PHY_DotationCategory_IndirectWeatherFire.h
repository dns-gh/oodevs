// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Dotations/PHY_DotationCategory_IndirectWeatherFire.h $
// $Author: Nld $
// $Modtime: 17/03/05 14:53 $
// $Revision: 4 $
// $Workfile: PHY_DotationCategory_IndirectWeatherFire.h $
//
// *****************************************************************************

#ifndef __PHY_DotationCategory_IndirectWeatherFire_h_
#define __PHY_DotationCategory_IndirectWeatherFire_h_

#include "PHY_DotationCategory_IndirectFire_ABC.h"

class MIL_Effect_Weather;

namespace xml
{
    class xistream;
}

// =============================================================================
// @class  PHY_DotationCategory_IndirectWeatherFire
// Created: JVT 2004-08-03
// =============================================================================
class PHY_DotationCategory_IndirectWeatherFire : public PHY_DotationCategory_IndirectFire_ABC
{
public:
             PHY_DotationCategory_IndirectWeatherFire( const PHY_IndirectFireDotationClass& type, const PHY_DotationCategory& dotationCategory, xml::xistream& xis,
                                                       unsigned int nInterventionType, double rDispersionX, double rDispersionY, double rDetectionRange );
    virtual ~PHY_DotationCategory_IndirectWeatherFire();

    //! @name
    //@{
    static PHY_DotationCategory_IndirectFire_ABC& Create( const PHY_IndirectFireDotationClass& type, const PHY_DotationCategory& dotationCategory, xml::xistream& xis,
                                                          unsigned int nInterventionType, double rDispersionX, double rDispersionY, double rDetectionRange );
    //@}

    //! @name Operations
    //@{
    virtual void ApplyEffect( const MIL_Agent_ABC* pFirer, const MT_Vector2D& vSourcePosition, const MT_Vector2D& vTargetPosition, double rInterventionTypeFired, PHY_FireResults_ABC& fireResult ) const;
    virtual double GetSmokeDuration() const;
    void NotifyEffectStarted( const MIL_Effect_Weather& effect ) const;
    //@}

private:
    double rDispersionCoef_;
    double rDeploymentDuration_;
    double rLifeDuration_;
};

#endif // __PHY_DotationCategory_IndirectWeatherFire_h_
