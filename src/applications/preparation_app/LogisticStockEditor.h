// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef _LogisticStockEditor_h
#define _LogisticStockEditor_h

#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"

class StaticModel;

namespace gui
{
    class CommonDelegate;
    class LogisticHierarchiesBase;
    class RichPushButton;
    template< typename T > class RichWidget;
}

namespace kernel
{
    class Agent_ABC;
    class Automat_ABC;
    class Controllers;
    class Entity_ABC;
    class Formation_ABC;
    class ModelLoaded;
    class ModelUnLoaded;
    class DotationType;
    class LogisticSupplyClass;
}

// =============================================================================
/** @class  LogisticStockEditor
@brief  Color editor
*/
// Created: MMC 2011-06-23
// =============================================================================
class LogisticStockEditor : public QDialog
                          , public tools::Observer_ABC
                          , public kernel::ContextMenuObserver_ABC< kernel::Automat_ABC >
                          , public kernel::ContextMenuObserver_ABC< kernel::Formation_ABC >
                          , public tools::ElementObserver_ABC< kernel::ModelLoaded >
                          , public tools::ElementObserver_ABC< kernel::ModelUnLoaded >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    LogisticStockEditor( QWidget* parent, kernel::Controllers& controllers, const ::StaticModel& staticModel );
    virtual ~LogisticStockEditor();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyUpdated( const kernel::ModelLoaded& );
    virtual void NotifyUpdated( const kernel::ModelUnLoaded& );
    virtual void NotifyContextMenu( const kernel::Automat_ABC& automat, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Formation_ABC& formation, kernel::ContextMenu& menu );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< const kernel::DotationType*, double > T_Requirements;
    typedef T_Requirements::const_iterator                CIT_Requirements;
    enum
    {
        eCategory,
        eDays
    };
    //@}

private:
    //! @name Helpers
    //@{
    void Update( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu );
    bool IsLogisticBase( const kernel::Entity_ABC& rootEntity );
    void SupplyHierarchy( kernel::SafePointer< kernel::Entity_ABC > entity );
    void SupplyLogisticBaseStocks( const kernel::Entity_ABC& logBase, const kernel::LogisticSupplyClass& logType, T_Requirements& requirements );
    void FindStocks( const kernel::Entity_ABC& rootEntity , const kernel::Entity_ABC& entity, std::set< const kernel::Agent_ABC* >& entStocks );
    void CleanStocks( std::set< const kernel::Agent_ABC* >& entStocks );
    void ComputeRequirements( const kernel::Agent_ABC& agent, const kernel::LogisticSupplyClass& logType, T_Requirements& requirements );
    void SupplyStocks( std::set< const kernel::Agent_ABC* >& entStocks, const T_Requirements& requirements );
    bool IsStockValid(  const kernel::Agent_ABC& stockUnit, const kernel::DotationType& dotation );
    void FillSupplyRequirements( const kernel::Entity_ABC& entity,const kernel::LogisticSupplyClass& logType, T_Requirements& requirements );
    void ComputeAvailableCapacity( const kernel::Agent_ABC& entStock, const kernel::DotationType& dotationType, double& weight, double& volume );
    double DoDotationDistribution( std::set< const kernel::Agent_ABC* >& entStocks, const kernel::DotationType& dotationType, double quantity );
    void GenerateLogChildrenQuotas( const gui::LogisticHierarchiesBase& logHierarchy );
    void SetQuotas( const gui::LogisticHierarchiesBase& logHierarchy, const T_Requirements& requirements );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnValueChanged( QStandardItem* item );
    void Validate();
    void Accept();
    void Reject();
    void closeEvent( QCloseEvent* pEvent );
    void ShowStocksDialog();
    void ShowQuotasDialog();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    const ::StaticModel& staticModel_;
    QStandardItemModel* dataModel_;
    gui::CommonDelegate* delegate_;
    gui::RichWidget< QTableView >* tableView_;
    gui::RichPushButton* validateButton_;
    gui::RichPushButton* cancelButton_;
    bool showStocks_;
    //@}
};

#endif // _LogisticStockEditor_h
