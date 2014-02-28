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

#include "LogisticEditor.h"

// =============================================================================
/** @class  LogisticStockEditor
@brief  Color editor
*/
// Created: MMC 2011-06-23
// =============================================================================
class LogisticStockEditor : public LogisticEditor
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             LogisticStockEditor( QWidget* parent, kernel::Controllers& controllers, const ::StaticModel& staticModel );
    virtual ~LogisticStockEditor();
    //@}

signals:
    //! @name Signals
    //@{
    void DotationsStocksComputed( const LogisticEditor::T_Requirements& );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void SupplyHierarchy( const kernel::Entity_ABC& entity, const gui::LogisticHierarchiesBase& logHierarchy );
    //@}

private:
    //! @name Helpers
    //@{
    void FindStocks( const kernel::Entity_ABC& rootEntity , const kernel::Entity_ABC& entity, std::set< const kernel::Agent_ABC* >& entStocks );
    double DoDotationDistribution( std::set< const kernel::Agent_ABC* >& entStocks, const kernel::DotationType& dotationType, double quantity );
    void ComputeAvailableCapacity( const kernel::Agent_ABC& entStock, const kernel::DotationType& dotationType, double& weight, double& volume );
    //@}
};

#endif // _LogisticStockEditor_h
