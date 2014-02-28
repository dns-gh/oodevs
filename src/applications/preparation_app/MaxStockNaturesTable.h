// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef _MaxStockNaturesTable_h
#define _MaxStockNaturesTable_h

#include "clients_gui/RichWidget.h"
#include "clients_kernel/Resolver2.h"

namespace gui
{
    class CommonDelegate;
}

namespace kernel
{
    class Agent_ABC;
    class Entity_ABC;
    class EquipmentType;
}

// =============================================================================
/** @class  MaxStockNaturesTable
    @brief  MaxStockNaturesTable
*/
// Created: MMC 2013-10-24
// =============================================================================
class MaxStockNaturesTable : public gui::RichWidget< QTableView >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             MaxStockNaturesTable( const QString& objectName, QWidget* parent, const kernel::Resolver2< kernel::EquipmentType >& equipmentTypes );
    virtual ~MaxStockNaturesTable();
    //@}

public:
    //! @name Types
    //@{
    struct WeightVolume
    {
         WeightVolume() : weight_( 0 ), volume_( 0 ) {}
        ~WeightVolume() {}

        double weight_;
        double volume_;
    };
    typedef std::map< std::string, WeightVolume > T_WeightVolumes;
    //@}

public:
    //! @name Operations
    //@{
    virtual void InitHeader();
    void Update( const kernel::Entity_ABC& logBase, const T_WeightVolumes& currentValues );
    const std::set< std::string >& GetAllowedNatures() const;
    //@}

private:
    //! @name Helpers
    //@{
    void ComputeMaxWeightVolumeByNature( const kernel::Agent_ABC& agent, T_WeightVolumes& maxValues ) const;
    void ComputeMaxStocksByNature( const kernel::Entity_ABC& rootEntity, const kernel::Entity_ABC& entity, T_WeightVolumes& maxValues ) const;
    //@}

private:
    //! @name Member data
    //@{
    QStandardItemModel* dataModel_;
    const kernel::Resolver2< kernel::EquipmentType >& equipments_;
    std::set< std::string > allowedNatures_;
    //@}
};

#endif // _MaxStockNaturesTable_h
