// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MagicOrdersInterface_h_
#define __MagicOrdersInterface_h_

#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "clients_kernel/OptionsObserver_ABC.h"
#include "clients_gui/ShapeHandler_ABC.h"
#include "clients_kernel/LocationVisitor_ABC.h"

namespace kernel
{
    class Agent_ABC;
    class KnowledgeGroup_ABC;
    class Team_ABC;
    class Controllers;
}

namespace gui
{
    class LocationCreator;
    class ParametersLayer;
}

class StaticModel;
class Publisher_ABC;

// =============================================================================
/** @class  MagicOrdersInterface
    @brief  MagicOrdersInterface
*/
// Created: AGE 2006-04-28
// =============================================================================
class MagicOrdersInterface : public QObject
                           , public kernel::Observer_ABC
                           , public kernel::ContextMenuObserver_ABC< kernel::Team_ABC >
                           , public kernel::ContextMenuObserver_ABC< kernel::KnowledgeGroup_ABC >
                           , public kernel::ContextMenuObserver_ABC< kernel::Agent_ABC >
                           , public kernel::OptionsObserver_ABC
                           , public gui::ShapeHandler_ABC
                           , private kernel::LocationVisitor_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             MagicOrdersInterface( QWidget* parent, kernel::Controllers& controllers, Publisher_ABC& publisher, const StaticModel& staticModel, gui::ParametersLayer& layer );
    virtual ~MagicOrdersInterface();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const kernel::Team_ABC& agent, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::KnowledgeGroup_ABC& agent, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Agent_ABC& agent, kernel::ContextMenu& menu );
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    virtual void Handle( kernel::Location_ABC& location );
    //@}

private slots:
    //! @name Slots
    //@{
    void Magic( int );
    void DestroyComponent();
    void Move();
    void Surrender();
    void RecoverHumanTransporters();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MagicOrdersInterface( const MagicOrdersInterface& );            //!< Copy constructor
    MagicOrdersInterface& operator=( const MagicOrdersInterface& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void AddMagicMove( const kernel::Agent_ABC& agent, QPopupMenu* menu );
    void AddMagic( const QString& label, int id,           QPopupMenu* menu );
    int  AddMagic( const QString& label, const char* slot, QPopupMenu* menu );
    void ApplyOnHierarchy( const kernel::KnowledgeGroup_ABC& group, int id );
    void ApplyOnHierarchy( const kernel::Team_ABC& team, int id );
    void FillCommonOrders( QPopupMenu* magicMenu );

    virtual void VisitLines  ( const T_PointVector& ) {};
    virtual void VisitPolygon( const T_PointVector& ) {};
    virtual void VisitCircle ( const geometry::Point2f& , float ) {};
    virtual void VisitPoint  ( const geometry::Point2f& point );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    Publisher_ABC& publisher_;
    const StaticModel& static_;
    bool controller_;
    kernel::SafePointer< kernel::Agent_ABC > selectedAgent_;
    kernel::SafePointer< kernel::KnowledgeGroup_ABC > selectedGroup_;
    kernel::SafePointer< kernel::Team_ABC > selectedTeam_;
    bool magicMove_;
    gui::LocationCreator* magicMoveLocation_;
    //@}
};

#endif // __MagicOrdersInterface_h_
