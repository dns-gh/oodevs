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

namespace kernel
{
    class Controllers;
    class DotationType;
    class Entity_ABC;
}

class MaxStockNaturesTable;
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
             StocksAndNaturesEditor( QWidget* parent, const StaticModel& model, kernel::Controllers& controllers );
    virtual ~StocksAndNaturesEditor();
    //@}

public:
    //! @name Operations
    //@{
    void Initialize( const kernel::Entity_ABC& entity );
    void SupplyStocks( kernel::Entity_ABC& entity ) const;
    //@}

private slots:
    //! @name Slots
    //@{
    void NotifyStocksUserChange();
    void NotifyAutomaticStocks( const std::map< const kernel::DotationType*, unsigned int >& stocks );
    //@}

private:
    //! @name Member data
    //@{
    StockResourcesTable* stockResourcesTable_;
    MaxStockNaturesTable* maxStockNaturesTable_;
    //@}
};

#endif // _StocksAndNaturesEditor_h
