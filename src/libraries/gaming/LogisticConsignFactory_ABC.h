// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticConsignFactory_ABC_h_
#define __LogisticConsignFactory_ABC_h_

namespace sword
{
    class LogMaintenanceHandlingCreation;
    class LogSupplyHandlingCreation;
    class LogMedicalHandlingCreation;
    class LogFuneralHandlingCreation;
}

class LogMaintenanceConsign;
class LogSupplyConsign;
class LogMedicalConsign;
class LogFuneralConsign;

// =============================================================================
/** @class  LogisticConsignFactory_ABC
    @brief  LogisticConsignFactory_ABC
*/
// Created: AGE 2006-02-28
// =============================================================================
class LogisticConsignFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             LogisticConsignFactory_ABC() {};
    virtual ~LogisticConsignFactory_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual LogMaintenanceConsign* CreateMaintenanceConsign( const sword::LogMaintenanceHandlingCreation& message ) = 0;
    virtual LogSupplyConsign*      CreateSupplyConsign     ( const sword::LogSupplyHandlingCreation& message ) = 0;
    virtual LogMedicalConsign*     CreateMedicalConsign    ( const sword::LogMedicalHandlingCreation& message ) = 0;
    virtual LogFuneralConsign*     CreateFuneralConsign    ( const sword::LogFuneralHandlingCreation& message ) = 0;
    //@}
};

#endif // __LogisticConsignFactory_ABC_h_
