// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "protocol_test_pch.h"
#include "MessageHelpers.h"
#pragma warning( push, 0 )
#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>
#pragma warning( pop )

namespace
{
    bool operator==( const google::protobuf::FieldDescriptor& lhs, const google::protobuf::FieldDescriptor& rhs )
    {
        BOOST_CHECK_EQUAL( lhs.name(), rhs.name() );
        BOOST_CHECK_EQUAL( lhs.index(), rhs.index() );
        BOOST_CHECK_EQUAL( lhs.type(), rhs.type() );
        // $$$$ SEB 2009-10-25: TODO: Check value
        return lhs.name() == rhs.name() && lhs.index() == rhs.index() && lhs.type() == rhs.type();
    }
}

bool operator==( const google::protobuf::Message& lhs, const google::protobuf::Message& rhs )
{
    const google::protobuf::Message::Reflection* lhsRef = lhs.GetReflection();
    const google::protobuf::Message::Reflection* rhsRef = rhs.GetReflection();
    if( !lhsRef )
        throw std::runtime_error( __FUNCTION__ " : reflection not available for lhs message." );
    if( !rhsRef )
        throw std::runtime_error( __FUNCTION__ " : reflection not available for rhs message." );
    typedef std::vector< const google::protobuf::FieldDescriptor* > T_Fields;
    T_Fields lFields; lhsRef->ListFields( lhs, &lFields );
    T_Fields rFields; rhsRef->ListFields( rhs, &rFields );
    T_Fields::const_iterator lIt = lFields.begin();
    T_Fields::const_iterator rIt = rFields.begin();
    for( ; lIt != lFields.end() && rIt != rFields.end(); ++lIt, ++rIt )
    {
        const google::protobuf::FieldDescriptor* lField = *lIt;
        const google::protobuf::FieldDescriptor* rField = *rIt;
        BOOST_CHECK( lField );
        BOOST_CHECK( rField );
        BOOST_CHECK( *lField == *rField );
    }
    return true;
}
