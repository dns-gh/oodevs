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

#include "Types.h"
#include "LogisticHierarchies.h"

// =============================================================================
/** @class  MaintenanceStates
    @brief  MaintenanceStates
*/
// Created: SBO 2006-10-24
// =============================================================================
class MaintenanceStates : public LogisticHierarchies< MaintenanceSuperior, kernel::MaintenanceHierarchies >
{

public:
    //! @name Constructors/Destructor
    //@{
             MaintenanceStates( kernel::Controller& controller, kernel::Entity_ABC& entity, kernel::PropertiesDictionary& dico );
    virtual ~MaintenanceStates();
    //@}

    //! @name Operations
    //@{
    void SetSuperior( const MaintenanceSuperior& automat );
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MaintenanceStates( const MaintenanceStates& );            //!< Copy constructor
    MaintenanceStates& operator=( const MaintenanceStates& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void CreateDictionary( kernel::PropertiesDictionary& dico, kernel::Entity_ABC& owner );
    //@}
};

#endif // __MaintenanceStates_h_
