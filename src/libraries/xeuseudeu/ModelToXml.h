// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef xsd_ModelToXml_h
#define xsd_ModelToXml_h

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
    class xostream;
}

// *****************************************************************************
//
// ModelToXml( const core::Model& model, const tools::Path& output )
//
// This function writes an xml file designated by 'output' with the data stored
// in 'model'.
//
// *****************************************************************************

namespace xsd
{
    void ModelToXml( const core::Model& model, const tools::Path& output );
    void ModelToXml( const core::Model& model, xml::xostream& xos );
}

#endif // xsd_ModelToXml_h
