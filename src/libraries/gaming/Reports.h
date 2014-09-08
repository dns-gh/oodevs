// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Reports_h_
#define __Reports_h_

#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Displayable_ABC.h"
#include <tools/Map.h>
#include <boost/shared_ptr.hpp>

namespace kernel
{
    class Controller;
    class Entity_ABC;
    class Displayer_ABC;
}

namespace sword
{
    class Report;
    class InvalidateReport;
    class Trace;
}

class Report;
class ReportFactory;

// =============================================================================
/** @class  Reports
    @brief  Reports
    // $$$$ AGE 2007-10-22: utiliser un resolver. Uniformiser avec les traces
*/
// Created: AGE 2006-02-13
// =============================================================================
class Reports : public kernel::Extension_ABC
              , public kernel::Updatable_ABC< sword::Report >
              , public kernel::Updatable_ABC< sword::InvalidateReport >
              , public kernel::Updatable_ABC< sword::Trace >
              , public kernel::Displayable_ABC
{
public:
    //! @name Types
    //@{
    typedef std::vector< Report* >      T_Reports;

    typedef boost::shared_ptr< Report > T_TextReport;
    typedef tools::Map< unsigned int, T_TextReport > T_TextReports;
    //@}

    //! @name Constructors/Destructor
    //@{
             Reports( const kernel::Entity_ABC& entity, kernel::Controller& controller, const ReportFactory& reportFactory );
    virtual ~Reports();
    //@}

    //! @name Operations
    //@{
    virtual void DisplayInTooltip( kernel::Displayer_ABC& displayer ) const;

    const T_Reports& GetTraces() const;
    const T_TextReports& GetReports() const;
    void Clear();
    void ClearTraces();
    void MarkAsRead();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Reports( const Reports& );            //!< Copy constructor
    Reports& operator=( const Reports& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::Report& message );
    virtual void DoUpdate( const sword::InvalidateReport& msg );
    virtual void DoUpdate( const sword::Trace& msg );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Entity_ABC& entity_;
    kernel::Controller& controller_;
    const ReportFactory& reportFactory_;
    T_Reports traces_;
    T_TextReports reports_;
    //@}
};

#endif // __Reports_h_
