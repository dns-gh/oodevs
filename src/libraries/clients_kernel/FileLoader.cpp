// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "FileLoader.h"
#include "tools/ExerciseConfig.h"
#include <boost/filesystem.hpp>
#include <tools/XmlCrc32Signature.h>
#include <xeumeuleu/xml.hpp>
#include <xeuseuleu/xsl.hpp>

using namespace kernel;
namespace bfs = boost::filesystem;

// -----------------------------------------------------------------------------
// Name: FileLoader constructor
// Created: JSR 2010-11-26
// -----------------------------------------------------------------------------
FileLoader::FileLoader( const tools::ExerciseConfig& config )
    : config_               ( config )
    , xis_                  ( new xml::xifstream( config.GetPhysicalFile() ) )
    , invalidSignatureFiles_( 0 )
{
}

// -----------------------------------------------------------------------------
// Name: FileLoader constructor
// Created: SBO 2009-08-20
// -----------------------------------------------------------------------------
FileLoader::FileLoader( const tools::ExerciseConfig& config, std::string& invalidSignatureFiles )
    : config_               ( config )
    , xis_                  ( new xml::xifstream( config.GetPhysicalFile() ) )
    , invalidSignatureFiles_( &invalidSignatureFiles )
{
    std::string filename = config.GetPhysicalFile();
    tools::EXmlCrc32SignatureError error = tools::CheckXmlCrc32Signature( filename );
    if( error == tools::eXmlCrc32SignatureError_Invalid || error == tools::eXmlCrc32SignatureError_NotSigned )
        invalidSignatureFiles_->append( "\n" + bfs::path( filename, bfs::native ).leaf() );
}

// -----------------------------------------------------------------------------
// Name: FileLoader destructor
// Created: SBO 2009-08-20
// -----------------------------------------------------------------------------
FileLoader::~FileLoader()
{
    // NOTHING
}

namespace
{
    struct CheckedLoader
    {
        typedef boost::function< void ( xml::xisubstream ) > T_Loader;

        CheckedLoader( const std::string& path, const tools::ExerciseConfig& config, T_Loader loader, const std::string& xslTransform, std::string* invalidSignatureFiles )
            : path_( path )
            , config_( config )
            , loader_( loader )
        {
            if( invalidSignatureFiles )
            {
                tools::EXmlCrc32SignatureError error = tools::CheckXmlCrc32Signature( path_ );
                if( error == tools::eXmlCrc32SignatureError_Invalid || error == tools::eXmlCrc32SignatureError_NotSigned )
                    invalidSignatureFiles->append( "\n" + bfs::path( path_, bfs::native ).leaf() );
            }
            xml::xifstream xis( path_ );
            LoadFile( xis, xslTransform );
        }

    private:
        void LoadFile( xml::xistream& xis, const std::string& xslTransform )
        {
            const std::string schema = xis.attribute< std::string >( "xsi:noNamespaceSchemaLocation", "" );
            if( !xslTransform.empty() )
            {
                xsl::xstringtransform xst( xslTransform );
                xst << xml::xifstream( path_ );
                std::string updatedFile = xst.str();
                if( schema.empty() )
                    loader_( xml::xistringstream( updatedFile ) );
                else
                {
                    CheckedLoader::GetModelVersion( schema );
                    loader_( xml::xistringstream( updatedFile, xml::external_grammar( config_.BuildResourceChildFile( schema ) ) ) );
                }
            }
            else
            {
                if( schema.empty() )
                    loader_( xml::xifstream( path_ ) );
                else
                {
                    CheckedLoader::GetModelVersion( schema );
                    loader_( xml::xifstream( path_, xml::external_grammar( config_.BuildResourceChildFile( schema ) ) ) );
                }
            }
        }

        void GetModelVersion( const std::string& grammarPath )
        {
            xml::xifstream xis( config_.BuildResourceChildFile( grammarPath ) );
            xis >> xml::start( "xs:schema" )
                    >> xml::optional >> xml::attribute( "version", modelVersion_ )
                >> xml::end;
        }

        CheckedLoader& operator=( const CheckedLoader& );
        const std::string path_;
        const tools::ExerciseConfig& config_;
        std::string modelVersion_;
        T_Loader loader_;
    };
}

// -----------------------------------------------------------------------------
// Name: FileLoader::Load
// Created: LDC 2010-11-25
// -----------------------------------------------------------------------------
FileLoader& FileLoader::Load( const std::string& rootTag, T_Loader loader )
{
    std::string empty;
    return Load( rootTag, loader, empty );
}

// -----------------------------------------------------------------------------
// Name: FileLoader::LoadExercise
// Created: LDC 2010-11-25
// -----------------------------------------------------------------------------
FileLoader& FileLoader::LoadExercise( const std::string& rootTag, T_Loader loader )
{
    std::string empty;
    return LoadExercise( rootTag, loader, empty );
}

namespace
{
    class AttributeReader
    {
    public:
        AttributeReader( const std::string& rootTag, std::string& file )
            : rootTag_( rootTag )
            , file_( file )
        {
            // NOTHING
        }
        void LoadTag( const std::string&, xml::xistream& xis )
        {
            xis >> xml::start( rootTag_ ) >> xml::attribute( "file", file_ ) >> xml::end;
        }
    private:
        AttributeReader& operator= ( const AttributeReader& );

        const std::string& rootTag_;
        std::string& file_;
    };
}

// -----------------------------------------------------------------------------
// Name: FileLoader::Load
// Created: SBO 2009-08-20
// -----------------------------------------------------------------------------
FileLoader& FileLoader::Load( const std::string& rootTag, T_Loader loader, const std::string& xslTransform )
{
    std::string file;
    AttributeReader fileAttributeReader( rootTag, file );
    *xis_ >> xml::list( fileAttributeReader, &AttributeReader::LoadTag );
    CheckedLoader( config_.BuildPhysicalChildFile( file ), config_, loader, xslTransform, invalidSignatureFiles_ );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: FileLoader::LoadExercise
// Created: LDC 2010-11-25
// -----------------------------------------------------------------------------
FileLoader& FileLoader::LoadExercise( const std::string& rootTag, T_Loader loader, const std::string& xslTransform )
{
    xml::xifstream xis( config_.GetExerciseFile() );
    std::string file;
    AttributeReader fileAttributeReader( rootTag, file );
    xis >> xml::list( fileAttributeReader, &AttributeReader::LoadTag );
    CheckedLoader( config_.BuildExerciseChildFile( file ), config_, loader, xslTransform, invalidSignatureFiles_ );
    return *this;
}
