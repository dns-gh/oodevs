// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MessageHelpers_h_
#define __MessageHelpers_h_

namespace google
{
namespace protobuf
{
    class Message;
}
}

bool operator==( const google::protobuf::Message& lhs, const google::protobuf::Message& rhs );

#endif // __MessageHelpers_h_
