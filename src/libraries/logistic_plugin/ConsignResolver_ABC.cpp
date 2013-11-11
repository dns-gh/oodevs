// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "ConsignResolver_ABC.h"
#include "ConsignWriter.h"
#include "clients_kernel/Tools.h"
#include "tools/FileWrapper.h"
#include <boost/regex.hpp>

namespace bg = boost::gregorian;

namespace plugins
{
namespace logistic
{

std::wstring EscapeRegex( const std::wstring& s )
{
    std::wstring escaped;
    escaped.reserve( 2*s.size() );
    for( size_t i = 0; i != s.size(); ++i )
    {
        if( !std::isalnum( s[i] ))
            escaped.push_back( L'\\' );
        escaped.push_back( s[i] );
    }
    return escaped;
}

// -----------------------------------------------------------------------------
// Name: ConsignResolver_ABC constructor
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
ConsignResolver_ABC::ConsignResolver_ABC( const tools::Path& name, const NameResolver_ABC& nameResolver )
    : name_( name ), nameResolver_( nameResolver ), curFileIndex_( 0 ), curLineIndex_( 0 ), maxLinesInFile_( 50000 ), daysBeforeToKeep_( 1 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ConsignResolver_ABC destructor
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
ConsignResolver_ABC::~ConsignResolver_ABC()
{
}

// -----------------------------------------------------------------------------
// Name: ConsignResolver_ABC::Receive
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
bool ConsignResolver_ABC::Receive( const sword::SimToClient& message, ConsignData_ABC& consign,
        const boost::gregorian::date& today )
{
    if( const auto data = ManageMessage( message, consign ) )
    {
        if( !data->empty() )
        {
            CheckOutputFile( today );
            if( output_.is_open() )
                output_ << *data << std::flush;
        }
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: ConsignResolver_ABC::SetNewFile
// Created: MMC 2012-08-27
// -----------------------------------------------------------------------------
void ConsignResolver_ABC::SetNewFile( const boost::gregorian::date& today )
{
    if( today != fileDate_ )
    {
        if( fileDate_ == boost::gregorian::date() )
            InitFileIndex( today );
        else
            curFileIndex_ = 0;
    }
    fileName_ = name_;
    fileName_.ReplaceExtension( tools::Path::FromUTF8( "." + to_iso_string( today ) + "." + boost::lexical_cast< std::string >( curFileIndex_++ ) + ".csv" ) );
    curLineIndex_ = 0;
    fileDate_ = today;
}

// -----------------------------------------------------------------------------
// Name: ConsignResolver_ABC::InitFileIndex
// Created: MMC 2012-09-12
// -----------------------------------------------------------------------------
void ConsignResolver_ABC::InitFileIndex( const boost::gregorian::date& today )
{
    boost::wregex todayRegex( EscapeRegex( name_.FileName().ToUnicode() ) + L"\\." + to_iso_wstring( today ) + L"\\.(\\d+)\\.csv$");
    boost::wsmatch m;
    for( auto it = name_.Parent().begin(); it != name_.end(); ++it )
    {
        if( it->IsRegularFile() )
        {
            const std::wstring fileName = it->FileName().ToUnicode();
            if( boost::regex_match( fileName, m, todayRegex ) )
            {
                const int curIndex = boost::lexical_cast< int >( m.str( 1 ) ) + 1;
                if( curIndex > curFileIndex_ )
                    curFileIndex_ = curIndex;
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ConsignResolver_ABC::RemoveOldFiles
// Created: MMC 2012-08-27
// -----------------------------------------------------------------------------
void ConsignResolver_ABC::RemoveOldFiles( const boost::gregorian::date& today )
{
    boost::wregex fileRegex( EscapeRegex( name_.FileName().ToUnicode() ) + L"\\.(\\d{8})\\.\\d+\\.csv$");
    const std::wstring minDate = to_iso_wstring( today - bg::days( daysBeforeToKeep_ ) );

    boost::wsmatch m;
    std::vector< tools::Path > filesToRemove;
    for( auto it = name_.Parent().begin(); it != name_.end(); ++it )
    {
        if( it->IsRegularFile() )
        {
            const std::wstring fileName = it->FileName().ToUnicode();
            // ISO date lexicographic order matches the chronological one
            if( boost::regex_match( fileName, m, fileRegex ) && m.str(1) < minDate )
                filesToRemove.push_back( *it );
        }
    }

    for( auto it = filesToRemove.begin(); it != filesToRemove.end(); ++it )
    {
        try
        {
            it->Remove();
        }
        catch( ... )
        {
            // NOTHING
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ConsignResolver_ABC::OpenFile
// Created: MMC 2012-08-27
// -----------------------------------------------------------------------------
void ConsignResolver_ABC::OpenFile()
{
    if( output_.is_open() )
        output_.close();
    if( !fileName_.IsEmpty() )
    {
        try
        {
            if( fileName_.Exists() )
            {
                if( !output_.is_open() )
                    output_.open( fileName_, std::ios_base::out | std::ios_base::app );
            }
            else
            {
                if( output_.is_open() )
                    output_.close();
                output_.open( fileName_, std::ios_base::out | std::ios_base::app );
                output_ << header_;
            }
        }
        catch( ... )
        {
            // NOTHING
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ConsignResolver_ABC::CheckOutputFile
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
void ConsignResolver_ABC::CheckOutputFile( const boost::gregorian::date& today )
{
    if( fileDate_ != today || curLineIndex_ >= maxLinesInFile_ )
    {
        SetNewFile( today );
        OpenFile();
        RemoveOldFiles( today );
    }
}

// -----------------------------------------------------------------------------
// Name: ConsignResolver_ABC::GetNameResolver
// Created: PMD 2012-09-02
// -----------------------------------------------------------------------------
const NameResolver_ABC& ConsignResolver_ABC::GetNameResolver() const
{
    return nameResolver_;
}

// -----------------------------------------------------------------------------
// Name: ConsignResolver_ABC::SetHeader
// Created: MMC 2012-08-23
// -----------------------------------------------------------------------------
void ConsignResolver_ABC::SetHeader( const ConsignData_ABC& consign )
{
    ConsignWriter writer;
    consign.WriteConsign( writer );
    header_ = writer.GetLine();
}

}  // namespace logistic
}  // namespace plugins
