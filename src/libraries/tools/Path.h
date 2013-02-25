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

#include <boost/filesystem/path.hpp>

namespace xml
{
    class xostream;
}

namespace tools
{

// =============================================================================
/** @class  Path
    @brief  Path
*/
// Created: ABR 2013-02-22
// =============================================================================
class Path
{
public:
    //! @name Static method
    //@{
    static Path CurrentPath();              // boost current path
    static Path FromUTF8( const std::string& path );
    static Path FromSystem( const std::string& path );
    //@}

    //! @name Constructors/Destructor
    //@{
            Path();
    virtual ~Path();
    //@}

    //! @name String conversion
    //@{
    const std::string ToStringSystem();
    const std::string ToStringUTF8();
    //@}

    //! @name Operations
    //@{
    bool Exists() const;
    Path& MakePreferred();              // transform slashes to backslashes
    const boost::filesystem::path& ToBoost() const;
    //@}

    //! @name Operators
    //@{
    Path& operator=( const Path& p );
    Path& operator/=( const Path& p );
    Path operator/( const Path& p ) const;
    Path& operator+=( const Path& p );
    bool operator==( const Path& p ) const;
    bool operator!=( const Path& p ) const;
    std::ostream& operator<<( std::ostream& os );
    xml::xostream& operator<<( xml::xostream& os );
   //@}

    //! @name Decomposition
    //@{
    Path Root() const;                                      // C:/foo/bar.txt   =>   C:/
    Path Parent() const;                                    // C:/foo/bar.txt   =>   C:/foo
    Path FileName() const;                                  // C:/foo/bar.txt   =>   bar.txt
    Path Extension() const;                                 // C:/foo/bar.txt   =>   txt
    Path Relative() const;                                  // C:/foo/bar.txt   =>   foo/bar.txt
    Path Absolute() const;                                  // bar.txt          =>   CurrentDirectory()/bar.txt
    //@}

    //! @name Query
    //@{
    bool IsEmpty() const;
    bool IsAbsolute() const;
    bool IsRelative() const;
    bool HasFilename() const;
    bool HasExtension() const;
    //@}

private:
    //! @name Types
    //@{
    enum Encoding
    {
        UTF8,
        System
    };
    //@}

private:
    //! @name Internal constructor
    //@{
    explicit Path( const boost::filesystem::path& boostPath );
    explicit Path( const std::string& sourceText, Encoding sourceEncoding );
    //@}

    //! @name Helpers
    //@{
    const std::string ToString( Encoding outputEncoding );
    //@}

private:
    //! @name Member data
    //@{
    boost::filesystem::path path_;
    //@}
};

} //! namespace tools

#endif // __tools_Path_h_
