// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __AgentFilter_h_
#define __AgentFilter_h_

#include "Filter_ABC.h"

namespace mission_tester
{
    class Criterion_ABC;
// =============================================================================
/** @class  AgentFilter
    @brief  AgentFilter
*/
// Created: PHC 2011-03-31
// =============================================================================
class AgentFilter : public Filter_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit AgentFilter( const Criterion_ABC& criterion );
    virtual ~AgentFilter();
    //@}

    //! @name Operations
    //@{
    virtual bool Accepts( const Schedulable_ABC& schedulable ) const;
    //@}

private:
    //! @name Member data
    //@{
    const Criterion_ABC& criterion_;
    //@}
};
}

#endif // __AgentFilter_h_
