// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticSupplyFlowDialog_ABC_h_
#define __LogisticSupplyFlowDialog_ABC_h_

#include "clients_kernel/SafePointer.h"
#include "clients_kernel/LocationVisitor_ABC.h"
#include "clients_gui/ValuedComboBox.h"
#include "clients_gui/ShapeHandler_ABC.h"
#include <tools/Resolver_ABC.h>

namespace kernel
{
    class Automat_ABC;
    class Controllers;
    class Entity_ABC;
    class EquipmentType;
    class Formation_ABC;
    class Location_ABC;
    class Profile_ABC;
    class Time_ABC;
}

namespace actions
{
    class ActionsModel;
}

namespace gui
{
    class LocationCreator;
    class ParametersLayer;
}

class Dotation;
class StaticModel;
class SupplyStates;
class LogisticSupplyAvailabilityTableWidget;
class LogisticSupplyCarriersTableWidget;

// =============================================================================
/** @class  LogisticSupplyFlowDialog_ABC
    @brief  LogisticSupplyFlowDialog_ABC
*/
// Created: MMC 2012-10-18
// =============================================================================
class LogisticSupplyFlowDialog_ABC : public QDialog
                                   , public gui::ShapeHandler_ABC
                                   , public kernel::LocationVisitor_ABC
{
    Q_OBJECT;

protected:
    //! @name Constructors/Destructor
    //@{
    LogisticSupplyFlowDialog_ABC( QWidget* parent,
                                  kernel::Controllers& controllers,
                                  actions::ActionsModel& actionsModel,
                                  const ::StaticModel& staticModel,
                                  const kernel::Time_ABC& simulation,
                                  gui::ParametersLayer& layer,
                                  const tools::Resolver_ABC< kernel::Automat_ABC >& automats,
                                  const kernel::Profile_ABC& profile );
    virtual ~LogisticSupplyFlowDialog_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void UpdateRouteDrawpoints() = 0;
    virtual void Draw( const kernel::Location_ABC& location, const geometry::Rectangle2f& viewport, const gui::GlTools_ABC& tools ) const;
    virtual void Handle( kernel::Location_ABC& location );
    virtual void ClearCarriersData();
    virtual void ClearRouteData();
    virtual void ClearRouteList();
    //@}

protected slots:
    //! @name Slots
    //@{
    virtual void Validate() = 0;
    virtual void Reject() = 0;
    virtual void closeEvent( QCloseEvent* /*pEvent*/ );
    virtual void AddWaypoint();
    virtual void DeleteWaypoint();
    virtual void MoveUpWaypoint();
    virtual void MoveDownWaypoint();

    virtual void OnWaypointSelect();
    virtual void OnTabChanged( int index );
    virtual void OnCarriersUseCheckStateChanged();
    virtual void OnWaypointRowChanged();
    //@}

protected:
    //! @name Types
    //@{
    class CustomStringListModel : public QStringListModel
    {
    public:
        CustomStringListModel( const QStringList& strings, QObject* parent = 0 ) : QStringListModel( strings, parent ) {}
        Qt::ItemFlags flags( const QModelIndex& index ) const
        {
            Qt::ItemFlags flags = Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsEnabled;
            if( !index.isValid() )
                flags |= Qt::ItemIsDropEnabled;
            return flags;
        }
    };

    typedef std::map< QString, Dotation > T_Supplies;
    typedef std::map< QString, Dotation > T_AvailableDotations;
    typedef QMap< QString, int > T_QuantitiesMap;
    typedef std::map< QString , const kernel::EquipmentType* > T_CarriersName;

    struct Waypoint
    {
        geometry::Point2f point_;
        const kernel::Entity_ABC* pRecipient_;

        Waypoint( const geometry::Point2f& point ) : pRecipient_( 0 ), point_( point ) {}
        Waypoint( const kernel::Entity_ABC* pRecipient ): pRecipient_( pRecipient ) {}

        bool IsPoint() const { return !pRecipient_; }
        bool IsRecipient() const { return !IsPoint(); }
    };
    typedef std::vector< Waypoint > T_Route;
    typedef std::map< QString , geometry::Point2f > T_PointNames;
    //@}

    //! @name Helpers
    //@{
    virtual void AddAvailable( const Dotation& dotation );
    virtual QString GetSelectedWaypoint();

    virtual void ComputeRoute( T_Route& route ) = 0;
    virtual void AddCarryingEquipment( const kernel::Entity_ABC& entity );

    virtual void VisitLines    ( const T_PointVector& /*points*/ ) {}
    virtual void VisitRectangle( const T_PointVector& /*points*/ ) {}
    virtual void VisitPolygon  ( const T_PointVector& /*points*/ ) {}
    virtual void VisitCircle   ( const geometry::Point2f& /*center*/, float /*radius*/ ) {}
    virtual void VisitPoint    ( const geometry::Point2f& point );
    virtual void VisitPath     ( const geometry::Point2f& /*first*/, const T_PointVector& /*points*/ ) {}
    virtual void VisitCurve    ( const T_PointVector& /*points*/ ) {}
    //@}

protected:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    actions::ActionsModel& actionsModel_;
    const ::StaticModel& static_;
    const kernel::Time_ABC& simulation_;
    const tools::Resolver_ABC< kernel::Automat_ABC >& automats_;
    gui::ParametersLayer& layer_;
    const kernel::Profile_ABC& profile_;

    kernel::SafePointer< kernel::Entity_ABC > selected_;

    QStringList dotationTypes_;
    T_AvailableDotations availableSupplies_;
    T_QuantitiesMap carriersTypes_;
    T_CarriersName carriersTypeNames_;

    QTabWidget* tabs_;
    LogisticSupplyAvailabilityTableWidget* resourcesTable_;
    LogisticSupplyCarriersTableWidget* carriersTable_;

    QListView* waypointList_;
    QCheckBox* carriersUseCheck_;

    QWidget* resourcesTab_;
    QPushButton* moveUpButton_;
    QPushButton* moveDownButton_;
    QPushButton* addWaypointButton_;
    QPushButton* delWaypointButton_;

    bool startWaypointLocation_;
    gui::LocationCreator* waypointLocationCreator_;
    gui::LocationCreator* routeLocationCreator_;
    geometry::Point2f selectedPoint_;
    T_PointNames points_;
    std::vector< geometry::Point2f > routeDrawpoints_;
    //@}
};

#endif // __LogisticSupplyFlowDialog_ABC_h_
