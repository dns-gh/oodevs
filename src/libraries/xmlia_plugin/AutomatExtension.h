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

#include "xmliaExtension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "game_asn/Simulation.h"

namespace dispatcher
{
    class Automat;
    class Model;
}

namespace plugins
{
namespace xmlia
{
    class Publisher_ABC;
    class ReportManager;

// =============================================================================
/** @class  AutomatExtension
    @brief  AutomatExtension
*/
// Created: MGD 2009-07-08
// =============================================================================
class AutomatExtension : public XmliaExtension_ABC
                       , public kernel::Updatable_ABC< ASN1T_MsgAutomatOrder >
{

public:
    //! @name Constructors/Destructor
    //@{
             AutomatExtension( dispatcher::Automat& holder, Publisher_ABC& publisher, ReportManager& reportManager, const dispatcher::Model& model );
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
    ReportManager& reportManager_;
    const dispatcher::Model& model_;
    //@}
};

}
}

#endif // __AutomatExtension_h_
