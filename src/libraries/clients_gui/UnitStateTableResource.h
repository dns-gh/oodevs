// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __gui_UnitStateTableResource_h_
#define __gui_UnitStateTableResource_h_

#include "UnitStateTable_ABC.h"

namespace gui
{
// =============================================================================
/** @class  UnitStateTableResource
    @brief  UnitStateTableResource
*/
// Created: ABR 2011-07-05
// =============================================================================
class UnitStateTableResource : public UnitStateTable_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             UnitStateTableResource( const QString& objectName,
                                     QWidget* parent,
                                     const QString maximalCapacityLabel,
                                     kernel::Controllers& controllers );
    virtual ~UnitStateTableResource();
    //@}

public:
    //! @name Operations
    //@{
    virtual bool IsReadOnlyForType( const std::string& typeName ) const;
    //@}

protected:
    //! @name Types
    //@{
    enum E_Column { eName = 0, eCategory = 1, eQuantity = 2, eMaximum = 3, ePercentage = 4, eLowThreshold = 5, eHighThreshold = 6, eConsumption = 7 };
    //@}

    //! @name Helpers
    //@{
    void MergeLine( const QString& name, const QString& category, unsigned quantity, unsigned maximum,
                    double lowThreshold, double highThreshold, double consumption );
    void AddLine( const QString& name, const QString& category, unsigned quantity = 0, unsigned maximum = 0,
                  double lowThreshold = 0., double highThreshold = 0., double consumption = 0. );
    void UpdateColor( QStandardItem* item, int quantity, int maximum );
    void UpdateNormalizedQuantity( int row, int quantity );
    virtual void CreateFilters( gui::RichView_ABC& richView );
    virtual bool NeedClearButton() const;
    //@}

protected slots:
    //! @name Helpers
    //@{
    void OnItemChanged( QStandardItem* item );
    //@}

private:
    //! @name Member data
    //@{
    bool blockSlots_;
    //@}
};
}

#endif // __gui_UnitStateTableResource_h_
