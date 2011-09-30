// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticSupplyPullFlowDialog_h_
#define __LogisticSupplyPullFlowDialog_h_

#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "clients_kernel/LocationVisitor_ABC.h"
#include "tools/Resolver_ABC.h"
#include "clients_gui/ValuedComboBox.h"
#include "clients_gui/ShapeHandler_ABC.h"

namespace sword {
    class PullFlowParameters;
    class PointList;
}

namespace kernel
{
    class Automat_ABC;
    class Controllers;
    class Entity_ABC;
    class EquipmentType;
    class Formation_ABC;
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

// =============================================================================
/** @class  LogisticSupplyPullFlowDialog
    @brief  LogisticSupplyPullFlowDialog
*/
// Created : AHC 2010-10-14
// =============================================================================
class LogisticSupplyPullFlowDialog : public QDialog
                                   , public tools::Observer_ABC
                                   , public kernel::ContextMenuObserver_ABC< kernel::Automat_ABC >
                                   , public gui::ShapeHandler_ABC
                                   , public kernel::LocationVisitor_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             LogisticSupplyPullFlowDialog( QWidget* parent, kernel::Controllers& controllers, actions::ActionsModel& actionsModel,
                     const StaticModel& staticModel, const kernel::Time_ABC& simulation, gui::ParametersLayer& layer, const tools::Resolver_ABC< kernel::Automat_ABC >& automats,
                     const tools::Resolver_ABC< kernel::Formation_ABC >& formations, const kernel::Profile_ABC& profile );
    virtual ~LogisticSupplyPullFlowDialog();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const kernel::Automat_ABC& agent, kernel::ContextMenu& menu );
    virtual void Handle( kernel::Location_ABC& location );
    virtual void Draw( const kernel::Location_ABC& location, const geometry::Rectangle2f& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private slots:
    //! @name Slots
    //@{
    void Show();
    void Validate();
    void Reject();
    void closeEvent( QCloseEvent* /*pEvent*/ );

    void AddWaypoint();
    void DeleteWaypoint();
    void MoveUpWaypoint();
    void MoveDownWaypoint();
    void OnSupplierValueChanged();
    void OnSupplierSelectionChanged();
    void OnResourcesValueChanged( int row, int col );
    void OnCarriersUseCheckStateChanged();
    void OnCarriersValueChanged( int row, int col );
    void OnWaypointSelect();
    void OnWaypointRowChanged();
    void OnTabChanged( int index );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LogisticSupplyPullFlowDialog( const LogisticSupplyPullFlowDialog& );            //!< Copy constructor
    LogisticSupplyPullFlowDialog& operator=( const LogisticSupplyPullFlowDialog& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void AddDotation( const SupplyStates& states );
    void AddResourceItem();
    void AddResourceItem( QString dotationName, int Available, int qtySupply );
    void AddCarrierItem();
    void AddCarrierItem( QString dotationName, int Available, int qtySupply );

    void ClearSuppliersTable();
    void ClearSuppliersData();
    void ClearResourcesTable();
    void ClearResourcesData();
    void ClearCarriersTable();
    void ClearCarriersData();
    void ClearRouteList();
    void ClearRouteData();
    void ComputeAvailableSuppliers();
    void EraseSupplierData();
    void ComputeAvailableCarriers( QStringList& carriersNames );
    void AddCarryingEquipment( const kernel::Entity_ABC& entity );

    virtual void VisitLines    ( const T_PointVector& /*points*/ ) {}
    virtual void VisitRectangle( const T_PointVector& /*points*/ ) {}
    virtual void VisitPolygon  ( const T_PointVector& /*points*/ ) {}
    virtual void VisitCircle   ( const geometry::Point2f& /*center*/, float /*radius*/ ) {}
    virtual void VisitPoint    ( const geometry::Point2f& point );
    virtual void VisitPath     ( const geometry::Point2f& /*first*/, const T_PointVector& /*points*/ ) {}
    //@}

    //! @name Types
    //@{
    typedef std::map< QString, Dotation > T_Supplies;

    struct ObjectQuantity 
    {
        QString objectName_;
        int quantity_;

        ObjectQuantity(): quantity_(0) {}
        ObjectQuantity( const QString& objectName, int quantity ): objectName_( objectName ), quantity_( quantity ) {}
    };

    struct Waypoint
    {
        geometry::Point2f point_;
        const kernel::Entity_ABC* pSupplier_;

        Waypoint( const geometry::Point2f& point ) : pSupplier_( 0 ), point_( point ) {}
        Waypoint( const kernel::Entity_ABC* pSupplier ): pSupplier_( pSupplier ) {}

        bool isPoint() { return !pSupplier_; }
        bool isSupplier() { return !isPoint(); }
    };

    typedef std::vector< ObjectQuantity > T_SuppliesVector;
    typedef std::vector< ObjectQuantity > T_CarriersVector;
    typedef std::vector< Waypoint > T_Route;
    typedef QMap< QString, const kernel::Entity_ABC* > T_SuppliersNames;

    typedef std::map< QString , unsigned int > T_CarriersQty;
    typedef std::map< QString , const kernel::EquipmentType* > T_CarriersName;   
    typedef std::map< QString , geometry::Point2f > T_PointNames;
    //@}

    //! @name Helpers
    //@{
    void ComputeRoute( T_Route& route );
    QString GetSelectedWaypoint();
    void UpdateRouteDrawpoints();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    actions::ActionsModel& actionsModel_;
    const StaticModel& static_;
    const kernel::Time_ABC& simulation_;
    const tools::Resolver_ABC< kernel::Automat_ABC >& automats_;
    const tools::Resolver_ABC< kernel::Formation_ABC >& formations_;
    const kernel::Profile_ABC& profile_;
    gui::ParametersLayer& layer_;

    kernel::SafePointer< kernel::Automat_ABC > selected_;
    QStringList dotationTypes_;
    T_Supplies supplies_;

    T_CarriersQty carriersTypes_;
    T_CarriersName carriersTypeNames_;

    QTabWidget* tabs_;
    gui::ValuedComboBox< const kernel::Entity_ABC* >* supplierCombo_;
    Q3Table* resourcesTable_;
    Q3Table* carriersTable_;
    QListView* waypointList_;
    QCheckBox* carriersUseCheck_;
    QPushButton* moveUpButton_;
    QPushButton* moveDownButton_;
    QPushButton* addWaypointButton_;
    QPushButton* delWaypointButton_;

    T_SuppliersNames suppliersNames_;
    const kernel::Entity_ABC* supplier_;
    T_SuppliesVector supplierSupplies_;
    T_CarriersVector carriers_;

    bool startWaypointLocation_;
    gui::LocationCreator* waypointLocationCreator_;
    gui::LocationCreator* routeLocationCreator_;
    geometry::Point2f selectedPoint_;
    T_PointNames points_;
    std::vector< geometry::Point2f > routeDrawpoints_;
    //@}
};

#endif // __LogisticSupplyPullFlowDialog_h_
