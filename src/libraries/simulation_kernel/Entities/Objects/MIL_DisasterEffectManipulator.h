// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __MIL_DisasterEffectManipulator_h_
#define __MIL_DisasterEffectManipulator_h_

#include "Entities/Agents/Units/Humans/WoundEffects_ABC.h"

class MIL_DisasterType;

// =============================================================================
/** @class  MIL_DisasterEffectManipulator
    @brief  MIL Disaster effect manipulator
*/
// Created: LGY 2012-11-29
// =============================================================================
class MIL_DisasterEffectManipulator : public WoundEffects_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_DisasterEffectManipulator( int thresholdId, const MIL_DisasterType& type );
    virtual ~MIL_DisasterEffectManipulator();
    //@}

public:
    //! @name Operations
    //@{
    virtual void ApplyWound( Human_ABC& human ) const;
    //@}

    //@}
private:
    //! @name Data Members
    //@{
    int thresholdId_;
    const MIL_DisasterType& type_;
    //@}
};

#endif // __MIL_DisasterEffect_ABC_h_
