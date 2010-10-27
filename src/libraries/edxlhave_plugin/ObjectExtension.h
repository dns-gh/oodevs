// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __edxl_ObjectExtension_h_
#define __edxl_ObjectExtension_h_

#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Extension_ABC.h"
#include "protocol/protocol.h"

namespace dispatcher
{
    class Model_ABC;
}

namespace plugins
{
namespace edxl
{
    class ReportBuilder_ABC;
    class ReportFactory;

// =============================================================================
/** @class  ObjectExtension
    @brief  ObjectExtension
*/
// Created: JCR 2010-05-31
// =============================================================================
 class ObjectExtension
     : public kernel::Extension_ABC
     , public kernel::Updatable_ABC< MsgsSimToClient::MsgObjectCreation >
     , public kernel::Updatable_ABC< MsgsSimToClient::MsgObjectUpdate >
{

public:
    //! @name Constructors/Destructor
    //@{
             ObjectExtension( ReportBuilder_ABC& builder, const dispatcher::Model_ABC& model );
    virtual ~ObjectExtension();
    //@}

    //! @name Operations
    //@{
    virtual void DoUpdate( const MsgsSimToClient::MsgObjectCreation& message );
    virtual void DoUpdate( const MsgsSimToClient::MsgObjectUpdate& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ObjectExtension( const ObjectExtension& );            //!< Copy constructor
    ObjectExtension& operator=( const ObjectExtension& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    ReportBuilder_ABC& builder_;
    const dispatcher::Model_ABC& model_;
    //@}
};

}
}

#endif // __edxl_ObjectExtension_h_
