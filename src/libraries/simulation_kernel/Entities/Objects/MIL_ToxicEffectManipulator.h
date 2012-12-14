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

#include "Entities/Agents/Units/Humans/WoundEffects_ABC.h"

class MIL_NbcAgentType;
class PHY_HumanWound;

// =============================================================================
/** @class  MIL_ToxicEffectManipulator
    @brief  MIL Toxic effect manipulator
*/
// Created: JCR 2008-06-06
// =============================================================================
class MIL_ToxicEffectManipulator : public WoundEffects_ABC
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

    //! @name Accessors
    //@{
    const MIL_NbcAgentType& GetType() const;
    double                  GetQuantity() const;
    //@}

    //! @name Operations
    //@{
    virtual void ApplyWound( Human_ABC& human ) const;
    //@}

private:
    //! @name Helpers
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

#endif // __MIL_ToxicEffect_ABC_h_
