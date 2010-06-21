// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Dotations/PHY_DotationCategory_IndirectFire_ABC.h $
// $Author: Jvt $
// $Modtime: 2/05/05 14:37 $
// $Revision: 5 $
// $Workfile: PHY_DotationCategory_IndirectFire_ABC.h $
//
// *****************************************************************************

#ifndef __PHY_DotationCategory_IndirectFire_ABC_h_
#define __PHY_DotationCategory_IndirectFire_ABC_h_

namespace xml
{
}

class MIL_AgentPion;
class MIL_Agent_ABC;
class MIL_EntityManager_ABC;
class PHY_DotationCategory;
class PHY_FireResults_ABC;
class PHY_IndirectFireDotationClass;

// =============================================================================
// @class  PHY_DotationCategory_IndirectFire_ABC
// Created: JVT 2004-08-03
// =============================================================================
class PHY_DotationCategory_IndirectFire_ABC : private boost::noncopyable
{

public:
             PHY_DotationCategory_IndirectFire_ABC( const PHY_IndirectFireDotationClass& category, const PHY_DotationCategory& dotationCategory, xml::xistream& xis );
    virtual ~PHY_DotationCategory_IndirectFire_ABC();

    //! @name Accessors
    //@{
    const PHY_IndirectFireDotationClass& GetIndirectFireDotationCategory() const;
    const PHY_DotationCategory&          GetDotationCategory            () const;
    //@}

    //! @name Operations
    //@{
            MT_Float ConvertToInterventionType( unsigned int nNbr ) const;
            MT_Float ConvertToNbrAmmo         ( MT_Float rNbrIT ) const;
    virtual void     ApplyEffect              ( const MIL_Agent_ABC& firer, const MT_Vector2D& vSourcePosition, const MT_Vector2D& vTargetPosition, MT_Float rInterventionTypeFired, PHY_FireResults_ABC& fireResult ) const = 0;
    virtual void     ApplyEffect              ( const MIL_Agent_ABC& firer, MIL_Agent_ABC& target, MT_Float rInterventionTypeFired, PHY_FireResults_ABC& fireResult ) const;
    virtual bool     HasHit                   ( const MIL_Agent_ABC& target, MT_Float ) const;
    //@}

protected:
    const PHY_IndirectFireDotationClass& category_;
    const PHY_DotationCategory&          dotationCategory_;
          unsigned int                   nInterventionType_;
          MT_Float                       rDispersionX_;
          MT_Float                       rDispersionY_;
};

#endif // __PHY_DotationCategory_IndirectFire_ABC_h_
