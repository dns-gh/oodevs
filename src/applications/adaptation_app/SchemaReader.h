// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __SchemaReader_h_
#define __SchemaReader_h_

namespace xml
{
    class xistream;
};

class SchemaReader
{
    public:
    void ReadSchema( const std::string&, xml::xistream& xis )
    {
        schema_ = xis.attribute< std::string >( "xsi:noNamespaceSchemaLocation", "" );
    }
    const std::string& GetSchema()
    {
        return schema_;
    }
    private:
        std::string schema_;
};

#endif // __SchemaReader_h_
