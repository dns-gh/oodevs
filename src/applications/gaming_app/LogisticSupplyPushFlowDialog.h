// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticSupplyPushFlowDialog_h_
#define __LogisticSupplyPushFlowDialog_h_

#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "clients_kernel/LocationVisitor_ABC.h"
#include "tools/Resolver_ABC.h"
#include "clients_gui/ValuedComboBox.h"
#include "clients_gui/ShapeHandler_ABC.h"

namespace kernel
{
    class Automat_ABC;
    class Controllers;
    class Entity_ABC;
    class EquipmentType;
    class Formation_ABC;
    class Profile_ABC;
    class Time_ABC;
    class Location_ABC;
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
/** @class  LogisticSupplyPushFlowDialog
    @brief  LogisticSupplyPushFlowDialog
*/
// $$$$ SBO 2006-07-03: look somehow similar to LogisticSupplyChangeQuotasDialog...
// Created: SBO 2006-07-03
// =============================================================================
class LogisticSupplyPushFlowDialog : public QDialog
                                   , public tools::Observer_ABC
                                   , public kernel::ContextMenuObserver_ABC< kernel::Automat_ABC >
                                   , public kernel::ContextMenuObserver_ABC< kernel::Formation_ABC >
                                   , public gui::ShapeHandler_ABC
                                   , public kernel::LocationVisitor_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             LogisticSupplyPushFlowDialog( QWidget* parent, kernel::Controllers& controllers, actions::ActionsModel& actionsModel, const StaticModel& staticModel, const kernel::Time_ABC& simulation, gui::ParametersLayer& layer, const tools::Resolver_ABC< kernel::Automat_ABC >& automats, const kernel::Profile_ABC& profile );
    virtual ~LogisticSupplyPushFlowDialog();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const kernel::Automat_ABC& agent, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Formation_ABC& agent, kernel::ContextMenu& menu );
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
    void OnRecipientContextMenu( Q3ListViewItem* item, const QPoint& point, int /*column*/ );
    void OnRecipientSelectionChanged( Q3ListViewItem* item );
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
    LogisticSupplyPushFlowDialog( const LogisticSupplyPushFlowDialog& );            //!< Copy constructor
    LogisticSupplyPushFlowDialog& operator=( const LogisticSupplyPushFlowDialog& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void AddDotation( const SupplyStates& states );
    void InsertMenuEntry( const kernel::Entity_ABC& agent, kernel::ContextMenu& menu );

    void AddRecipient( const QString& recipientName );
    void RemoveRecipient( Q3ListViewItem& item );
    void AddResourceItem();
    void AddResourceItem( const QString& dotationName, int Available, int qtySupply );
    void AddCarrierItem();
    void AddCarrierItem( const QString& dotationName, int Available, int qtySupply );

    void Clear();
    void ClearRecipientsData();
    void ClearResourcesTable();
    void ClearResourcesData();
    void ClearCarriersTable();
    void ClearCarriersData();
    void ClearRouteList();
    void ClearRouteData();
    void ComputeRecipients();
    void ComputeAvailableRecipients( QStringList& displayRecipientsNames );
    void EraseRecipientData( const QString& recipient );
    void EraseRecipientData( int index );
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
        const kernel::Automat_ABC* pRecipient_;

        Waypoint( const geometry::Point2f& point ) : pRecipient_( 0 ), point_( point ) {}
        Waypoint( const kernel::Automat_ABC* pRecipient ): pRecipient_( pRecipient ) {}

        bool isPoint() { return !pRecipient_; }
        bool isRecipient() { return !isPoint(); }
    };

    typedef std::vector< ObjectQuantity > T_SuppliesVector;
    typedef std::vector< ObjectQuantity > T_CarriersVector;
    typedef std::vector< Waypoint > T_Route;

    typedef std::map< const kernel::Automat_ABC*, T_SuppliesVector > T_RecipientSupplies;
    typedef std::vector< const kernel::Automat_ABC* > T_Recipients;
    typedef QMap< QString, const kernel::Automat_ABC* > T_RecipientsNames;

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
    const kernel::Profile_ABC& profile_;
    gui::ParametersLayer& layer_;

    kernel::SafePointer< kernel::Entity_ABC > selected_;
    QStringList dotationTypes_;
    T_Supplies supplies_;
    T_CarriersQty carriersTypes_;
    T_CarriersName carriersTypeNames_;

    QTabWidget* tabs_;
    Q3ListView* recipientsList_;
    Q3Table* resourcesTable_;
    Q3Table* carriersTable_;
    QListView* waypointList_;
    QCheckBox* carriersUseCheck_;
    QPushButton* moveUpButton_;
    QPushButton* moveDownButton_;
    QPushButton* delWaypointButton_;
    QPushButton* addWaypointButton_;

    T_Recipients recipients_;
    T_RecipientSupplies recipientSupplies_;
    T_RecipientsNames recipientsNames_;
    T_CarriersVector carriers_;

    bool startWaypointLocation_;
    gui::LocationCreator* waypointLocationCreator_;
    gui::LocationCreator* routeLocationCreator_;
    geometry::Point2f selectedPoint_;
    T_PointNames points_;
    std::vector< geometry::Point2f > routeDrawpoints_;

    const kernel::Automat_ABC* pRecipientSelected_;
    //@}
};

#endif // __LogisticSupplyPushFlowDialog_h_
