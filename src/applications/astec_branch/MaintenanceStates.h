// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MaintenanceStates_h_
#define __MaintenanceStates_h_

#include "ASN_Types.h"
#include "Extension_ABC.h"
#include "Updatable_ABC.h"

class Controller;

// =============================================================================
/** @class  MaintenanceStates
    @brief  MaintenanceStates
*/
// Created: AGE 2006-02-14
// =============================================================================
class MaintenanceStates : public Extension_ABC
                        , public Updatable_ABC< ASN1T_MsgLogMaintenanceEtat >
{
    // $$$$ AGE 2006-02-14: entierement optionnel
public:
    //! @name Constructors/Destructor
    //@{
             MaintenanceStates( Controller& controller );
    virtual ~MaintenanceStates();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MaintenanceStates( const MaintenanceStates& );            //!< Copy constructor
    MaintenanceStates& operator=( const MaintenanceStates& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void Update( const ASN1T_MsgLogMaintenanceEtat& message );
    //@}

    //! @name Types
    //@{
    // $$$$ AGE 2006-02-14: resolve
    typedef std::vector< unsigned >                        T_Priorities;
    typedef std::vector< std::pair< unsigned, unsigned > > T_LogisticAvailabilities;
    //@}

private:
    //! @name Member data
    //@{
    Controller&              controller_;

    bool                     bChainEnabled_;
    unsigned                 nTempsBordee_;
    T_Priorities             priorities_;
    T_Priorities             tacticalPriorities_;
    T_LogisticAvailabilities dispoHaulers_;
    T_LogisticAvailabilities dispoRepairers_;
    //@}
};

#endif // __MaintenanceStates_h_
