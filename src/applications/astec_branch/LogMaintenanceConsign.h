// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2004-03-18 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/LogMaintenanceConsign.h $
// $Author: Age $
// $Modtime: 1/04/05 11:06 $
// $Revision: 1 $
// $Workfile: LogMaintenanceConsign.h $
//
// *****************************************************************************

#ifndef __LogMaintenanceConsign_h_
#define __LogMaintenanceConsign_h_

#include "Types.h"
#include "ASN_Types.h"
#include "Resolver_ABC.h"

class Agent;
class Controller;
class Displayer_ABC;

// =============================================================================
// Created: NLD 2004-03-18
// =============================================================================
class LogMaintenanceConsign
{
public:
    //! @name Constructor / Destructor
    //@{
             LogMaintenanceConsign( Controller& controller, const Resolver_ABC< Agent >& resolver, const ASN1T_MsgLogMaintenanceTraitementEquipementCreation& asn );
    virtual ~LogMaintenanceConsign();
    //@}

    //! @name Operations
    //@{
    void Display( Displayer_ABC& displayer ) const;
    //@}

    //! @name Accessors
    //@{
    const char* GetStateString() const;
    //@}

    //! @name Network events
    //@{
    void Update( const ASN1T_MsgLogMaintenanceTraitementEquipementUpdate& asn );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LogMaintenanceConsign( const LogMaintenanceConsign& );
    LogMaintenanceConsign& operator=( const LogMaintenanceConsign& );
    //@}

private:
    Controller& controller_;
    const Resolver_ABC< Agent >& resolver_;
    uint    nID_;
    Agent&  pion_;
    Agent*  pPionLogHandling_;
    uint    nEquipmentTypeID_; // $$$$ AGE 2006-02-28: resolve
    uint    nBreakdownTypeID_;
    ASN1T_EnumLogMaintenanceTraitementEtat nState_;
};

#endif // __LogMaintenanceConsign_h_
