// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __MIL_ContaminationEffect_h_
#define __MIL_ContaminationEffect_h_

#include <boost/noncopyable.hpp>

class MIL_PopulationHumans;

// =============================================================================
/** @class  MIL_ContaminationEffect
    @brief  MIL_ContaminationEffect
*/
// Created: LGY 2011-04-29
// =============================================================================
class MIL_ContaminationEffect : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_ContaminationEffect( MIL_PopulationHumans& humans, unsigned int delay, unsigned int time );
    virtual ~MIL_ContaminationEffect();
    //@}

    //! @name Operations
    //@{
    void Update( unsigned int time );
    //@}

private:
    //! @name Member data
    //@{
    unsigned int delay_;
    MIL_PopulationHumans& humans_;
    unsigned int time_;
    //@}
};

#endif // __MIL_ContaminationEffect_h_
