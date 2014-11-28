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
#include <boost/shared_ptr.hpp>
#include <memory>
#include <vector>

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
    // Execute a query in the current transaction for
    // inserting the report.
    // If no transaction is open, one will be created.
    void AddReport( const sword::Report& report, int tick );
    void ListReports( sword::ListReportsAck& reports, unsigned int count,
                      const boost::optional< unsigned int >& fromReport,
                      int fromTick,
                      int toTick );
    // If a transaction is open, commits reports in the database.
    void Commit();
    void Save( const tools::Path& filename );
    //@}

private:
    //! @name Member data
    //@{
    std::unique_ptr< tools::Sql_ABC > database_;
    tools::Sql_ABC::T_Transaction transaction_;
    std::vector< sword::Report > reports_;
    //@}
};
}
}

#endif // __Reports_h_
