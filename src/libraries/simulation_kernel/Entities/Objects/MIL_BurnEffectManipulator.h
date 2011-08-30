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

class MIL_FireClass;
class PHY_HumanWound;

// =============================================================================
/** @class  MIL_BurnEffectManipulator
    @see "Incendie" in 4311340-Modèles de feu.doc
*/
// Created: BCI 2010-12-14
// =============================================================================
class MIL_BurnEffectManipulator
{
public:
    //! @name Constructors/Destructor
    //@{
    MIL_BurnEffectManipulator( const MIL_FireClass& );
    //@}

    //! @name Operations
    //@{
    template< typename WoundFunctor > bool ApplyRandomWound( WoundFunctor functor ) const;
    //@}

private:
    //! @name Helpers
    //@{
    const PHY_HumanWound* ChooseRandomWound() const;
    //@}

    //! @name Data Members
    //@{
    const MIL_FireClass* pFireClass_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: MIL_BurnEffectManipulator::ApplyRandomWound
// Created: JCR 2008-08-28
// -----------------------------------------------------------------------------
template< typename WoundFunctor >
bool MIL_BurnEffectManipulator::ApplyRandomWound( WoundFunctor functor ) const
{
    const PHY_HumanWound* pWound = ChooseRandomWound();
    if( pWound )
        return functor( *pWound );
    else
        return false;
}

#endif // __MIL_ToxicEffect_ABC_h_
