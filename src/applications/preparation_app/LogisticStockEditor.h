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

private:
    //! @name Operations
    //@{
    virtual void Update( kernel::ContextMenu& menu );
    virtual void SupplyHierarchy( const kernel::Entity_ABC& entity, const gui::LogisticHierarchiesBase& logHierarchy );
    //@}

private:
    //! @name Helpers
    //@{
    void FindStocks( const kernel::Entity_ABC& rootEntity , const kernel::Entity_ABC& entity, std::set< const kernel::Agent_ABC* >& entStocks );
    void CleanStocks( std::set< const kernel::Agent_ABC* >& entStocks );
    void SupplyStocks( std::set< const kernel::Agent_ABC* >& entStocks, const T_Requirements& requirements );
    bool IsStockValid(  const kernel::Agent_ABC& stockUnit, const kernel::DotationType& dotation );
    double DoDotationDistribution( std::set< const kernel::Agent_ABC* >& entStocks, const kernel::DotationType& dotationType, double quantity );
    void ComputeAvailableCapacity( const kernel::Agent_ABC& entStock, const kernel::DotationType& dotationType, double& weight, double& volume );
    //@}
};

#endif // _LogisticStockEditor_h
