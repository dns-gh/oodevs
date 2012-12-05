// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __MIL_ToxicEffectManipulator_h_
#define __MIL_ToxicEffectManipulator_h_

class MIL_NbcAgentType;
class PHY_HumanWound;

// =============================================================================
/** @class  MIL_ToxicEffectManipulator
    @brief  MIL Toxic effect manipulator
*/
// Created: JCR 2008-06-06
// =============================================================================
class MIL_ToxicEffectManipulator
{
public:
    //! @name Types
    //@{
    typedef std::vector< const MIL_NbcAgentType* >  T_NBCAgents;
    typedef T_NBCAgents::const_iterator           CIT_NBCAgents;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             MIL_ToxicEffectManipulator( const T_NBCAgents& type, double quantity );
             MIL_ToxicEffectManipulator( const MIL_ToxicEffectManipulator& rhs );
    virtual ~MIL_ToxicEffectManipulator();
    //@}

    //! @name Operations
    //@{
    const MIL_NbcAgentType& GetType() const;
    double                  GetQuantity() const;
    //@}

    //! @name
    //@{
    template< typename WoundFunctor > bool ApplyRandomWound( WoundFunctor functor ) const;
    //@}

private:
    //! @name Helper
    //@{
    const PHY_HumanWound& GetRandomWound( const MIL_NbcAgentType& type ) const;
    //@}

private:
    //! @name Data Members
    //@{
    T_NBCAgents types_;
    double quantity_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: MIL_ToxicEffectManipulator::ApplyRandomWound
// Created: JCR 2008-08-28
// -----------------------------------------------------------------------------
template< typename WoundFunctor >
bool MIL_ToxicEffectManipulator::ApplyRandomWound( WoundFunctor functor ) const
{
    bool result = false;
    for( auto it = types_.begin(); it != types_.end(); ++it )
        result |= functor( GetRandomWound( **it ) );
    return result;
}

#endif // __MIL_ToxicEffect_ABC_h_
