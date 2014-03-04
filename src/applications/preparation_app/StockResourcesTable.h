// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef _StockResourcesTable_h
#define _StockResourcesTable_h

#include "ResourcesEditorTable_ABC.h"

#include "MaxStockNaturesTable.h" // pour typedef, à virer
#include "LogisticEditor.h" // à virer

namespace kernel
{
    class Entity_ABC;
}

class StaticModel;

// =============================================================================
/** @class  StockResourcesTable
    @brief  StockResourcesTable
*/
// Created: MMC 2013-10-24
// =============================================================================
class StockResourcesTable : public ResourcesEditorTable_ABC
{
    Q_OBJECT
public:
    //! @name Constructors/Destructor
    //@{
             StockResourcesTable( const QString& objectName, QWidget* parent, const StaticModel& model );
    virtual ~StockResourcesTable();
    //@}

public:
    //! @name Operations
    //@{
    void UpdateInitStocks( const kernel::Entity_ABC& entity );
    void UpdateStocks( const LogisticEditor::T_Requirements& stocks );
    void SupplyStocks( kernel::Entity_ABC& entity ) const;
    virtual void AddResource( const kernel::DotationType& resource, int value = 0 );
    virtual void UpdateLine( int row, int value );
    virtual void CustomizeMenuAction( QAction* action, const kernel::DotationType& actionDotation ) const;
    void SetAllowedNatures( const std::set< std::string >& allowedNatures );
    //@}


private:
    void ComputeStockWeightVolumeLeft( const kernel::Agent_ABC& stockUnit, std::string nature, MaxStockNaturesTable::WeightVolume& result ) const;
    void CleanStocks( std::vector< const kernel::Agent_ABC* >& entStocks ) const;
    bool IsStockValid( const kernel::Agent_ABC& stockUnit, const kernel::DotationType& dotation ) const;

private:
    //! @name Member data
    //@{
    const StaticModel& staticModel_;
    std::set< std::string > allowedNatures_;
    //@}
};

#endif // _StockResourcesTable_h
