// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PHY_DotationCategory_IndirectFire_ABC_h_
#define __PHY_DotationCategory_IndirectFire_ABC_h_

namespace xml
{
    class xistream;
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
    const PHY_DotationCategory& GetDotationCategory() const;
    //@}

    //! @name Operations
    //@{
    MT_Float ConvertToInterventionType( unsigned int nNbr ) const;
    MT_Float ConvertToNbrAmmo( MT_Float rNbrIT ) const;
    virtual void ApplyEffect( const MIL_Agent_ABC& firer, const MT_Vector2D& vSourcePosition, const MT_Vector2D& vTargetPosition, MT_Float rInterventionTypeFired, PHY_FireResults_ABC& fireResult ) const = 0;
    virtual void ApplyEffect( const MIL_Agent_ABC& firer, MIL_Agent_ABC& target, MT_Float rInterventionTypeFired, PHY_FireResults_ABC& fireResult ) const;
    virtual bool HasHit( const MIL_Agent_ABC& target, MT_Float ) const;
    //@}

protected:
    //! @name Member data
    //@{
    const PHY_IndirectFireDotationClass& category_;
    const PHY_DotationCategory& dotationCategory_;
    unsigned int nInterventionType_;
    MT_Float rDispersionX_;
    MT_Float rDispersionY_;
    //@}
};

#endif // __PHY_DotationCategory_IndirectFire_ABC_h_
