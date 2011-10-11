// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __gui_UnitStateTableEquipment_h_
#define __gui_UnitStateTableEquipment_h_

#include "UnitStateTable_ABC.h"

namespace gui
{
// =============================================================================
/** @class  UnitStateTableEquipment
    @brief  UnitStateTableEquipment
*/
// Created: ABR 2011-07-05
// =============================================================================
class UnitStateTableEquipment : public UnitStateTable_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit UnitStateTableEquipment( QWidget* parent );
    virtual ~UnitStateTableEquipment();
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void Purge();
    void AddLines( const QString& name, int size, E_EquipmentState state, const QStringList& breakdowns, const std::vector< unsigned int > currentBreakdowns = std::vector< unsigned int >() );
    //@}

protected:
    //! @name Types
    //@{
    enum E_Column { eName = 0, eState = 1, eBreakdown = 2 };
    //@}

protected slots:
    //! @name Slots
    //@{
    void OnItemChanged( QStandardItem* item );
    //@}
};
}

#endif // __gui_UnitStateTableEquipment_h_
