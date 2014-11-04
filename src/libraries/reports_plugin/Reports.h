// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __Reports_h_
#define __Reports_h_

#include "tools/Sql_ABC.h"
#include <boost/noncopyable.hpp>
#include <memory>

namespace tools
{
    class SessionConfig;
}

namespace dispatcher
{
    class Model_ABC;
}

namespace sword
{
    class ListReportsAck;
    class Report;
}

namespace plugins
{
namespace reports
{
class Reports : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Reports( const tools::SessionConfig& config, const dispatcher::Model_ABC& model );
    virtual ~Reports();
    //@}

    //! @name Operations
    //@{
    void AddReport( const sword::Report& report );
    void ListReports( sword::ListReportsAck& reports,
                      unsigned int count, unsigned int from );
    void Save( const std::string& path );
    void Update( int tick );
    //@}

private:
    //! @name Member data
    //@{
    std::unique_ptr< tools::Sql_ABC > database_;
    const tools::SessionConfig& config_;
    const dispatcher::Model_ABC& model_;
    int tick_;
    //@}
};
}
}

#endif // __Reports_h_
