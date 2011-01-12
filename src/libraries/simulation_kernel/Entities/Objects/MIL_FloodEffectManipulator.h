// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __MIL_FloodEffectManipulator_h_
#define __MIL_FloodEffectManipulator_h_

class PHY_HumanWound;

// =============================================================================
/** @class  MIL_FloodEffectManipulator
    @brief  MIL Flood effect manipulator
*/
// Created: JSR 2011-01-11
// =============================================================================
class MIL_FloodEffectManipulator
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_FloodEffectManipulator( const std::map< const PHY_HumanWound*, double >& map );
    virtual ~MIL_FloodEffectManipulator();
    //@}

    //! @name
    //@{
    template< typename WoundFunctor > void ApplyRandomWound( WoundFunctor functor ) const;
    //@}

private:
    //! @name Helper
    //@{
    const PHY_HumanWound& GetRandomWound() const;
    //@}

private:
    //! @name Data Members
    //@{
    const std::map< const PHY_HumanWound*, double >& map_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: MIL_FloodEffectManipulator::ApplyRandomWound
// Created: JSR 2011-01-11
// -----------------------------------------------------------------------------
template< typename WoundFunctor >
void MIL_FloodEffectManipulator::ApplyRandomWound( WoundFunctor functor ) const
{
    functor( GetRandomWound() );
}

#endif // __MIL_FloodEffectManipulator_h_
