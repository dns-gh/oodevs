// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __UnitStateDialog_h_
#define __UnitStateDialog_h_

#include "clients_gui/UnitStateDialog.h"

namespace actions
{
    class ActionsModel;
}

namespace kernel
{
    class Profile_ABC;
    class Time_ABC;
}

namespace gui
{
    class DisplayExtractor;
}

namespace tools
{
    class ExerciseConfig;
}

class StaticModel;

// =============================================================================
/** @class  UnitStateDialog
    @brief  UnitStateDialog
*/
// Created: ABR 2011-07-07
// =============================================================================
class UnitStateDialog : public gui::UnitStateDialog
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             UnitStateDialog( QWidget* parent,
                              kernel::Controllers& controllers,
                              const tools::ExerciseConfig& config,
                              const StaticModel& staticModel,
                              actions::ActionsModel& actionsModel,
                              const kernel::Time_ABC& simulation,
                              const kernel::Profile_ABC& profile,
                              gui::DisplayExtractor& extractor );
    virtual ~UnitStateDialog();
    //@}

    //! @name Helpers
    //@{
    const QString& GetEquipmentToolTip() const;
    const QString& GetResourceToolTip() const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual void hideEvent( QHideEvent* );
    virtual void closeEvent( QCloseEvent * event );
    virtual void NotifySelected( const kernel::Entity_ABC* element );
    void Toggle( E_Tabs tab );
    void EmitSignal( bool state, E_Tabs tab );
    void EmitSignals();
    void EmitCloseCurrentSignal();
    bool CheckCurrent( E_Tabs tab ) const;
    //@}

private slots:
    //! @name Slots
    //@{
    void ToggleResource( bool );
    void ToggleEquipment( bool );
    void OnTabChanged( QWidget* );
    void OnExportClicked();
    //@}

signals:
    //! @name Signals
    //@{
    void OnToggleResource( bool );
    void OnToggleEquipment( bool );
    void Disabled( bool );
    //@}

private:
    //! @name Member data
    //@{
    const tools::ExerciseConfig& config_;
    const kernel::Profile_ABC& profile_;
    QAbstractButton*           exportButton_;
    const QString              equipmentToolTip_;
    const QString              resourceToolTip_;
    bool                       disconnected_;
    //@}
};

#endif // __UnitStateDialog_h_
