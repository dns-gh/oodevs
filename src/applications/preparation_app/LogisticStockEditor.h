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
#include <boost/array.hpp>
#include "ENT/ENT_Enums_Gen.h"

class Dotation;
class LogisticHierarchiesBase;
class StaticModel;
class Stocks;

namespace kernel
{
    class Entity_ABC;
    class Automat_ABC;
    class Formation_ABC;
    class Agent_ABC;
    class Controllers;
    class TacticalHierarchies;
    class DotationType;
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
    , private boost::noncopyable
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
    LogisticStockEditor( QWidget* parent, kernel::Controllers& controllers, const StaticModel& staticModel );
    virtual ~LogisticStockEditor();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const kernel::Automat_ABC& automat, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Formation_ABC& formation, kernel::ContextMenu& menu );
    //@}

private :
    //! @name Types
    //@{
    boost::array< QString, eNbrStockCategory > dotationTypeName;
    boost::array< std::string, eNbrStockCategory > dotationTypeId;
    //@}

private:

    //! @name Helpers
    //@{
    void Update( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu );

    bool IsLogisticBase( const kernel::Entity_ABC& rootEntity );
    void SupplyHierarchy( kernel::SafePointer< kernel::Entity_ABC > entity );
    void SupplyLogisticBaseStocks( const kernel::Entity_ABC& logBase, E_StockCategory logType, std::map< const kernel::DotationType*, double >& requirements );
    void FindStocks( const kernel::Entity_ABC& rootEntity , const kernel::Entity_ABC& entity, std::set< const kernel::Agent_ABC* >& entStocks );
    void ComputeRequirements( const kernel::Agent_ABC& agent, E_StockCategory logType, std::map< const kernel::DotationType*, double >& requirements );
    void SupplyStocks( std::set< const kernel::Agent_ABC* >& entStocks, const std::map< const kernel::DotationType*, double >& requirements );
    bool IsStockValid(  const kernel::Agent_ABC& stockUnit, const kernel::DotationType& dotation );
    E_StockCategory GetDotationLogisticType( const kernel::DotationType& dotationType );
    unsigned int CountAvailableStockBases( const std::set< const kernel::Agent_ABC* >& entStocks, const kernel::DotationType& requirement );
    void FillSupplyRequirements( const kernel::Entity_ABC& entity, E_StockCategory logType, std::map< const kernel::DotationType*, double >& requirements );
    //@}

    private slots:
        //! @name Slots
        //@{
        void Show();
        void Validate();
        void Accept();
        void Reject();
        void closeEvent( QCloseEvent* pEvent );
        //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    const StaticModel& staticModel_;

    QGridLayout* layout_;
    QLabel* categotyLabel_;
    QLabel* quantityLabel_;
    QListView* category_;
    QSpinBox* factor_;
    QPushButton* validateButton_;
    QPushButton* cancelButton_;
    QStringList* listDotationLogisticTypes_;
    //@}
};

#endif // _LogisticStockEditor_h
