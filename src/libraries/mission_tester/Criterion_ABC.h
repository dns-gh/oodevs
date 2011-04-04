// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Criterion_ABC_h_
#define __Criterion_ABC_h_

#include <boost/noncopyable.hpp>

namespace mission_tester
{

// =============================================================================
/** @class  Criterion_ABC
    @brief  Criterion_ABC
*/
// Created: PHC 2011-04-04
// =============================================================================
class Criterion_ABC : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             Criterion_ABC() {};
    virtual ~Criterion_ABC() {};
    //@}
};

class AgentCriterion : public Criterion_ABC
{};

}

#endif // __Criterion_ABC_h_
