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
#include <boost/optional.hpp>
#include <boost/noncopyable.hpp>
#include <memory>

namespace tools
{
    class Path;
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
    explicit Reports( const tools::Path& filename );
    virtual ~Reports();
    //@}

    //! @name Operations
    //@{
    void AddReport( const sword::Report& report );
    void ListReports( sword::ListReportsAck& reports, unsigned int count,
                      const boost::optional< unsigned int >& fromReport,
                      const boost::optional< unsigned int >& untilTick );
    void Save( const tools::Path& filename );
    void SetTick( int tick );
    //@}

private:
    //! @name Member data
    //@{
    std::unique_ptr< tools::Sql_ABC > database_;
    int currentTick_;
    //@}
};
}
}

#endif // __Reports_h_
