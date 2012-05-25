// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __kernel_ChangePopulationDisplay_h_
#define __kernel_ChangePopulationDisplay_h_

#include <boost/noncopyable.hpp>

namespace kernel
{
// =============================================================================
/** @class  ChangePopulationDisplay
    @brief  ChangePopulationDisplay
*/
// Created: LDC 2011-03-28
// =============================================================================
struct ChangePopulationDisplay : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    ChangePopulationDisplay( const std::string& populationName, bool visible )
        : populationName_( populationName )
        , visible_       ( visible )
    {
        // NOTHING
    }
    //@}

    //! @name Member data
    //@{
    const std::string populationName_;
    const bool visible_;
    //@}
};

} //! namespace kernel

#endif // __kernel_ChangePopulationDisplay_h_
