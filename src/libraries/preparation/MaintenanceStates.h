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

#include "LogisticHierarchies.h"

// =============================================================================
/** @class  MaintenanceStates
    @brief  MaintenanceStates
*/
// Created: SBO 2006-10-24
// =============================================================================
class MaintenanceStates : public LogisticHierarchies< MaintenanceSuperior >
{

public:
    //! @name Constructors/Destructor
    //@{
             MaintenanceStates( kernel::Controller& controller, kernel::Entity_ABC& entity, const kernel::Resolver_ABC< kernel::DotationType, QString >& resolver, kernel::PropertiesDictionary& dico );
    virtual ~MaintenanceStates();
    //@}

    //! @name Operations
    //@{
    void SetSuperior( const MaintenanceSuperior& automat );
    virtual QString GetLinkType() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MaintenanceStates( const MaintenanceStates& );            //!< Copy constructor
    MaintenanceStates& operator=( const MaintenanceStates& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void CreateDictionary( kernel::PropertiesDictionary& dico );
    //@}
};

#endif // __MaintenanceStates_h_
