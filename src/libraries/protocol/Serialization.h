// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef PROTOCOL_SERIALIZATION_H__
#define PROTOCOL_SERIALIZATION_H__

#include <boost/interprocess/streams/bufferstream.hpp>
#include <boost/interprocess/streams/vectorstream.hpp>
#include <vector>

namespace boost {
namespace serialization {
template< class Archive >
void save( Archive& ar, const google::protobuf::Message& src, const unsigned int )
{
    boost::interprocess::basic_vectorstream< std::vector< char > > stream;
    src.SerializeToOstream( &stream );
    ar << stream.vector();
}

template< class Archive >
void load( Archive& ar, google::protobuf::Message& dst, const unsigned int )
{
    std::vector< char > data;
    ar >> data;
    boost::interprocess::ibufferstream stream( &data[0], data.size() );
    dst.ParseFromIstream( &stream );
}
}
}

#endif // PROTOCOL_SERIALIZATION_H__
