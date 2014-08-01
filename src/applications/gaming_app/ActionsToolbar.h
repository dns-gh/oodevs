// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionsToolbar_h_
#define __ActionsToolbar_h_

#include "gaming/Simulation.h"
#include "clients_kernel/ModesObserver_ABC.h"
#include <tools/ElementObserver_ABC.h>

namespace kernel
{
    class Controllers;
}

namespace actions
{
    class ActionsFilter_ABC;
    class ActionsModel;
}

namespace gui
{
    class RichAction;
}

class GamingConfig;
class Services;

// =============================================================================
/** @class  ActionsToolbar
    @brief  ActionsToolbar
*/
// Created: SBO 2007-03-12
// =============================================================================
class ActionsToolbar : public Q3HBox
                     , public tools::Observer_ABC
                     , public tools::ElementObserver_ABC< Simulation >
                     , public tools::ElementObserver_ABC< Services >
                     , public tools::ElementObserver_ABC< Simulation::sCheckPoint >
                     , public kernel::ModesObserver_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ActionsToolbar( QWidget* parent, actions::ActionsModel& actions, const GamingConfig& config, kernel::Controllers& controllers );
    virtual ~ActionsToolbar();
    //@}

    //! @name Operations
    //@{
    void SetFilter( const actions::ActionsFilter_ABC& filter );
    //@}

private:
    //! @name Helpers
    //@{
    void PurgeConfirmed( int result );
    virtual void NotifyUpdated( const Simulation& simulation );
    virtual void NotifyUpdated( const Services& services );
    virtual void NotifyUpdated( const Simulation::sCheckPoint& checkPoint );
    virtual void NotifyModeChanged( E_Modes newMode );

    QToolButton* CreateToolButton( const QString label, const QPixmap& pixmap, const char* slot );
    void DoLoad( const tools::Path& filename );
    //@}

private slots:
    //! @name Helpers
    //@{
    void Load();
    void Save();
    void Purge();
    void Reload();
    void OnModeChanged();
    //@}

signals:
    void PlanificationModeChange();
    void activeRefreshButton( bool );

private:
    //! @name Member data
    //@{
    kernel::Controllers&              controllers_;
    actions::ActionsModel&            actions_;
    const GamingConfig&                config_;
    const actions::ActionsFilter_ABC* filter_;
    QMessageBox*                      confirmation_;
    bool                              initialized_;
    bool                              hasReplay_;
    tools::Path                       filename_;
    gui::RichAction*                  planningAction_;
    //@}
};

#endif // __ActionsToolbar_h_
