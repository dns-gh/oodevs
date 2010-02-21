// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticConsignFactory_h_
#define __LogisticConsignFactory_h_

#include "LogisticConsignFactory_ABC.h"

namespace kernel
{
    class Controllers;
}

class Model;
class StaticModel;

// =============================================================================
/** @class  LogisticConsignFactory
    @brief  LogisticConsignFactory
*/
// Created: AGE 2006-02-28
// =============================================================================
class LogisticConsignFactory : public LogisticConsignFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             LogisticConsignFactory( kernel::Controllers& controllers, Model& model, const StaticModel& staticModel );
    virtual ~LogisticConsignFactory();
    //@}

    //! @name Operations
    //@{
    virtual LogMaintenanceConsign* CreateMaintenanceConsign( const MsgsSimToClient::MsgLogMaintenanceHandlingCreation& message );
    virtual LogSupplyConsign*      CreateSupplyConsign     ( const MsgsSimToClient::MsgLogSupplyHandlingCreation& message );
    virtual LogMedicalConsign*     CreateMedicalConsign    ( const MsgsSimToClient::MsgLogMedicalHandlingCreation& message );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    LogisticConsignFactory( const LogisticConsignFactory& );            //!< Copy constructor
    LogisticConsignFactory& operator=( const LogisticConsignFactory& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    Model& model_;
    const StaticModel& static_;
    //@}
};

#endif // __LogisticConsignFactory_h_
