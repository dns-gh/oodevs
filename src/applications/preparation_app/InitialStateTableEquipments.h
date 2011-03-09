// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __InitialStateTableEquipments_h_
#define __InitialStateTableEquipments_h_

#include "InitialStateTable_ABC.h"

class InitialState;

// =============================================================================
/** @class  InitialStateTableEquipments
    @brief  InitialStateTableEquipments
*/
// Created: ABR 2011-02-24
// =============================================================================
class InitialStateTableEquipments : public InitialStateTable_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             InitialStateTableEquipments( QWidget* parent, const char* name = 0 );
    virtual ~InitialStateTableEquipments();
    //@}

    //! @name Operations
    //@{
    virtual void ReadExtension( const InitialState& extension );
    virtual void WriteExtension( InitialState& extension ) const;
    //@}

private:
    //! @name Types
    //@{
    enum E_Column { eName = 0, eState = 1, eBreakdown = 2 };
    //@}

private slots:
    //! @name Slots
    //@{
    virtual void OnValueChanged( int row, int col );
    //@}

private:
    //! @name Member data
    //@{
    QStringList states_;
    //@}
};

#endif // __InitialStateTableEquipments_h_
