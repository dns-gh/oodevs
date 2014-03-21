// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef xsd_XmlToModel_h
#define xsd_XmlToModel_h

namespace core
{
    class Model;
}

namespace tools
{
    class Path;
}

namespace xml
{
    class xistream;
}

// *****************************************************************************
//
// XmlToModel( const core::Model& schema, const tools::Path& input, core::Model& model,
//             core::Model& model, T_Constraints constraints = T_Constraints() )
//
// This function uses 'schema' to fill 'model' with the data stored in the xml
// file designated by 'path'.
//
// Visit ProtoToModel.h to have an overview of the resulting model.
//
// *****************************************************************************

namespace xsd
{
    void XmlToModel( const core::Model& schema, const tools::Path& input, core::Model& model );
    void XmlToModel( const core::Model& schema, xml::xistream& xis, core::Model& model );
}

#endif // xsd_XmlToModel_h
