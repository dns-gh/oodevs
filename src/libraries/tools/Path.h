// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __tools_Path_h_
#define __tools_Path_h_

#pragma warning( push, 0 )
#pragma warning( disable: 4512 ) // No assignment operator found
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/function.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/split_member.hpp>
#pragma warning( pop )

namespace xml
{
    class xostream;
    class xistream;
}

namespace boost
{
    class any;
    template< typename T > class function;
}

namespace tools
{
    class Path;
    bool FileExtractor( const Path& );
    bool DirectoryExtractor( const Path& );

// =============================================================================
/** @class  Path
    @brief  Path
*/
// Created: ABR 2013-02-22
// =============================================================================
class Path
{
public:
    //! @name Types
    //@{
    enum CopyOption
    {
        FailIfExists,
        OverwriteIfExists,
        IgnoreIfExists
    };
    typedef std::vector< Path > T_Paths;
    typedef boost::function< bool( const Path& ) > T_Functor;
    //@}

public:
    //! @name Static method
    //@{
    static Path CurrentPath();                  // boost current path
    static Path TemporaryPath();                // operating system temp directory

    static Path FromUTF8( const std::string& utf8Path );
    static Path FromUnicode( const std::wstring& unicodePath );
    //@}

    //! @name Constructors/Destructor
    //@{
            Path();                             // empty constructor
            Path( const char* hardPath );       // from hard coded path, no conversion
            Path( const wchar_t* hardPath );    // from hard coded path, no conversion
    virtual ~Path();
    //@}

private:
    //! @name Private constructor, use FromXXX methods to avoid wrong conversion
    //@{
            Path( const std::string& encodedPath );                 // from utf8
            Path( const std::wstring& unicodePath );                // from unicode, no conversion
            Path( const boost::filesystem::path& boostPath );       // from boost path, no conversion
    //@}

public:
    //! @name Accessors
    //@{
    const std::string ToLocal() const; // $$$$ ABR 2013-03-13: Terrain and directIA libraries are not Unicode compliant yet, so for now we use this accessor for them (at least compatible with system encoding)
    const std::string ToUTF8() const;
    const std::wstring ToUnicode() const;
    const boost::filesystem::path& ToBoost() const;

    unsigned int FileSize() const;
    std::time_t LastWriteTime() const;
    //@}

    //! @name List operations
    //@{
    T_Paths ListFiles( bool recursive = true, bool relative = true, bool normalized = false ) const;
    T_Paths ListDirectories( bool recursive = true, bool relative = true, bool normalized = false ) const;
    T_Paths ListElements( const T_Functor& validator = T_Functor(), bool recursive = true, bool relative = true, bool normalized = false ) const;
    bool Apply( const T_Functor& functor, bool recursive = true ) const;
    //@}

    //! @name File operations
    //@{
    void Rename( const Path& newPath ) const;
    void Remove() const;
    void RemoveAll() const;
    bool CreateDirectories() const;
    bool Copy( const Path& to, CopyOption option = FailIfExists, const T_Functor& functor = T_Functor() ) const;
    //@}

    //! @name Modifiers
    //@{
    Path& Clear();
    Path& MakePreferred();                          // transform slashes to backslashes
    Path& Normalize();                              // transform backslashes to slashes
    Path MakePreferred() const;                     // transform slashes to backslashes, return a copy
    Path Normalize() const;                         // transform backslashes to slashes, return a copy
    Path& SystemComplete();                         // remove internal ../ and complete path as system would
    Path& ReplaceExtension( const Path& newExtension = Path() );
    //@}

    //! @name Operators
    //@{
    Path& operator=( const Path& p );
    Path operator/( const Path& p ) const;
    Path& operator/=( const Path& p );
    Path operator+( const Path& p ) const;
    Path& operator+=( const Path& p );
    bool operator<( const Path& p ) const;
    bool operator==( const Path& p ) const;
    bool operator!=( const Path& p ) const;
    //@}

    //! @name Decomposition
    //@{
    Path Root() const;                                      // C:/foo/bar.txt   =>   C:/
    Path Parent() const;                                    // C:/foo/bar.txt   =>   C:/foo
    Path FileName() const;                                  // C:/foo/bar.txt   =>   bar.txt
    Path BaseName() const;                                  // C:/foo/bar.txt   =>   bar
    Path Extension() const;                                 // C:/foo/bar.txt   =>   .txt
    Path Absolute( const Path& p = CurrentPath() ) const;   // bar.txt          =>   CurrentPath()/bar.txt
    Path Relative() const;                                  // C:/foo/bar.txt   =>   foo/bar.txt
    Path Relative( const Path& p ) const;                   // C:/foo/bar.txt with p=C:/foo  =>  bar.txt
    //@}

    //! @name Queries
    //@{
    bool Exists() const;
    bool IsDirectory() const;
    bool IsRegularFile() const;
    bool IsEmpty() const;
    bool IsAbsolute() const;
    bool IsRelative() const;
    bool HasFilename() const;
    bool HasExtension() const;
    bool HasRootDirectory() const;
    //@}

    //! @name Iterators
    //@{
    class Iterator
    {
        friend class Path;

    public:
        Iterator();
        Iterator( const Iterator& rhs );
        ~Iterator();

        Iterator& operator++();
        Iterator& operator=( const Iterator& p );
        bool operator==( const Iterator& rhs );
        bool operator!=( const Iterator& rhs );
        Path& operator*();
        Path* operator->();
        const Path& operator*() const;
        const Path* operator->() const;

    private:
        Iterator( const boost::filesystem::path& path );
        void SetInternalPath();

    private:
        boost::filesystem::directory_iterator it_;
        std::auto_ptr< Path > itPath_;
    };

    Iterator begin() const;
    Iterator end() const;
    //@}

    //! @name Serialization
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    template< class Archive > void load( Archive& archive, const unsigned int version );
    template< class Archive > void save( Archive& archive, const unsigned int version ) const;
    //@}

private:
    //! @name Helpers
    //@{
    bool ExtractElement( const T_Functor& functor, tools::Path::T_Paths& result, bool relative, bool normalized, const Path& path ) const;
    //@}

private:
    //! @name Member data
    //@{
    boost::filesystem::path path_;
    //@}
};

// -----------------------------------------------------------------------------
// boost program option validator
// -----------------------------------------------------------------------------
void validate( boost::any& v, const std::vector< std::string >& values, tools::Path*, int );

// -----------------------------------------------------------------------------
// stream manipulator
// -----------------------------------------------------------------------------
std::ostream& operator<<( std::ostream& os, const Path& path );
xml::xostream& operator<<( xml::xostream& xos, const tools::Path& path );
xml::xistream& operator>>( xml::xistream& xis, tools::Path& path );

// -----------------------------------------------------------------------------
// Boost serialize
// -----------------------------------------------------------------------------
template< class Archive >
void Path::load( Archive& archive, const unsigned int )
{
    std::wstring path;
    archive >> path;
    path_ = path;
}

template< class Archive >
void Path::save( Archive& archive, const unsigned int ) const
{
    archive << path_.wstring();
}

} //! namespace tools

BOOST_CLASS_EXPORT_KEY( tools::Path )

#endif // __tools_Path_h_
