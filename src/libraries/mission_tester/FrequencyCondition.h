// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __FrequencyCondition_h_
#define __FrequencyCondition_h_

#include "TriggerCondition_ABC.h"

namespace mission_tester
{
// =============================================================================
/** @class  FrequencyCondition
    @brief  FrequencyCondition
*/
// Created: PHC 2011-03-30
// =============================================================================
class FrequencyCondition : public TriggerCondition_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             FrequencyCondition();
    virtual ~FrequencyCondition();
    //@}

    //! @name Operations
    //@{
    virtual bool Check( const unsigned int delta );
    //@}
};
}

#endif // __FrequencyCondition_h_
