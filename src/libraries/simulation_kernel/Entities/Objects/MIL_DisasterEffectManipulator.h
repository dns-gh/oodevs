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

#include <boost/noncopyable.hpp>
#include "Entities/Objects/MIL_DisasterType.h"

class PHY_HumanWound;

// =============================================================================
/** @class  MIL_DisasterEffectManipulator
    @brief  MIL Disaster effect manipulator
*/
// Created: LGY 2012-11-29
// =============================================================================
class MIL_DisasterEffectManipulator : private boost::noncopyable
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
    //! @name
    //@{
    template< typename WoundFunctor >
    void ApplyRandomWound( WoundFunctor functor ) const;
    //@}

    //@}
private:
    //! @name Data Members
    //@{
    int thresholdId_;
    const MIL_DisasterType& type_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: MIL_DisasterEffectManipulator::ApplyRandomWound
// Created: LGY 2012-11-29
// -----------------------------------------------------------------------------
template< typename WoundFunctor >
void MIL_DisasterEffectManipulator::ApplyRandomWound( WoundFunctor functor ) const
{
    functor( type_.GetRandomWound( thresholdId_ ) );
}

#endif // __MIL_DisasterEffect_ABC_h_
