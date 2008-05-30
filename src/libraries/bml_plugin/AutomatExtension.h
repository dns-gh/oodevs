// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AutomatExtension_h_
#define __AutomatExtension_h_

#include "BmlExtension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "game_asn/Simulation.h"

namespace dispatcher
{
    class Automat;
}

namespace bml
{
    class Publisher_ABC;
    class ReportFactory;

// =============================================================================
/** @class  AutomatExtension
    @brief  AutomatExtension
*/
// Created: SBO 2008-05-22
// =============================================================================
class AutomatExtension : public BmlExtension_ABC
                       , public kernel::Updatable_ABC< ASN1T_MsgAutomatOrder >
{

public:
    //! @name Constructors/Destructor
    //@{
             AutomatExtension( dispatcher::Automat& holder, Publisher_ABC& publisher, const ReportFactory& factory );
    virtual ~AutomatExtension();
    //@}

    //! @name Operations
    //@{
    virtual void DoUpdate( const ASN1T_MsgAutomatOrder& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AutomatExtension( const AutomatExtension& );            //!< Copy constructor
    AutomatExtension& operator=( const AutomatExtension& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::Automat& holder_;
    Publisher_ABC& publisher_;
    const ReportFactory& factory_;
    //@}
};

}

#endif // __AutomatExtension_h_
