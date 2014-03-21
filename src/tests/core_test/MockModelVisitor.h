// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef CORE_TEST_MOCK_MODEL_VISITOR_H
#define CORE_TEST_MOCK_MODEL_VISITOR_H

#include "core/ModelVisitor_ABC.h"

namespace core
{
    MOCK_BASE_CLASS( MockModelVisitor, ModelVisitor_ABC )
    {
        MOCK_METHOD( Visit, 1, void( int64_t ), VisitInt64 )
        MOCK_METHOD( Visit, 1, void( uint64_t ), VisitUint64 )
        MOCK_METHOD( Visit, 1, void( double ), VisitDouble )
        MOCK_METHOD( Visit, 1, void( const std::string& ), VisitString )
        MOCK_METHOD( Visit, 2, void( const std::string&, const Model& ), VisitNamedChild )
        MOCK_METHOD( Visit, 2, void( std::size_t, const Model& ), VisitIdentifiedChild )
        MOCK_METHOD( Visit, 1, void( const Model& ), VisitElement )
        MOCK_METHOD( Visit, 1, void( const boost::shared_ptr< UserData_ABC >& ), VisitUserData )
    };
}

#endif // CORE_TEST_MOCK_MODEL_VISITOR_H
