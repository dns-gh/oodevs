// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef MIME_READER_H
#define MIME_READER_H

#include <boost/shared_ptr.hpp>
#include <vector>

namespace web
{
class MimeReader
{
public:
     MimeReader();
    ~MimeReader();

    void PutHeader( const std::string& name, const std::string& value );
    bool IsValid() const;
    std::istream& Register( const std::string& name );
    void Parse( std::istream& src );

private:
    struct Part;
    typedef boost::shared_ptr< Part > T_Part;
    typedef std::vector< T_Part > T_Parts;
    std::string boundary_;
    T_Parts parts_;
};
}

#endif // MIME_READER_H
