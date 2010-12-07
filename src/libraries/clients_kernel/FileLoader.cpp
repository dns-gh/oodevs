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
    , invalidSignatureFiles_( 0 )
    , missingSignatureFiles_( 0 )
    , addToCRC_             ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FileLoader constructor
// Created: SBO 2009-08-20
// -----------------------------------------------------------------------------
FileLoader::FileLoader( const tools::ExerciseConfig& config, std::string& invalidSignatureFiles, std::string& missingSignatureFiles )
    : config_               ( config )
    , invalidSignatureFiles_( &invalidSignatureFiles )
    , missingSignatureFiles_( &missingSignatureFiles )
    , addToCRC_             ( false )
{
    std::string filename = config.GetPhysicalFile();
    CheckSignatures( filename, invalidSignatureFiles, missingSignatureFiles );
}

// -----------------------------------------------------------------------------
// Name: FileLoader destructor
// Created: SBO 2009-08-20
// -----------------------------------------------------------------------------
FileLoader::~FileLoader()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FileLoader::AddToCRC
// Created: LDC 2010-11-30
// -----------------------------------------------------------------------------
void FileLoader::AddToCRC()
{
    addToCRC_ = true;
}

// -----------------------------------------------------------------------------
// Name: FileLoader::CheckSignatures
// Created: LDC 2010-11-29
// -----------------------------------------------------------------------------
void FileLoader::CheckSignatures( const std::string& file, std::string& invalidSignatureFiles, std::string& missingSignatureFiles ) const
{
    tools::EXmlCrc32SignatureError error = tools::CheckXmlCrc32Signature( file );
    if( error == tools::eXmlCrc32SignatureError_Invalid )
        invalidSignatureFiles.append( "\n" + bfs::path( file, bfs::native ).leaf() );
    else if( error == tools::eXmlCrc32SignatureError_NotSigned )
        missingSignatureFiles.append( "\n" + bfs::path( file, bfs::native ).leaf() );
}

namespace
{
    struct CheckedLoader
    {
        typedef boost::function< void ( xml::xisubstream ) > T_Loader;

        CheckedLoader( const std::string& path, const tools::ExerciseConfig& config, T_Loader loader, const std::string& xslTransform, std::string* invalidSignatureFiles, std::string* missingSignatureFiles )
            : path_( path )
            , config_( config )
            , loader_( loader )
        {
            if( invalidSignatureFiles && missingSignatureFiles )
            {
                tools::EXmlCrc32SignatureError error = tools::CheckXmlCrc32Signature( path_ );
                if( error == tools::eXmlCrc32SignatureError_Invalid )
                    invalidSignatureFiles->append( "\n" + bfs::path( path_, bfs::native ).leaf() );
                else if( error == tools::eXmlCrc32SignatureError_NotSigned )
                    missingSignatureFiles->append( "\n" + bfs::path( path_, bfs::native ).leaf() );
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
    return LoadAndUpdate( rootTag, loader, empty );
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
// Name: FileLoader::GetFile
// Created: LDC 2010-11-29
// -----------------------------------------------------------------------------
void FileLoader::GetFile( const std::string& rootTag, xml::xistream& xis, std::string& file ) const
{
    AttributeReader fileAttributeReader( rootTag, file );
    xis >> xml::list( fileAttributeReader, &AttributeReader::LoadTag );
}

// -----------------------------------------------------------------------------
// Name: FileLoader::Check
// Created: LDC 2010-11-29
// -----------------------------------------------------------------------------
void FileLoader::Check( const std::string& file, T_Loader loader, const std::string& xslTransform )
{
    CheckedLoader( file, config_, loader, xslTransform, invalidSignatureFiles_, missingSignatureFiles_ );
    if( addToCRC_ )
        (const_cast< tools::ExerciseConfig& >( config_ ) ).AddFileToCRC( file ); // $$$$ LDC FIXME
}