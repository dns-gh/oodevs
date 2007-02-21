// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticStatusWidgets_h_
#define __LogisticStatusWidgets_h_

#include "LogisticStatusWidget_ABC.h"
#include "gaming/MaintenanceStates.h"
#include "gaming/MedicalStates.h"
#include "gaming/SupplyStates.h"

// =============================================================================
/** @class  MaintenanceStatusWidget
    @brief  MaintenanceStatusWidget
*/
// Created: SBO 2007-02-19
// =============================================================================
class MaintenanceStatusWidget : public LogisticStatusWidget_ABC< MaintenanceStates >
{

public:
    //! @name Constructors/Destructor
    //@{
             MaintenanceStatusWidget( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~MaintenanceStatusWidget();
    //@}
};

// =============================================================================
/** @class  MedicalStatusWidget
    @brief  MedicalStatusWidget
*/
// Created: SBO 2007-02-19
// =============================================================================
class MedicalStatusWidget : public LogisticStatusWidget_ABC< MedicalStates >
{

public:
    //! @name Constructors/Destructor
    //@{
             MedicalStatusWidget( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~MedicalStatusWidget();
    //@}
};

// =============================================================================
/** @class  SupplyStatusWidget
    @brief  SupplyStatusWidget
*/
// Created: SBO 2007-02-19
// =============================================================================
class SupplyStatusWidget : public LogisticStatusWidget_ABC< SupplyStates >
{

public:
    //! @name Constructors/Destructor
    //@{
             SupplyStatusWidget( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~SupplyStatusWidget();
    //@}
};

#endif // __LogisticStatusWidgets_h_
