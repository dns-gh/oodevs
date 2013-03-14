// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "tools_pch.h"
#include "Path.h"
#include <tools/Exception.h>
#pragma warning( push, 0 )
#include <boost/program_options.hpp>
#pragma warning( pop )
#include <xeumeuleu/xml.hpp>
#include <boost/algorithm/string/replace.hpp>

using namespace tools;

namespace bfs = boost::filesystem;
namespace bpo = boost::program_options;

BOOST_CLASS_EXPORT_IMPLEMENT( tools::Path )

// -----------------------------------------------------------------------------
// Name: Path constructor
// Created: ABR 2013-02-22
// -----------------------------------------------------------------------------
Path::Path()
    : path_()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Path constructor from local
// Created: ABR 2013-02-26
// -----------------------------------------------------------------------------
Path::Path( const char* hardPath )
    : path_( hardPath )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Path constructor
// Created: ABR 2013-03-13
// -----------------------------------------------------------------------------
Path::Path( const wchar_t* hardPath )
    : path_( hardPath )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Path constructor from utf8
// Created: ABR 2013-02-22
// -----------------------------------------------------------------------------
Path::Path( const std::string& encodedPath )
    : path_( FromUtf8ToUnicode( encodedPath ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Path constructor from unicode
// Created: ABR 2013-03-11
// -----------------------------------------------------------------------------
Path::Path( const std::wstring& unicodePath )
    : path_( unicodePath )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Path private constructor from boost path
// Created: ABR 2013-02-22
// -----------------------------------------------------------------------------
Path::Path( const boost::filesystem::path& boostPath )
    : path_( boostPath )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Path destructor
// Created: ABR 2013-02-22
// -----------------------------------------------------------------------------
Path::~Path()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Path::CurrentPath
// Created: ABR 2013-02-22
// -----------------------------------------------------------------------------
Path Path::CurrentPath()
{
    return Path( bfs::current_path() );
}

// -----------------------------------------------------------------------------
// Name: Path::TemporaryPath
// Created: ABR 2013-03-05
// -----------------------------------------------------------------------------
Path Path::TemporaryPath()
{
    return Path( bfs::temp_directory_path() );
}

// -----------------------------------------------------------------------------
// Name: Path::FromUTF8
// Created: ABR 2013-02-22
// -----------------------------------------------------------------------------
Path Path::FromUTF8( const std::string& utf8Path )
{
    return Path( utf8Path );
}

// -----------------------------------------------------------------------------
// Name: Path::FromUnicode
// Created: ABR 2013-03-11
// -----------------------------------------------------------------------------
Path Path::FromUnicode( const std::wstring& unicodePath )
{
    return Path( unicodePath );
}

// -----------------------------------------------------------------------------
// Name: Path::ToLocal
// Created: ABR 2013-02-22
// -----------------------------------------------------------------------------
const std::string Path::ToLocal() const
{
    return FromUnicodeToLocalCharset( path_.wstring() );
}

// -----------------------------------------------------------------------------
// Name: Path::ToUTF8
// Created: ABR 2013-02-22
// -----------------------------------------------------------------------------
const std::string Path::ToUTF8() const
{
    return FromUnicodeToUtf8( path_.wstring() );
}

// -----------------------------------------------------------------------------
// Name: Path::ToUnicode
// Created: ABR 2013-03-11
// -----------------------------------------------------------------------------
const std::wstring Path::ToUnicode() const
{
    return path_.wstring();
}

// -----------------------------------------------------------------------------
// Name: Path::ToBoost
// Created: ABR 2013-02-22
// -----------------------------------------------------------------------------
const bfs::path& Path::ToBoost() const
{
    return path_;
}

// -----------------------------------------------------------------------------
// Name: Path::FileSize
// Created: ABR 2013-02-26
// -----------------------------------------------------------------------------
unsigned int Path::FileSize() const
{
    return static_cast< unsigned int >( bfs::file_size( path_ ) );
}

// -----------------------------------------------------------------------------
// Name: Path::LastWriteTime
// Created: ABR 2013-02-26
// -----------------------------------------------------------------------------
std::time_t Path::LastWriteTime() const
{
    return bfs::last_write_time( path_ );
}

// -----------------------------------------------------------------------------
// Name: Path::ListFiles
// Created: ABR 2013-03-01
// -----------------------------------------------------------------------------
Path::T_Paths Path::ListFiles( bool recursif /* = true */, bool relative /*= true*/, bool normalized /*= false*/  ) const
{
    return ListElements( &FileExtractor, recursif, relative, normalized );
}

// -----------------------------------------------------------------------------
// Name: Path::ListDirectories
// Created: ABR 2013-03-01
// -----------------------------------------------------------------------------
Path::T_Paths Path::ListDirectories( bool recursif /* = true */, bool relative /*= true*/, bool normalized /*= false*/  ) const
{
    return ListElements( &DirectoryExtractor, recursif, relative, normalized );
}

// -----------------------------------------------------------------------------
// Name: Path::ListElements
// Created: ABR 2013-03-07
// -----------------------------------------------------------------------------
Path::T_Paths Path::ListElements( bool recursif /*= true*/, bool relative /*= true*/, bool normalized /*= false*/ ) const
{
    return ListElements( &DefaultExtractor, recursif, relative, normalized );
}

// -----------------------------------------------------------------------------
// Name: Path::Rename
// Created: ABR 2013-03-05
// -----------------------------------------------------------------------------
void Path::Rename( const Path& newPath ) const
{
    try
    {
        bfs::rename( path_, newPath.path_ );
    }
    catch( std::exception& e )
    {
        throw MASA_EXCEPTION( tools::GetExceptionMsg( e ) );
    }
}

// -----------------------------------------------------------------------------
// Name: Path::Delete
// Created: ABR 2013-02-27
// -----------------------------------------------------------------------------
void Path::Remove() const
{
    try
    {
        bfs::remove( path_ );
    }
    catch( std::exception& e )
    {
        throw MASA_EXCEPTION( tools::GetExceptionMsg( e ) );
    }
}

// -----------------------------------------------------------------------------
// Name: Path::DeleteAll
// Created: ABR 2013-02-26
// -----------------------------------------------------------------------------
void Path::RemoveAll() const
{
    try
    {
        bfs::remove_all( path_ );
    }
    catch( std::exception& e )
    {
        throw MASA_EXCEPTION( tools::GetExceptionMsg( e ) );
    }
}

// -----------------------------------------------------------------------------
// Name: Path::CreateDirectories
// Created: ABR 2013-02-26
// -----------------------------------------------------------------------------
bool Path::CreateDirectories() const
{
    bool result = false;
    try
    {
        if( Exists() )
            return result;
        result = bfs::create_directories( path_ );
    }
    catch( std::exception& e )
    {
        throw MASA_EXCEPTION( tools::GetExceptionMsg( e ) );
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: Path::Copy
// Created: ABR 2013-02-26
// -----------------------------------------------------------------------------
bool Path::Copy( const Path& to, CopyOption option /* = FailIfExists */ ) const
{
    return Copy( &DefaultExtractor, to, option );
}

// -----------------------------------------------------------------------------
// Name: Path::Clear
// Created: ABR 2013-02-25
// -----------------------------------------------------------------------------
Path& Path::Clear()
{
    path_.clear();
    return *this;
}

// -----------------------------------------------------------------------------
// Name: Path::MakePreferred
// Created: ABR 2013-02-22
// -----------------------------------------------------------------------------
Path& Path::MakePreferred()
{
    path_.make_preferred();
    return *this;
}

// -----------------------------------------------------------------------------
// Name: Path::Normalize
// Created: ABR 2013-03-07
// -----------------------------------------------------------------------------
Path& Path::Normalize()
{
    std::wstring text = ToUnicode();
    boost::replace_all( text, L"\\", L"/" );
    path_ = text;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: Path::MakePreferred
// Created: ABR 2013-03-13
// -----------------------------------------------------------------------------
Path Path::MakePreferred() const
{
    tools::Path copy = *this;
    return copy.MakePreferred();
    //copy.path_.make_preferred();
    //return copy;
}

// -----------------------------------------------------------------------------
// Name: Path::Normalize
// Created: ABR 2013-03-13
// -----------------------------------------------------------------------------
Path Path::Normalize() const
{
    tools::Path copy = *this;
    return copy.Normalize();
    //std::wstring text = ToUnicode();
    //boost::replace_all( text, L"\\", L"/" );
    //copy.path_ = text;
    //return copy;
}

// -----------------------------------------------------------------------------
// Name: Path::SystemComplete
// Created: ABR 2013-03-04
// -----------------------------------------------------------------------------
Path& Path::SystemComplete()
{
    path_ = bfs::system_complete( path_ );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: Path::ReplaceExtension
// Created: ABR 2013-02-28
// -----------------------------------------------------------------------------
Path& Path::ReplaceExtension( const Path& newExtension /*= Path()*/ )
{
    path_.replace_extension( newExtension.ToBoost() );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: Path::operator=
// Created: ABR 2013-02-22
// -----------------------------------------------------------------------------
Path& Path::operator=( const Path& p )
{
    path_ = p.path_;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: Path::operator/=
// Created: ABR 2013-02-22
// -----------------------------------------------------------------------------
Path& Path::operator/=( const Path& p )
{
    path_ /= p.path_;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: Path::operator/
// Created: ABR 2013-02-22
// -----------------------------------------------------------------------------
Path Path::operator/( const Path& p ) const
{
    return Path( path_ / p.path_ );
}

// -----------------------------------------------------------------------------
// Name: Path::operator+
// Created: ABR 2013-02-26
// -----------------------------------------------------------------------------
Path Path::operator+( const Path& p ) const
{
    Path result = *this;
    result += p;
    return result;
}

// -----------------------------------------------------------------------------
// Name: Path::operator+=
// Created: ABR 2013-02-22
// -----------------------------------------------------------------------------
Path& Path::operator+=( const Path& p )
{
    path_ += p.path_;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: Path::operator<
// Created: ABR 2013-02-22
// -----------------------------------------------------------------------------
bool Path::operator<( const Path& p ) const
{
    return path_ < p.path_;
}

// -----------------------------------------------------------------------------
// Name: Path::operator==
// Created: ABR 2013-02-22
// -----------------------------------------------------------------------------
bool Path::operator==( const Path& p ) const
{
    return path_ == p.path_;
}

// -----------------------------------------------------------------------------
// Name: Path::operator!=
// Created: ABR 2013-02-22
// -----------------------------------------------------------------------------
bool Path::operator!=( const Path& p ) const
{
    return path_ != p.path_;
}

// -----------------------------------------------------------------------------
// Name: Path::Root
// Created: ABR 2013-02-22
// -----------------------------------------------------------------------------
Path Path::Root() const
{
    return Path( path_.root_path() );
}

// -----------------------------------------------------------------------------
// Name: Path::Branch
// Created: ABR 2013-02-22
// -----------------------------------------------------------------------------
Path Path::Parent() const
{
    return Path( path_.parent_path() );
}

// -----------------------------------------------------------------------------
// Name: Path::FileName
// Created: ABR 2013-02-22
// -----------------------------------------------------------------------------
Path Path::FileName() const
{
    return Path( path_.filename() );
}

// -----------------------------------------------------------------------------
// Name: Path::Extension
// Created: ABR 2013-02-22
// -----------------------------------------------------------------------------
Path Path::Extension() const
{
    return Path( path_.extension() );
}

// -----------------------------------------------------------------------------
// Name: Path::BaseName
// Created: ABR 2013-02-26
// -----------------------------------------------------------------------------
Path Path::BaseName() const
{
    return Path( path_.stem() );
}

// -----------------------------------------------------------------------------
// Name: Path::Absolute
// Created: ABR 2013-02-22
// -----------------------------------------------------------------------------
Path Path::Absolute( const Path& p /* = CurrentPath() */ ) const
{
    return ( IsRelative() ) ? p / *this : *this;
}

// -----------------------------------------------------------------------------
// Name: Path::Relative
// Created: ABR 2013-02-22
// -----------------------------------------------------------------------------
Path Path::Relative() const
{
    return Path( path_.relative_path() );
}

// -----------------------------------------------------------------------------
// Name: Path::Relative
// Created: ABR 2013-03-05
// -----------------------------------------------------------------------------
Path Path::Relative( const Path& p ) const
{
    bfs::path result;
    bfs::path::const_iterator it = path_.begin();
    bfs::path::const_iterator otherIt = p.path_.begin();
    while( it != path_.end() )
    {
        if( otherIt != p.path_.end() && *it == *otherIt )
        {
            ++otherIt;
        }
        else if( otherIt == p.path_.end() )
        {
            result /= *it;
        }
        ++it;
    }
    if( !result.empty() )
        return Path( result.relative_path() );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: Path::Exists
// Created: ABR 2013-02-22
// -----------------------------------------------------------------------------
bool Path::Exists() const
{
    return bfs::exists( path_ );
}

// -----------------------------------------------------------------------------
// Name: Path::IsDirectory
// Created: ABR 2013-02-25
// -----------------------------------------------------------------------------
bool Path::IsDirectory() const
{
    return bfs::is_directory( path_ );
}

// -----------------------------------------------------------------------------
// Name: Path::IsRegularFile
// Created: ABR 2013-02-28
// -----------------------------------------------------------------------------
bool Path::IsRegularFile() const
{
    return bfs::is_regular_file( path_ );
}

// -----------------------------------------------------------------------------
// Name: Path::IsEmpty
// Created: ABR 2013-02-22
// -----------------------------------------------------------------------------
bool Path::IsEmpty() const
{
    return path_.empty();
}

// -----------------------------------------------------------------------------
// Name: Path::IsAbsolute
// Created: ABR 2013-02-22
// -----------------------------------------------------------------------------
bool Path::IsAbsolute() const
{
    return path_.is_absolute();
}

// -----------------------------------------------------------------------------
// Name: Path::IsRelative
// Created: ABR 2013-02-22
// -----------------------------------------------------------------------------
bool Path::IsRelative() const
{
    return path_.is_relative();
}

// -----------------------------------------------------------------------------
// Name: Path::HasFilename
// Created: ABR 2013-02-22
// -----------------------------------------------------------------------------
bool Path::HasFilename() const
{
    return path_.has_filename();
}

// -----------------------------------------------------------------------------
// Name: Path::HasExtension
// Created: ABR 2013-02-22
// -----------------------------------------------------------------------------
bool Path::HasExtension() const
{
    return path_.has_extension();
}

// -----------------------------------------------------------------------------
// Name: Path::HasRootDirectory
// Created: ABR 2013-02-25
// -----------------------------------------------------------------------------
bool Path::HasRootDirectory() const
{
    return path_.has_root_directory();
}


// -----------------------------------------------------------------------------
// Extractors
// -----------------------------------------------------------------------------
bool tools::DefaultExtractor( const Path& )
{
    return true;
}

bool tools::FileExtractor( const Path& path )
{
    return path.IsRegularFile();
}

bool tools::DirectoryExtractor( const Path& path )
{
    return path.IsDirectory();
}


// -----------------------------------------------------------------------------
// Boost program option validator
// -----------------------------------------------------------------------------
void tools::validate( boost::any& v, const std::vector< std::string >& values, Path*, int )
{
    if( values.size() != 1 )
        throw bpo::validation_error( bpo::validation_error::invalid_option_value, "Invalid path specification" );

    Path path;
    try
    {
        path = Path::FromUTF8( values.at( 0 ).c_str() );
    }
    catch( std::exception& )
    {
        path = values.at( 0 ).c_str();
    }
    v = path;
}


// -----------------------------------------------------------------------------
// Stream manipulators
// -----------------------------------------------------------------------------
std::ostream& tools::operator<<( std::ostream& os, const Path& path )
{
    return os << "\"" << path.ToUTF8().c_str() << "\"";
}

xml::xostream& tools::operator<<( xml::xostream& xos, const tools::Path& path )
{
    std::string text = path.ToUTF8();
    std::replace( text.begin(), text.end(), '\\', '/' );
    return xos << text;
}

xml::xistream& tools::operator>>( xml::xistream& xis, tools::Path& path )
{
    std::string tmp;
    xis >> tmp;
    path = tools::Path::FromUTF8( tmp );
    return xis;
}


// -----------------------------------------------------------------------------
// Iterators
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: Path::begin
// Created: ABR 2013-02-27
// -----------------------------------------------------------------------------
Path::Iterator Path::begin() const
{
    return Iterator( path_ );
}

// -----------------------------------------------------------------------------
// Name: Path::end
// Created: ABR 2013-02-27
// -----------------------------------------------------------------------------
Path::Iterator Path::end() const
{
    return Iterator();
}

// -----------------------------------------------------------------------------
// Name: Path::Iterator constructor
// Created: ABR 2013-02-27
// -----------------------------------------------------------------------------
Path::Iterator::Iterator()
    : it_()
    , itPath_( new Path() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Path::Iterator constructor
// Created: ABR 2013-02-27
// -----------------------------------------------------------------------------
Path::Iterator::Iterator( const Path::Iterator& rhs )
    : it_( rhs.it_ )
    , itPath_( 0 )
{
    SetInternalPath();
}

// -----------------------------------------------------------------------------
// Name: Path::Iterator constructor
// Created: ABR 2013-02-27
// -----------------------------------------------------------------------------
Path::Iterator::Iterator( const bfs::path& path )
    : it_( path )
    , itPath_( 0 )
{
    SetInternalPath();
}

// -----------------------------------------------------------------------------
// Name: Path::Iterator destructor
// Created: ABR 2013-02-27
// -----------------------------------------------------------------------------
Path::Iterator::~Iterator()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Path::Iterator::SetInternalPath
// Created: ABR 2013-02-27
// -----------------------------------------------------------------------------
void Path::Iterator::SetInternalPath()
{
    itPath_.reset( ( it_ != bfs::directory_iterator() ) ? new Path( it_->path() ) : 0 );
}

// -----------------------------------------------------------------------------
// Name: Path::Iterator::operator++
// Created: ABR 2013-02-27
// -----------------------------------------------------------------------------
Path::Iterator& Path::Iterator::operator++()
{
    ++it_;
    SetInternalPath();
    return *this;
}

// -----------------------------------------------------------------------------
// Name: Path::Iterator::operator=
// Created: ABR 2013-02-27
// -----------------------------------------------------------------------------
Path::Iterator& Path::Iterator::operator=( const Iterator& p )
{
    it_ = p.it_;
    SetInternalPath();
    return *this;
}

// -----------------------------------------------------------------------------
// Name: Path::Iterator::operator==
// Created: ABR 2013-02-27
// -----------------------------------------------------------------------------
bool Path::Iterator::operator==( const Path::Iterator& rhs )
{
    return it_ == rhs.it_;
}

// -----------------------------------------------------------------------------
// Name: Path::Iterator::operator!=
// Created: ABR 2013-02-27
// -----------------------------------------------------------------------------
bool Path::Iterator::operator!=( const Path::Iterator& rhs )
{
    return it_ != rhs.it_;
}

// -----------------------------------------------------------------------------
// Name: Path::Iterator::operator*
// Created: ABR 2013-02-27
// -----------------------------------------------------------------------------
Path& Path::Iterator::operator*()
{
    if( itPath_.get() == 0 )
        throw MASA_EXCEPTION( "Trying to acces to an invalid Path::Iterator" );
    return *itPath_;
}

// -----------------------------------------------------------------------------
// Name: Path::Iterator::operator->
// Created: ABR 2013-02-27
// -----------------------------------------------------------------------------
Path* Path::Iterator::operator->()
{
    return itPath_.get();
}

// -----------------------------------------------------------------------------
// Name: Path::Iterator::operator*
// Created: ABR 2013-02-27
// -----------------------------------------------------------------------------
const Path& Path::Iterator::operator*() const
{
    if( it_ != bfs::directory_iterator() )
        throw MASA_EXCEPTION( "Trying to acces to an invalid Path::Iterator" );
    return *itPath_;
}

// -----------------------------------------------------------------------------
// Name: Path::Iterator::operator->
// Created: ABR 2013-02-27
// -----------------------------------------------------------------------------
const Path* Path::Iterator::operator->() const
{
    return itPath_.get();
}
