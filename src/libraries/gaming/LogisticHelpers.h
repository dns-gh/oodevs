// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticTools_h_
#define __LogisticTools_h_

#include <boost/function.hpp>

namespace kernel
{
    class Entity_ABC;
}

class Dotation;

// =============================================================================
/** @namespace logistic_helpers
    @brief  logistic helpers...
*/
// Created: MMC 2012-10-10
// =============================================================================
namespace logistic_helpers
{
    void VisitBaseStocksDotations( const kernel::Entity_ABC& logisticBase, boost::function< void( const Dotation& ) > func );

} // namespace

#endif // __LogisticTools_h_
