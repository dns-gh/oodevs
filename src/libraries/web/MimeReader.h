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

namespace io
{
    struct Reader_ABC;
}

namespace runtime
{
    struct Pool_ABC;
}

namespace web
{
class MimeReader
{
public:
     MimeReader();
    ~MimeReader();

    typedef std::function< void( io::Reader_ABC& ) > Handler;

    void PutHeader( const std::string& name, const std::string& value );
    bool IsValid() const;
    void Register( const std::string& name, const Handler& handler );
    void Parse( runtime::Pool_ABC& pool, io::Reader_ABC& src );

private:
    struct Part;
    typedef boost::shared_ptr< Part > T_Part;
    typedef std::vector< T_Part > T_Parts;
    std::string boundary_;
    T_Parts parts_;
};
}

#endif // MIME_READER_H
