// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef _StocksAndNaturesEditor_h
#define _StocksAndNaturesEditor_h

// todo Put typedefs elsewhere to remove includes
#include "LogisticEditor.h"
#include "MaxStockNaturesTable.h"

class StockResourcesTable;
class StaticModel;

// =============================================================================
/** @class  StocksAndNaturesEditor
    @brief  StocksAndNaturesEditor
*/
// Created: JSR 2014-03-04
// =============================================================================
class StocksAndNaturesEditor : public QWidget
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             StocksAndNaturesEditor( QWidget* parent, const StaticModel& model );
    virtual ~StocksAndNaturesEditor();
    //@}

public:
    //! @name Operations
    //@{
    void ComputeValueByDotation( LogisticEditor::T_Requirements& dotations ) const;
    void ClearStocks();
    void UpdateInitStocks( const kernel::Entity_ABC& entity );
    void UpdateMaxStocks( const kernel::Entity_ABC& entity );
    void Update( const MaxStockNaturesTable::T_WeightVolumes& currentValues ); // todo change name
    //@}

private:
    //! @name Helpers
    //@{
    void ComputeStocksByNature( std::map< std::string, MaxStockNaturesTable::WeightVolume >& result ) const;
    //@}

private slots:
    //! @name Slots
    //@{
    void NotifyStocksUserChange();
    void NotifyAutomaticStocks( const LogisticEditor::T_Requirements& stocks );
    //@}

private:
    //! @name Member data
    //@{
    StockResourcesTable* stocksTableView_; // todo rename
    MaxStockNaturesTable* maxStocksTableView_; // todo rename
    //@}
};

#endif // _StocksAndNaturesEditor_h
