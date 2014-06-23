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
             PHY_DotationCategory_IndirectFire_ABC( const PHY_IndirectFireDotationClass& category, const PHY_DotationCategory& dotationCategory,
                                                    unsigned int nInterventionType, double rDispersionX, double rDispersionY, double rDetectionRange );
    virtual ~PHY_DotationCategory_IndirectFire_ABC();

    //! @name Accessors
    //@{
    const PHY_IndirectFireDotationClass& GetIndirectFireDotationCategory() const;
    const PHY_DotationCategory& GetDotationCategory() const;
    virtual double GetSmokeDuration() const;
    //@}

    //! @name Operations
    //@{
    virtual void ApplyEffect( const MIL_Agent_ABC* pFirer, const MT_Vector2D& vSourcePosition, const MT_Vector2D& vTargetPosition, double rInterventionTypeFired, PHY_FireResults_ABC& fireResult ) const = 0;
    virtual void ApplyStrikeEffect( const MIL_Agent_ABC& firer, MIL_Agent_ABC& target, double rInterventionTypeFired, PHY_FireResults_ABC& fireResult ) const;
    virtual bool HasHit( const MIL_Agent_ABC& target, double ) const;
    void ApplyDetectionRangeEffect( const MT_Vector2D& vTargetPosition, const std::vector< unsigned int >& fireEffectsIds ) const;
    //@}

protected:
    //! @name Member data
    //@{
    const PHY_IndirectFireDotationClass& category_;
    const PHY_DotationCategory& dotationCategory_;
    unsigned int nInterventionType_;
    double rDispersionX_;
    double rDispersionY_;
    double rDetectionRange_;
    //@}
};

#endif // __PHY_DotationCategory_IndirectFire_ABC_h_
