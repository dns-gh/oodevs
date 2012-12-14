// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __MIL_BurnEffectManipulator_h_
#define __MIL_BurnEffectManipulator_h_

#include "Entities/Agents/Units/Humans/WoundEffects_ABC.h"

class MIL_FireClass;
class PHY_HumanWound;

// =============================================================================
/** @class  MIL_BurnEffectManipulator
    @see "Incendie" in 4311340-Modèles de feu.doc
*/
// Created: BCI 2010-12-14
// =============================================================================
class MIL_BurnEffectManipulator : public WoundEffects_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    MIL_BurnEffectManipulator( const MIL_FireClass& );
    //@}

    //! @name Operations
    //@{
    virtual void ApplyWound( Human_ABC& human ) const;
    //@}

private:
    //! @name Helpers
    //@{
    const PHY_HumanWound* ChooseRandomWound() const;
    //@}

private:
    //! @name Data Members
    //@{
    const MIL_FireClass* pFireClass_;
    //@}
};

#endif // __MIL_ToxicEffect_ABC_h_
