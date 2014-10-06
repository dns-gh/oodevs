// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __LogisticTreeView_h_
#define __LogisticTreeView_h_

#include "clients_gui/LogisticTreeView.h"

namespace actions
{
    class ActionsModel;
}

namespace kernel
{
    class Time_ABC;
}

class LogisticLinks;
class StaticModel;

// =============================================================================
/** @class  LogisticTreeView
    @brief  LogisticTreeView
*/
// Created: ABR 2012-09-21
// =============================================================================
class LogisticTreeView : public gui::LogisticTreeView
                       , public tools::ElementObserver_ABC< LogisticLinks >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             LogisticTreeView( const QString& objectName,
                               kernel::Controllers& controllers,
                               const kernel::Profile_ABC& profile,
                               gui::ModelObserver_ABC& modelObserver,
                               const gui::EntitySymbols& symbols,
                               const StaticModel& staticModel,
                               const kernel::Time_ABC& simulation,
                               actions::ActionsModel& actionsModel,
                               gui::ChangeSuperiorDialog& changeSuperiorDialog,
                               QWidget* parent /*= 0*/ );
    virtual ~LogisticTreeView();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const LogisticLinks& links );
    virtual const kernel::Entity_ABC* RetrieveSuperior( const kernel::Entity_ABC& entity ) const;
    virtual void SetSuperior( const kernel::Entity_ABC& entity, const kernel::Entity_ABC* currentsuperior, const kernel::Entity_ABC* nominalsuperior );
    bool PopContext( int context );
    QString GetErrorText( const sword::UnitMagicActionAck& ack );
    //@}

private:
    //! @name Member data
    //@{
    actions::ActionsModel&  actionsModel_;
    const StaticModel&      static_;
    const kernel::Time_ABC& simulation_;
    std::vector< int >      contexts_;
    //@}
};

#endif // __LogisticTreeView_h_
