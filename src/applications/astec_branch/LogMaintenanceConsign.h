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
#include "Drawable_ABC.h"

class Agent;
class Controller;
class Displayer_ABC;
class ComponentType;

// =============================================================================
// Created: NLD 2004-03-18
// =============================================================================
class LogMaintenanceConsign : public Drawable_ABC
{
public:
    //! @name Constructor / Destructor
    //@{
             LogMaintenanceConsign( Controller& controller, const ASN1T_MsgLogMaintenanceTraitementEquipementCreation& asn,
                                    const Resolver_ABC< Agent >& resolver, const Resolver_ABC< ComponentType >& componentResolver );
    virtual ~LogMaintenanceConsign();
    //@}

    //! @name Operations
    //@{
    void Display( Displayer_ABC& displayer ) const;
    virtual void Draw( const geometry::Point2f& where, const GlTools_ABC& tools ) const;
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
    const ComponentType* equipmentType_;
    uint    nBreakdownTypeID_; // $$$$ AGE 2006-02-28: resolve
    bool    diagnosed_;
    E_LogMaintenanceTraitementEtat nState_;
};

#endif // __LogMaintenanceConsign_h_
