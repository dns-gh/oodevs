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
    explicit UnitStateTableResource( QWidget* parent, bool forPreparation = false, const char* name = 0 );
    virtual ~UnitStateTableResource();
    //@}

protected:
    //! @name Types
    //@{
    enum E_Column { eName = 0, eCategory = 1, eQuantity = 2, eMaximum = 3, ePercentage = 4, eThreshold = 5 };
    //@}

    //! @name Helpers
    //@{
    void MergeLine( const QString& name, const QString& category, unsigned quantity, unsigned maximum, double threshold );
    void AddLine( const QString& name, const QString& category, unsigned quantity = 0, unsigned maximum = 0, double threshold = 0. );
    //@}

protected slots:
    //! @name Helpers
    //@{
    void OnValueChanged( int row, int col );
    //@}

private:
    //! @name Members Data
    //@{
    bool isPreparation_;
    //@}

};
}

#endif // __gui_UnitStateTableResource_h_
