// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PERCEPTION_LIST_IN_CIRCLE_VISITOR_H
#define SWORD_PERCEPTION_LIST_IN_CIRCLE_VISITOR_H

#include "wrapper/View.h"
#include <vector>
#include <boost/noncopyable.hpp>

namespace sword
{
namespace perception
{
// =============================================================================
/** @class  ListInCircleVisitor
    @brief  List in circle visitor
*/
// Created: SLI 2012-07-10
// =============================================================================
struct ListInCircleVisitor : private boost::noncopyable
{
    //! @name Constructors/Destructor
    //@{
    explicit ListInCircleVisitor( std::vector< wrapper::View >& list )
        : list_( list )
    {}
    //@}

    //! @name Operations
    //@{
    static void Add( const SWORD_Model* data, void* userData )
    {
        static_cast< ListInCircleVisitor* >( userData )->list_.push_back( data );
    }
    //@}

    //! @name Member data
    //@{
    std::vector< wrapper::View >& list_;
    //@}
};

}
}

#endif // ListInCircleVisitor_h
