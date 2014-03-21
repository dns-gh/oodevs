// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "core/Model.h"
#include "xeuseudeu/XsdToModel.h"
#include "xeuseudeu/GenerateProto.h"
#include <tools/Exception.h>
#include <tools/Path.h>

#pragma warning( disable: 4100 4512 )
#include <boost/algorithm/string.hpp>
#include <boost/bind.hpp>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

namespace
{
    int InputError( const std::string& message )
    {
        std::cerr << message << std::endl;
        return EXIT_FAILURE;
    }
    bool IsXsdFile( const tools::Path& file )
    {
        return file.IsRegularFile() && file.Extension() == ".xsd";
    }
    bool XsdToProto( const tools::Path& inputFile, const tools::Path& output, const tools::Path& relativeDir, const std::string& package, bool generateDoc, const std::string& language )
    {
        if( !IsXsdFile( inputFile ) )
            return false;
        std::cout << "Proto file generation" << std::endl;
        std::cout << "> Parsing input file " << inputFile << std::endl;
        core::Model model;
        xsd::XsdToModel( inputFile, model );

        const std::string packageName = boost::to_lower_copy( package.empty() ? inputFile.BaseName().ToUTF8() : package );
        tools::Path outputFile = output;
        if( output.IsRegularFile() && output.Extension() != ".proto" )
            throw MASA_EXCEPTION( "Invalid parameter, --output should be a .proto file if it's not a directory" );
        if( output.IsDirectory() )
            outputFile = relativeDir.IsEmpty() ? output / inputFile.BaseName() + ".proto"
                                               : output / ( inputFile.Relative( relativeDir ) ).ReplaceExtension( ".proto" );
        if( !outputFile.Parent().Exists() )
            outputFile.Parent().CreateDirectories();
        std::cout << "> Generating output file " << outputFile << std::endl;
        xsd::GenerateProto( model[ "schemas" ][ inputFile.BaseName().ToUTF8() ], outputFile, packageName, generateDoc, language );
        return false;
    }
    bool XsdToCppWrapper( const tools::Path& /*inputFile*/, const tools::Path& /*outputDir*/, const tools::Path& /*relativeDir*/, const std::string& /*package*/, bool /*generateDoc*/, const std::string& /*language*/  )
    {
        // TODO
        return true;
    }
    void ApplyFunctor( const tools::Path& input, const tools::Path::T_Functor& functor )
    {
        if( input.IsDirectory() )
            input.Apply( functor );
        else
            functor( input );
    }
}

int main( int argc, char** argv )
{
    std::string language;
    std::string package;
    tools::Path input;
    tools::Path output;
    try
    {
        po::options_description desc( "Allowed options" );
        desc.add_options()
            ( "help,h", "display help" )
            ( "generate-proto", "generate protobuf file(s) according to input xsd file(s)" )
            ( "generate-cpp-wrapper", "generate cpp wrapper file(s) according to input xsd file(s)" )
            ( "do-not-generate-doc", "do not generate documentation in the resulting file(s)" )
            ( "do-not-keep-relative-path", "do not keep relative hierarchy of the input directory to the output" )
            ( "input,i" ,   po::value( &input )->default_value( "" ),       "specify input file or directory" )
            ( "output,o",   po::value( &output )->default_value( "" ),      "specify output file or directory (if output is a file, input must be a file)" )
            ( "language,l", po::value( &language )->default_value( "en" ),  "the language of the generated documentation" )
            ( "package,p",  po::value( &package )->default_value( "" ),     "the destination package/namespace (if empty, the input filename or input directory name will be used)" );
        po::variables_map vm;
        po::store( po::command_line_parser( argc, argv ).options( desc ).allow_unregistered().run(), vm );
        po::notify( vm );
        if( vm.count( "help" ) )
        {
            std::cout << desc;
            return EXIT_FAILURE;
        }
        if( vm.count( "generate-proto" ) == 0 && vm.count( "generate-cpp-wrapper" ) == 0 )
            return InputError( "Invalid parameter, --generate-proto or --generate-cpp-wrapper must be provide" );
        if( !input.Exists() )
            return InputError( "Invalid parameter, --input must be a valid file or directory" );
        if( output.IsRegularFile() && !input.IsRegularFile() )
            return InputError( "Invalid parameter, --input must be a single file if --output is a single file" );
        if( input.IsRegularFile() && !IsXsdFile( input ) || input.IsDirectory() && input.ListElements( boost::bind( &IsXsdFile, _1 ) ).size() == 0 )
            return InputError( "Invalid parameter, --input must be a xsd file or a directory containing some" );
        const tools::Path relativeDir = vm.count( "do-not-keep-relative-path" ) ? tools::Path() : input;
        bool generateDoc = vm.count( "do-not-generate-doc" ) == 0;
        ApplyFunctor( input, vm.count( "generate-proto" ) ? boost::bind( &XsdToProto, _1, boost::cref( output ), boost::cref( relativeDir ), boost::cref( package ), generateDoc, boost::cref( language ) )
                                                          : boost::bind( &XsdToCppWrapper, _1, boost::cref( output ), boost::cref( relativeDir ), boost::cref( package ), generateDoc, boost::cref( language ) ) );
    }
    catch( const std::exception& e )
    {
        return InputError( tools::GetExceptionMsg( e ) );
    }
    return EXIT_SUCCESS;
}
