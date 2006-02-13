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

#include "ASN_Types.h"
#include "DIN_Types.h"
#include "Extension_ABC.h"
#include "Updatable_ABC.h"

class Report_ABC;
class Controller;
class Agent_ABC;

// =============================================================================
/** @class  Reports
    @brief  Reports
*/
// Created: AGE 2006-02-13
// =============================================================================
class Reports : public Extension_ABC
              , public Updatable_ABC< ASN1T_MsgCR >
              , public Updatable_ABC< ASN1T_MsgAttenteOrdreConduite >
              , public Updatable_ABC< TraceMessage >
{

public:
    //! @name Constructors/Destructor
    //@{
             Reports( Agent_ABC& agent, Controller& controller );
    virtual ~Reports();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Reports( const Reports& );            //!< Copy constructor
    Reports& operator=( const Reports& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< Report_ABC* >      T_Reports;
    typedef T_Reports::iterator            IT_Reports;
    typedef T_Reports::const_iterator     CIT_Reports;
    //@}

    //! @name Helpers
    //@{
    virtual void Update( const ASN1T_MsgCR& message );
    virtual void Update( const ASN1T_MsgAttenteOrdreConduite& message );
    virtual void Update( const TraceMessage& msg );
    //@}

private:
    //! @name Member data
    //@{
    Agent_ABC& agent_;
    Controller& controller_;
    T_Reports reports_;
    //@}
};

#endif // __Reports_h_
