// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __PHY_DotationCategory_IndirectFire_h_
#define __PHY_DotationCategory_IndirectFire_h_

#include "PHY_DotationCategory_IndirectFire_ABC.h"
#include "MIL_Random.h"

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
public:
    //! @name Constructors/Destructor
    //@{
             PHY_DotationCategory_IndirectFire( const PHY_IndirectFireDotationClass& type, const PHY_DotationCategory& dotationCategory, xml::xistream& xis );
    virtual ~PHY_DotationCategory_IndirectFire();
    //@}

    //! @name
    //@{
    static PHY_DotationCategory_IndirectFire_ABC& Create( const PHY_IndirectFireDotationClass& type, const PHY_DotationCategory& dotationCategory, xml::xistream& xis );
    //@}

    //! @name Operations
    //@{
    virtual void ApplyEffect( const MIL_Agent_ABC& firer, const MT_Vector2D& vSourcePosition, const MT_Vector2D& vTargetPosition, double rInterventionTypeFired, PHY_FireResults_ABC& fireResult ) const;
    virtual void ApplyEffect( const MIL_Agent_ABC& firer, MIL_Agent_ABC& target, double rInterventionTypeFired, PHY_FireResults_ABC& fireResult ) const;
    virtual bool HasHit     ( const MIL_Agent_ABC& target, double ratio ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< double >       T_PhVector;
    typedef T_PhVector::const_iterator  CIT_PhVector;
    //@}

    //! @name Helpers
    //@{
    void ReadPh( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    double   rNeutralizationCoef_;
    T_PhVector phs_;
    //@}
};

#endif // __PHY_DotationCategory_IndirectFire_h_
