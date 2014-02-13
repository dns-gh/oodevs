// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef LogisticSupplyCarriersTableWidget_h
#define LogisticSupplyCarriersTableWidget_h

#include "LogisticSupplyAvailabilityTableWidget.h"
#include "clients_kernel/EquipmentType.h"

class Dotation;

namespace kernel
{
    class EquipmentType;
}

class LogisticSupplyCarriersTableWidget: public LogisticSupplyAvailabilityTableWidget
{
    Q_OBJECT

private:
    typedef std::map< QString , const kernel::EquipmentType* > T_CarrierTypes;
    typedef std::map< QString, Dotation > T_AvailableDotations;

public:
    LogisticSupplyCarriersTableWidget( QWidget* parent, const QStringList& header,
        const T_CarrierTypes& types, const LogisticSupplyAvailabilityTableWidget& resources,
        const T_AvailableDotations& dotations );

    bool IsOverloaded() const;
    bool IsUnderloaded() const;
    bool IsIncomplete() const;

private slots:
    void Update();
    void UpdateRow( int row );

private:
    double ComputeMaxMass( const std::string& nature ) const;
    const kernel::EquipmentType::CarryingSupplyFunction* GetLogSupplyFunctionCarrying( int row ) const;
    std::pair< double, double > ComputeMassVolume( const kernel::EquipmentType::CarryingSupplyFunction*, double maxMass, double maxVolume ) const;

private:
    const T_CarrierTypes& types_;
    const LogisticSupplyAvailabilityTableWidget& resources_;
    const T_AvailableDotations& dotations_;
};

#endif // LogisticSupplyCarriersTableWidget_h
