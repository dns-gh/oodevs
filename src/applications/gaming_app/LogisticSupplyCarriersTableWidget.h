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

private slots:
    void Update();
    void UpdateRow( int row );

private:
    double ComputeMaxMass( const std::string& nature ) const;
    double ComputeMass( const std::string& nature ) const;
    double ComputeVolume( const std::string& nature ) const;

private:
    const T_CarrierTypes& types_;
    const LogisticSupplyAvailabilityTableWidget& resources_;
    const T_AvailableDotations& dotations_;
};

#endif // LogisticSupplyCarriersTableWidget_h
