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
#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class Agent_ABC;
    class Entity_ABC;
    class Controllers;
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
             StockResourcesTable( const QString& objectName, QWidget* parent, const StaticModel& model, kernel::Controllers& controllers );
    virtual ~StockResourcesTable();
    //@}

public:
    //! @name Operations
    //@{
    void Initialize( const kernel::Entity_ABC& entity );
    void UpdateStocks( const std::map< const kernel::DotationType*, unsigned int >& stocks );
    void SupplyStocks( kernel::Entity_ABC& entity ) const;
    virtual void AddResource( const kernel::DotationType& resource, int value = 0 );
    virtual void UpdateLine( int row, int value );
    virtual void CustomizeMenuAction( QAction* action, const kernel::DotationType& actionDotation ) const;
    void SetAllowedNatures( const std::set< std::string >& allowedNatures );
    //@}

private slots:
    //! @name Slots
    //@{
    virtual void OnDataChanged( const QModelIndex& index, const QModelIndex& );
    //@}

private:
    //! @name Helpers
    //@{
    void UpdateInitStocks( const kernel::Entity_ABC& entity );
    void ComputeStockWeightVolumeLeft( const kernel::Agent_ABC& stockUnit, const std::string& nature, double& weightResult, double& volumeResult ) const;
    bool IsStockValid( const kernel::Agent_ABC& agent, const kernel::DotationType& dotationType ) const;
    unsigned int FillStockUntilMaxReached( kernel::Agent_ABC& agent, const kernel::DotationType& dotationType, unsigned int quantity ) const;
    //@}

private:
    //! @name Member data
    //@{
    const StaticModel& staticModel_;
    kernel::SafePointer< kernel::Entity_ABC > entity_;
    std::set< std::string > allowedNatures_;
    //@}
};

#endif // _StockResourcesTable_h
