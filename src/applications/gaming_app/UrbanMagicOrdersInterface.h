// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UrbanMagicOrdersInterface_h_
#define __UrbanMagicOrdersInterface_h_

#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "clients_gui/TerrainObjectProxy.h"

namespace kernel
{
    class Time_ABC;
    class Profile_ABC;
    class Entity_ABC;
}
namespace actions
{
    class ActionsModel;
}

class StaticModel;

// =============================================================================
/** @class  UrbanMagicOrdersInterface
    @brief  UrbanMagicOrdersInterface
*/
// Created: SLG 2010-12-21
// =============================================================================
class UrbanMagicOrdersInterface : public QObject
                               , public tools::Observer_ABC
                               , public kernel::ContextMenuObserver_ABC< gui::TerrainObjectProxy >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             UrbanMagicOrdersInterface( QWidget* parent, kernel::Controllers& controllers, actions::ActionsModel& actionsModel, const StaticModel& staticModel, const kernel::Time_ABC& simulation, const kernel::Profile_ABC& profile );
    virtual ~UrbanMagicOrdersInterface();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const gui::TerrainObjectProxy& object, kernel::ContextMenu& menu );
    //@}

private slots:
    //! @name Slots
    //@{
    void ChangeStructuralState();

    //@}

private:
    //! @name Copy/Assignment
    //@{
    UrbanMagicOrdersInterface( const UrbanMagicOrdersInterface& );            //!< Copy constructor
    UrbanMagicOrdersInterface& operator=( const UrbanMagicOrdersInterface& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void AddMagic( const QString& label, int id,           QPopupMenu* menu );
    int  AddMagic( const QString& label, const char* slot, QPopupMenu* menu );
    void AddValuedMagic( QPopupMenu* parent, kernel::ContextMenu& menu, const QString& label, const char* slot );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    actions::ActionsModel& actionsModel_;
    const StaticModel& static_;
    const kernel::Time_ABC& simulation_;
    const kernel::Profile_ABC& profile_;
    kernel::SafePointer< kernel::Entity_ABC > selectedEntity_;
    //@}
};

#endif // __UrbanMagicOrdersInterface_h_
