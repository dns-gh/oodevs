// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
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
class MT_Vector2D;

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
    double ConvertToInterventionType( unsigned int nNbr ) const;
    double ConvertToNbrAmmo( double rNbrIT ) const;
    virtual void ApplyEffect( const MIL_Agent_ABC& firer, const MT_Vector2D& vSourcePosition, const MT_Vector2D& vTargetPosition, double rInterventionTypeFired, PHY_FireResults_ABC& fireResult ) const = 0;
    virtual void ApplyEffect( const MIL_Agent_ABC& firer, MIL_Agent_ABC& target, double rInterventionTypeFired, PHY_FireResults_ABC& fireResult ) const;
    virtual bool HasHit( const MIL_Agent_ABC& target, double ) const;
    //@}

protected:
    //! @name Member data
    //@{
    const PHY_IndirectFireDotationClass& category_;
    const PHY_DotationCategory& dotationCategory_;
    unsigned int nInterventionType_;
    double rDispersionX_;
    double rDispersionY_;
    //@}
};

#endif // __PHY_DotationCategory_IndirectFire_ABC_h_
