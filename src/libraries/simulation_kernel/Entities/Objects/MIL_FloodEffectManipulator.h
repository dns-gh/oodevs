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

#include "Entities/Agents/Units/Humans/WoundEffects_ABC.h"
#include <tools/Map.h>

class PHY_HumanWound;

// =============================================================================
/** @class  MIL_FloodEffectManipulator
    @brief  MIL Flood effect manipulator
*/
// Created: JSR 2011-01-11
// =============================================================================
class MIL_FloodEffectManipulator : public WoundEffects_ABC
{
public:
    typedef tools::Map< const PHY_HumanWound*, double > T_HumanWounds;

public:
    //! @name Constructors/Destructor
    //@{
    explicit MIL_FloodEffectManipulator( const T_HumanWounds& wounds );
    virtual ~MIL_FloodEffectManipulator();
    //@}

    //! @name Operations
    //@{
    virtual void ApplyWound( Human_ABC& human ) const;
    //@}

private:
    //! @name Helper
    //@{
    const PHY_HumanWound& GetRandomWound() const;
    //@}

private:
    //! @name Data Members
    //@{
    const T_HumanWounds& wounds_;
    //@}
};

#endif // __MIL_FloodEffectManipulator_h_
