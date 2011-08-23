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
class Simulation;

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
             LogisticConsignFactory( kernel::Controllers& controllers, Model& model, const StaticModel& staticModel, const Simulation& simulation );
    virtual ~LogisticConsignFactory();
    //@}

    //! @name Operations
    //@{
    virtual LogMaintenanceConsign* CreateMaintenanceConsign( const sword::LogMaintenanceHandlingCreation& message );
    virtual LogSupplyConsign*      CreateSupplyConsign     ( const sword::LogSupplyHandlingCreation& message );
    virtual LogMedicalConsign*     CreateMedicalConsign    ( const sword::LogMedicalHandlingCreation& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LogisticConsignFactory( const LogisticConsignFactory& );            //!< Copy constructor
    LogisticConsignFactory& operator=( const LogisticConsignFactory& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    Model& model_;
    const StaticModel& static_;
    const Simulation& simulation_;
    //@}
};

#endif // __LogisticConsignFactory_h_
