// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __TriggerCondition_ABC_h_
#define __TriggerCondition_ABC_h_

#include <boost/noncopyable.hpp>

namespace mission_tester
{
// =============================================================================
/** @class  TriggerCondition_ABC
    @brief  TriggerCondition_ABC
*/
// Created: PHC 2011-03-30
// =============================================================================
class TriggerCondition_ABC : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             TriggerCondition_ABC() {}
    virtual ~TriggerCondition_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual bool Check( const unsigned int delta ) = 0;
    //@}
};
}

#endif // __TriggerCondition_ABC_h_
