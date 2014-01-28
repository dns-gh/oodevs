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

namespace gui
{
    class LogisticBase;
}

// =============================================================================
/** @class  MaintenanceStatusWidget
    @brief  MaintenanceStatusWidget
*/
// Created: SBO 2007-02-19
// =============================================================================
class MaintenanceStatusWidget : public LogisticStatusWidget_ABC< kernel::MaintenanceStates_ABC >
                              , public tools::ElementObserver_ABC< gui::LogisticBase >
{
public:
    //! @name Constructors/Destructor
    //@{
             MaintenanceStatusWidget( QWidget* parent, kernel::Controllers& controllers );
    virtual ~MaintenanceStatusWidget();
    //@}

private:
    virtual void OnUpdated( const kernel::MaintenanceStates_ABC& states );
    virtual void OnUpdated( const gui::LogisticBase& base );
    virtual void NotifySelected( const kernel::Entity_ABC* element );
    virtual void NotifyUpdated( const kernel::MaintenanceStates_ABC& state );
    virtual void NotifyUpdated( const gui::LogisticBase& base );
    void Update();
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
             MedicalStatusWidget( QWidget* parent, kernel::Controllers& controllers );
    virtual ~MedicalStatusWidget();
    //@}

private:
    virtual void OnUpdated( const MedicalStates& states );
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
             SupplyStatusWidget( QWidget* parent, kernel::Controllers& controllers );
    virtual ~SupplyStatusWidget();
    //@}

private:
    virtual void OnUpdated( const SupplyStates& states );
};

#endif // __LogisticStatusWidgets_h_
