// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "GenerateProto.h"
#include "Helpers.h"
#include "core/Model.h"
#include "core/NamedChildrenVisitor.h"
#include <tools/Path.h>
#include <tools/StdFileWrapper.h>
#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>
#pragma warning( push )
#pragma warning( disable: 4996 )
#include <boost/lexical_cast.hpp>
#pragma warning( pop )
#include <map>

const char* const tab = "    ";

namespace
{
    const std::string header =
        "// *****************************************************************************\n"
        "//\n"
        "// This file is part of a MASA library or program.\n"
        "// Refer to the included end-user license agreement for restrictions.\n"
        "//\n"
        "// Copyright (c) MASA Group\n"
        "//\n"
        "// This file was generated, do not modify it.\n"
        "//\n"
        "// *****************************************************************************\n";

    std::string ReadAnnotation( const core::Model& annotation, const std::string& language )
    {
        if( !language.empty() && annotation.HasChild( language ) )
            return annotation[ language ];
        if( annotation.HasChild( "default" ) )
            return annotation[ "default" ];
        return "";
    }

    void ConvertInclude( std::ostream& os, const std::string& key )
    {
        if( boost::algorithm::ends_with( key, ".xsd" ) )
        {
            const std::string proto = boost::algorithm::replace_last_copy( key, ".xsd", ".proto" );
            os << "import \"" << proto << "\";" << std::endl;
        }
    }

    class ValueVisitor : public core::ModelVisitor_ABC
    {
    public:
        explicit ValueVisitor( std::string& text ) : text_( text ) {}
        virtual ~ValueVisitor() {}
        virtual void Visit( int64_t value )
        {
            text_ += boost::lexical_cast< std::string >( value );
        }
        virtual void Visit( uint64_t value )
        {
            text_ += boost::lexical_cast< std::string >( value );
        }
        virtual void Visit( double value )
        {
            text_ += boost::lexical_cast< std::string >( value );
        }
        virtual void Visit( const std::string& value )
        {
            text_ += "\"" + value + "\"";
        }
        virtual void Visit( const std::string& /*key*/, const core::Model& /*type*/ ) {}
        virtual void Visit( std::size_t /*key*/, const core::Model& /*child*/ ) {}
        virtual void Visit( const core::Model& /*element*/ ) {}
        virtual void Visit( const boost::shared_ptr< core::UserData_ABC >& /*data*/ ) {}
    private:
        std::string& text_;
    };

    void AttributeFunctor( std::ostream& os, int& index, bool generateDoc, const std::string& language,
                           const std::string& key, const core::Model& attribute )
    {
        if( generateDoc && attribute.HasChild( "annotations" ) )
            os << tab << "//! " << ReadAnnotation( attribute[ "annotations" ], language ) << std::endl;

        std::string use = "optional";
        if( attribute.HasChild( "use" ) )
            use = attribute[ "use" ];
        std::string defaultText;
        if( attribute.HasChild( "default" ) && xsd::IsBuiltinXsdType( attribute[ "type" ] ) )
        {
            defaultText = " [default = ";
            ValueVisitor valueVisitor( defaultText );
            attribute[ "default" ].Accept( valueVisitor );
            defaultText += "]";
        }
        os << tab << use << " " << xsd::XsdTypeToProtoType( attribute[ "type" ] ) << " " << xsd::XsdNameToProtoName( key ) << " = " << index++ << defaultText << ";" << std::endl;
    }
 
    void ElementFunctor( std::ostream& os, int& index, bool generateDoc, const std::string& language,
                         const std::string& key, const core::Model& element )
    {
        if( generateDoc && element.HasChild( "annotations" ) )
            os << tab << "//! " << ReadAnnotation( element[ "annotations" ], language ) << std::endl;

        int minOccurs = 1;
        int maxOccurs = 1;
        if( element.HasChild( "minOccurs" ) )
            minOccurs = element[ "minOccurs" ];
        if( element.HasChild( "maxOccurs" ) )
            maxOccurs = element[ "maxOccurs" ];
        std::string use = "invalid";
        if( minOccurs == 0 && maxOccurs == 1 )
            use = "optional";
        else if( maxOccurs == -1 )
            use = "repeated";
        else if( minOccurs == 1 && maxOccurs == 1 )
            use = "required";

        os << tab << use << " " << xsd::XsdTypeToProtoType( element[ "type" ] ) << " " << xsd::XsdNameToProtoName( key ) << " = " << index++ << ";" << std::endl;
    }
    void ChoiceFunctor( std::ostream& enumOs, std::ostream& attributesOs, int& index, bool generateDoc, const std::string& language,
                        const std::string& key, const core::Model& element )
    {
        if( generateDoc && element.HasChild( "annotations" ) )
            attributesOs << tab << "//! " << ReadAnnotation( element[ "annotations" ], language ) << std::endl;

        int maxOccurs = 1;
        if( element.HasChild( "maxOccurs" ) )
            maxOccurs = element[ "maxOccurs" ];
        std::string use = ( maxOccurs == 1 ) ? "optional" : "repeated";

        enumOs << tab << tab << "CHOICE_" << key << " = " << index - 2 << ";" << std::endl;
        attributesOs << tab << use << " " << xsd::XsdTypeToProtoType( element[ "type" ] ) << " " << key << " = " << index++ << ";" << std::endl;
    }

    void TypesFunctor( std::ostream& os, bool generateDoc, const std::string& language, const std::string& key, const core::Model& type )
    {
        if( generateDoc && type.HasChild( "annotations" ) )
            os << "/** @struct " << key << std::endl
                << " *  @brief " << ReadAnnotation( type[ "annotations" ], language ) << std::endl
                << " */" << std::endl;
        const std::string simpleBaseType = ( type.HasChild( "restriction/base" ) ) ? static_cast< std::string >( type[ "restriction/base" ] ) : "";
        const bool isEnum = simpleBaseType == "xs:string" && type.HasChild( "restriction/enumeration" );
        const std::string protoType = isEnum ? "enum " : "message ";
        os << protoType << key << " {" << std::endl;
        if( type[ "type" ] == "simple" )
        {
            if( !simpleBaseType.empty() )
            {
                if( isEnum )
                {
                    const core::Model& enumNode = type[ "restriction/enumeration" ];
                    for( std::size_t i = 0; i < enumNode.GetSize(); ++i )
                        os << tab << xsd::XsdEnumToProtoEnum( enumNode.GetElement( i ) ) << " = " << i << ";" << std::endl;
                }
                else
                    os << tab << "required " << xsd::XsdTypeToProtoType( simpleBaseType ) << " value = 1;" << std::endl;
            }
            else if( type.HasChild( "list" ) )
                os << tab << "repeated " << xsd::XsdTypeToProtoType( type[ "list" ] ) << " values = 1;" << std::endl;
            else if( type.HasChild( "union" ) )
            {
                std::vector< std::string > types;
                const core::Model& unionNode = type[ "union" ];
                for( std::size_t i = 0; i < unionNode.GetSize(); ++i )
                    types.push_back( xsd::XsdTypeToProtoType( unionNode.GetElement( i ) ) );

                os << tab << "enum Type {" << std::endl;
                for( std::size_t i = 0; i < types.size(); ++i )
                    os << tab << tab << "UNION_" << types[ i ] << " = " << i << ";" << std::endl;
                os << tab << "}" << std::endl
                    << std::endl
                    << tab << "required Type type = 1;" << std::endl;
                for( std::size_t i = 0; i < types.size(); ++i )
                    os << tab << "optional " << types[ i ] << " " << types[ i ] << "_value = " << i + 2 << ";" << std::endl;
            }
        }
        else
        {
            assert( type[ "type" ] == "complex" );
            int index = 1;
            if( type.HasChild( "choice" ) )
            {
                std::ostringstream enumOs, attributesOs;
                enumOs << tab << "enum Type {" << std::endl;
                attributesOs << tab << "required Type type = " << index++ << ";" << std::endl;
                core::NamedChildrenVisitor( type[ "choice" ], boost::bind( &ChoiceFunctor, boost::ref( enumOs ), boost::ref( attributesOs ),
                                            boost::ref( index ), boost::cref( generateDoc ), boost::cref( language ), _1, _2 ) );
                enumOs << tab << "}" << std::endl << std::endl;
                os << enumOs.str() << attributesOs.str();
            }
            else
            {
                if( type.HasChild( "sequence" ) )
                    core::NamedChildrenVisitor( type[ "sequence" ], boost::bind( &ElementFunctor, boost::ref( os ),
                                                boost::ref( index ), boost::cref( generateDoc ), boost::cref( language ), _1, _2 ) );
                else if( type.HasChild( "all" ) )
                    core::NamedChildrenVisitor( type[ "all" ], boost::bind( &ElementFunctor, boost::ref( os ),
                                                boost::ref( index ), boost::cref( generateDoc ), boost::cref( language ), _1, _2 ) );
            }
            if( type.HasChild( "attributes" ) )
                core::NamedChildrenVisitor( type[ "attributes" ], boost::bind( &AttributeFunctor, boost::ref( os ),
                                            boost::ref( index ), boost::cref( generateDoc ), boost::cref( language ), _1, _2 ) );
        }
        os << "}" << std::endl
            << std::endl;
    }

    struct FileAnnotation : private boost::noncopyable
    {
        FileAnnotation( const core::Model& schema, const tools::Path& path, std::ostream& os, bool generateDoc, const std::string& language )
            : needAnnotation_( generateDoc && schema.HasChild( "annotations" ) )
            , os_( os )
        {
            if( needAnnotation_ )
                os_ << "/** @file " << path.FileName().ToUTF8() << std::endl
                    << " *  @brief " << ReadAnnotation( schema[ "annotations" ], language ) << std::endl
                    << " *  @{" << std::endl
                    << " */" << std::endl << std::endl;
        }
        ~FileAnnotation()
        {
            if( needAnnotation_ )
                os_ << "/** @}*/" << std::endl;
        }

    private:
        bool needAnnotation_;
        std::ostream& os_;
    };
}

// -----------------------------------------------------------------------------
// Name: xsd::GenerateProto
// Created: ABR 2013-08-09
// -----------------------------------------------------------------------------
void xsd::GenerateProto( const core::Model& schema, const tools::Path& path, const std::string& package, bool generateDoc, const std::string& language )
{
    tools::Ofstream os( path );
    if( generateDoc )
        os << header << std::endl;
    if( schema.HasChild( "includes" ) )
    {
        core::NamedChildrenVisitor( schema[ "includes" ], boost::bind( &ConvertInclude, boost::ref( os ), _1 ) );
        os << std::endl;
    }
    if( !package.empty() )
        os << "package " << package << ";" << std::endl << std::endl;
    FileAnnotation annotation( schema, path, os, generateDoc, language );
    core::NamedChildrenVisitor( schema[ "types" ], boost::bind( &TypesFunctor, boost::ref( os ),
                                boost::cref( generateDoc ), boost::cref( language ), _1, _2 ) );
}
