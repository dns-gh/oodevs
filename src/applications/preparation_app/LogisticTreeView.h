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

class LogisticBaseStates;

namespace gui
{
    class LogisticBase;
}

// =============================================================================
/** @class  LogisticTreeView
    @brief  LogisticTreeView
*/
// Created: ABR 2012-09-19
// =============================================================================
class LogisticTreeView : public gui::LogisticTreeView
                       , public tools::ElementObserver_ABC< LogisticBaseStates >
                       , public tools::ElementObserver_ABC< gui::LogisticBase >
{

public:
    //! @name Constructors/Destructor
    //@{
             LogisticTreeView( const QString& objectName,
                               kernel::Controllers& controllers,
                               const kernel::Profile_ABC& profile,
                               gui::ModelObserver_ABC& modelObserver,
                               const gui::EntitySymbols& symbols,
                               gui::ChangeSuperiorDialog& changeSuperiorDialog,
                               QWidget* parent /*= 0*/ );
    virtual ~LogisticTreeView();
    //@}

private:
    //! @name gui::LogisticTreeView implementation
    //@{
    virtual void SetSuperior( const kernel::Entity_ABC& entity, const kernel::Entity_ABC* currentsuperior, const kernel::Entity_ABC* nominalsuperior );
    virtual const kernel::Entity_ABC* RetrieveSuperior( const kernel::Entity_ABC& entity ) const;
    //@}

private:
    //! @name Observers
    //@{
    virtual void NotifyUpdated( const LogisticBaseStates& hierarchy );
    virtual void NotifyUpdated( const gui::LogisticBase& hierarchy );
    virtual void keyPressEvent( QKeyEvent* event );
    //@}
};

#endif // __LogisticTreeView_h_
