// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Dotations/PHY_DotationCategory_IndirectFire.h $
// $Author: Nld $
// $Modtime: 17/03/05 14:53 $
// $Revision: 3 $
// $Workfile: PHY_DotationCategory_IndirectFire.h $
//
// *****************************************************************************

#ifndef __PHY_DotationCategory_IndirectFire_h_
#define __PHY_DotationCategory_IndirectFire_h_

#include "MIL.h"

#include "PHY_DotationCategory_IndirectFire_ABC.h"
#include "MT_Tools/MT_Random.h"

namespace xml
{
    class xistream;
}

// =============================================================================
// @class  PHY_DotationCategory_IndirectFire
// Created: JVT 2004-08-03
// =============================================================================
class PHY_DotationCategory_IndirectFire : public PHY_DotationCategory_IndirectFire_ABC
{
    MT_COPYNOTALLOWED( PHY_DotationCategory_IndirectFire )

public:
    PHY_DotationCategory_IndirectFire( const PHY_IndirectFireDotationClass& type, const PHY_DotationCategory& dotationCategory, xml::xistream& xis );
    virtual ~PHY_DotationCategory_IndirectFire();

    //! @name 
    //@{
    static PHY_DotationCategory_IndirectFire_ABC& Create( const PHY_IndirectFireDotationClass& type, const PHY_DotationCategory& dotationCategory, xml::xistream& xis );
    //@}

    //! @name Operations
    //@{
    virtual void ApplyEffect( const MIL_AgentPion& firer, const MT_Vector2D& vSourcePosition, const MT_Vector2D& vTargetPosition, MT_Float rInterventionTypeFired, PHY_FireResults_ABC& fireResult ) const;
    virtual bool HasHit     ( const MIL_Agent_ABC& target ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< MT_Float >     T_PhVector;
    typedef T_PhVector::const_iterator  CIT_PhVector;
    //@}
    //! @name Helpers
    //@{
    void ReadPh( xml::xistream& xis );
    //@}

private:
    MT_Float   rNeutralizationCoef_;
    T_PhVector phs_;

private:
    static MT_Random random_;
};

#include "PHY_DotationCategory_IndirectFire.inl"

#endif // __PHY_DotationCategory_IndirectFire_h_
