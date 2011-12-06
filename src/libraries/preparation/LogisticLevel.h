// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __LogisticLevel_h_
#define __LogisticLevel_h_

#include "tools/Resolver.h"

namespace kernel
{
    class LogisticLevel;
}

namespace preparation
{

// =============================================================================
/** @class  LogisticLevel
    @brief  LogisticLevel
*/
// Created: HBD 2010-11-15
// =============================================================================
class LogisticLevel : public tools::Resolver< kernel::LogisticLevel, QString >
{

public:
    //! @name Constructors/Destructor
    //@{
             LogisticLevel();
    virtual ~LogisticLevel();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LogisticLevel( const LogisticLevel& );            //!< Copy constructor
    LogisticLevel& operator=( const LogisticLevel& ); //!< Assignment operator
    //@}

};
}
#endif // __LogisticLevel_h_
