// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __MIL_DecontaminationEffect_h_
#define __MIL_DecontaminationEffect_h_

#include <boost/noncopyable.hpp>

class MIL_PopulationHumans;

// =============================================================================
/** @class  MIL_DecontaminationEffect
    @brief  MIL_DecontaminationEffect
*/
// Created: LGY 2011-11-09
// =============================================================================
class MIL_DecontaminationEffect : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_DecontaminationEffect( MIL_PopulationHumans& humans, double delay );
    virtual ~MIL_DecontaminationEffect();
    //@}

    //! @name Operations
    //@{
    bool Apply( double rRatioWorkers, unsigned int tick );
    //@}

private:
    //! @name Member data
    //@{
    double delay_;
    MIL_PopulationHumans& humans_;
    unsigned int tick_;
    //@}
};

#endif // __MIL_DecontaminationEffect_h_
