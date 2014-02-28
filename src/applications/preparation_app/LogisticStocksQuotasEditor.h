// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef _LogisticStocksQuotasEditor_h
#define _LogisticStocksQuotasEditor_h

#include "MaxStockNaturesTable.h" // todo a supprimer?
#include "LogisticEditor.h" // todo for typedef
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/Resolver2.h"
#include "clients_kernel/SafePointer.h"
#include "tools/ElementObserver_ABC.h"
#include "tools/Resolver.h"

class LogisticQuotaEditor;
class LogisticStockEditor;
class QuotasResourcesTable;
class StaticModel;
class StockResourcesTable;

namespace gui
{
    class LogisticHierarchiesBase;
    class RichPushButton;
}

namespace kernel
{
    class Agent_ABC;
    class AgentType;
    class Automat_ABC;
    class Controllers;
    class Entity_ABC;
    class EquipmentType;
    class Formation_ABC;
    class ModelUnLoaded;
    class DotationType;
    class LogisticSupplyClass;
}

// =============================================================================
/** @class  LogisticStocksQuotasEditor
@brief  LogisticStocksQuotasEditor
*/
// Created: MMC 2013-10-24
// =============================================================================
class LogisticStocksQuotasEditor : public QDialog
                                 , public tools::Observer_ABC
                                 , public kernel::ContextMenuObserver_ABC< kernel::Agent_ABC >
                                 , public kernel::ContextMenuObserver_ABC< kernel::Automat_ABC >
                                 , public kernel::ContextMenuObserver_ABC< kernel::Formation_ABC >
                                 , public tools::ElementObserver_ABC< kernel::ModelUnLoaded >
{
    Q_OBJECT
    
public:
    //! @name Constructors/Destructor
    //@{
             LogisticStocksQuotasEditor( QWidget* parent, kernel::Controllers& controllers, const StaticModel& staticModel );
    virtual ~LogisticStocksQuotasEditor();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyUpdated( const kernel::ModelUnLoaded& );
    virtual void NotifyContextMenu( const kernel::Agent_ABC& agent, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Automat_ABC& automat, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Formation_ABC& formation, kernel::ContextMenu& menu );
    //@}

private:
    //! @name Types
    //@{
    class CustomTabWidget : public QTabWidget
    {
    public:
        CustomTabWidget( QWidget* parent ) : QTabWidget( parent ) {}
        virtual ~CustomTabWidget() {}

        void HideTabBar() { if( tabBar() ) tabBar()->hide(); }
        void ShowTabBar() { if( tabBar() ) tabBar()->show(); }
    };
    //@}

private:
    //! @name Helpers
    //@{
    void Update( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu );
    bool IsLogisticBase( const kernel::Entity_ABC& rootEntity );
    void ComputeStocksByNature( std::map< std::string, MaxStockNaturesTable::WeightVolume >& result ) const;
    void CleanStocks( std::vector< const kernel::Agent_ABC* >& entStocks ) const;
    bool IsStockValid( const kernel::Agent_ABC& stockUnit, const kernel::DotationType& dotation ) const;
    void ComputeStockWeightVolumeLeft( const kernel::Agent_ABC& stockUnit, std::string nature, MaxStockNaturesTable::WeightVolume& result ) const;
    void SupplyStocks( const kernel::Entity_ABC& entityBase, const LogisticEditor::T_Requirements& requirements ) const;
    void NotifyAutomaticQuotas( const LogisticEditor::T_Requirements& stocks );
    void ApplyQuotas();
    void ApplyQuotas( const gui::LogisticHierarchiesBase& logHierarchy, const LogisticEditor::T_Requirements& generatedQuotas );
    void ComputeInitStocks( const kernel::Entity_ABC& entity, LogisticEditor::T_Requirements& requirements ) const;
    void ComputeInitQuotas( const kernel::Entity_ABC& entity, LogisticEditor::T_RequirementsMap& requirements ) const;
    //@}

private slots:
    //! @name Slots
    //@{
    void Accept();
    void Reject();
    void ShowAutomaticDialog();
    void ShowDialog();
    void NotifyStocksUserChange();
    void NotifyQuotasUserChange();
    void NotifyAutomaticStocks( const LogisticEditor::T_Requirements& stocks );
    void NotifyAutomaticQuotas( LogisticEditor::T_RequirementsMap& quotas );
    void OnQuotaNameChanged( int index );
    //@}

private:
    //! @name Member data
    //@{
    const tools::StringResolver< kernel::LogisticSupplyClass >& supplyClasses_;
    const tools::Resolver< kernel::AgentType >& agentTypes_;
    const kernel::Resolver2< kernel::EquipmentType >& equipments_;

    kernel::Controllers& controllers_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;

    LogisticStockEditor* automaticStocksEditor_;
    LogisticQuotaEditor* automaticQuotaEditor_;
    CustomTabWidget* tabs_;
    QComboBox* subordinateCombo_;
    StockResourcesTable* stocksTableView_;
    MaxStockNaturesTable* maxStocksTableView_;
    QuotasResourcesTable* quotasTableView_;
    std::map< unsigned long, LogisticEditor::T_Requirements > quotasByEntity_;
    std::map< unsigned long, const kernel::Entity_ABC* > quotasEntityFromId_;

    QWidget* pQuotas_;
    int stocksTabIndex_;
    int quotasTabIndex_;

    gui::RichPushButton* automaticEditButton_;
    //@}
};

#endif // _LogisticStocksQuotasEditor_h
