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

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>

namespace host
{
    class Pool_ABC;
}

namespace web
{
class MimeReader
{
public:
     MimeReader();
    ~MimeReader();

    typedef boost::function< void( std::istream& ) > Handler;

    void PutHeader( const std::string& name, const std::string& value );
    bool IsValid() const;
    void Register( const std::string& name, const Handler& handler );
    void Parse( host::Pool_ABC& pool, std::istream& src );

private:
    struct Part;
    typedef boost::shared_ptr< Part > T_Part;
    typedef std::vector< T_Part > T_Parts;
    std::string boundary_;
    T_Parts parts_;
};
}

#endif // MIME_READER_H
