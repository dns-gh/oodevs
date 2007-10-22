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

#include "game_asn/Asn.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Displayable_ABC.h"

namespace kernel
{
    class Controller;
    class Entity_ABC;
    class Displayer_ABC;
}

class Report;
class ReportFactory;

// =============================================================================
/** @class  Reports
    @brief  Reports
*/
// Created: AGE 2006-02-13
// =============================================================================
class Reports : public kernel::Extension_ABC
              , public kernel::Updatable_ABC< ASN1T_MsgReport >
              , public kernel::Updatable_ABC< ASN1T_MsgTrace >
              , public kernel::Displayable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Reports( const kernel::Entity_ABC& agent, kernel::Controller& controller, const ReportFactory& reportFactory );
    virtual ~Reports();
    //@}

    //! @name Operations
    //@{
    virtual void DisplayInTooltip( kernel::Displayer_ABC& displayer ) const;

    void Clear();
    void ClearTraces();
    void MarkAsRead();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Reports( const Reports& );            //!< Copy constructor
    Reports& operator=( const Reports& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< Report* >          T_Reports;
    typedef T_Reports::iterator            IT_Reports;
    typedef T_Reports::const_iterator     CIT_Reports;
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgReport& message );
    virtual void DoUpdate( const ASN1T_MsgTrace& msg );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Entity_ABC& agent_;
    kernel::Controller& controller_;
    const ReportFactory& reportFactory_;

public:  // $$$$ AGE 2006-03-09: 
    T_Reports reports_;
    //@}
};

#endif // __Reports_h_
