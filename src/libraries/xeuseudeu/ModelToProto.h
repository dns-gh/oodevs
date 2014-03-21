// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef xsd_ModelToProto_h
#define xsd_ModelToProto_h

namespace core
{
    class Model;
}

namespace google
{
namespace protobuf
{
    class Message;
}
}

// *****************************************************************************
//
// ModelToProto( const core::Model& model, google::protobuf::Message& message )
//
// This function fills 'message' with the data stored in 'model'
//
// *****************************************************************************

namespace xsd
{
    void ModelToProto( const core::Model& model, google::protobuf::Message& message );
    void ModelToProto( const core::Model& model, int& enumMsg );
}

#endif // xsd_ModelToProto_h
