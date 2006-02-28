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

class Controller;
class Model;

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
             LogisticConsignFactory( Controller& controller, Model& model );
    virtual ~LogisticConsignFactory();
    //@}

    //! @name Operations
    //@{
    virtual LogMaintenanceConsign* CreateMaintenanceConsign( const ASN1T_MsgLogMaintenanceTraitementEquipementCreation& message );
    virtual LogSupplyConsign*      CreateSupplyConsign     ( const ASN1T_MsgLogRavitaillementTraitementCreation& message );
    virtual LogMedicalConsign*     CreateMedicalConsign    ( const ASN1T_MsgLogSanteTraitementHumainCreation& message );
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
    Controller& controller_;
    Model& model_;
    //@}
};

#endif // __LogisticConsignFactory_h_
