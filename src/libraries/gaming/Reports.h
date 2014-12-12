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
#include "clients_kernel/Displayable_ABC.h"
#include <deque>

namespace kernel
{
    class Entity_ABC;
    class Displayer_ABC;
}

class ReportFactory;

// =============================================================================
/** @class  Reports
    @brief  Reports
    // $$$$ AGE 2007-10-22: utiliser un resolver. Uniformiser avec les traces
*/
// Created: AGE 2006-02-13
// =============================================================================
class Reports : public kernel::Extension_ABC
              , public kernel::Displayable_ABC
              , public kernel::Updatable_ABC< sword::Report >
{
public:
    //! @name Constructors/Destructor
    //@{
             Reports( const kernel::Entity_ABC& entity, const ReportFactory& reportFactory );
    virtual ~Reports();
    //@}

    //! @name Operations
    //@{
    virtual void DisplayInTooltip( kernel::Displayer_ABC& displayer ) const;
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
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Entity_ABC& entity_;
    const ReportFactory& reportFactory_;
    std::deque< sword::Report > reports_;
    //@}
};
#endif // __Reports_h_
