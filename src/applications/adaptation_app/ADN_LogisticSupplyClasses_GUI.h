// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __ADN_LogisticSupplyClasses_GUI_h_
#define __ADN_LogisticSupplyClasses_GUI_h_

#include "ADN_GUI_ABC.h"

class ADN_LogisticSupplyClasses_Data;

// =============================================================================
/** @class  ADN_LogisticSupplyClasses_GUI
    @brief  ADN_LogisticSupplyClasses_GUI
*/
// Created: APE 2004-12-06
// =============================================================================
class ADN_LogisticSupplyClasses_GUI : public ADN_GUI_ABC
{
    Q_OBJECT

public:
    //! @name Types
    //@{
    enum E_LogisticSupplyClassGuiElements
    {
        eLogisticSupplyClassName,
        eNbrLogisticSupplyClassGuiElements
    };
    //@}

    //! @name Constructors/Destructor
    //@{
    explicit ADN_LogisticSupplyClasses_GUI( ADN_LogisticSupplyClasses_Data& data );
    virtual ~ADN_LogisticSupplyClasses_GUI();
    //@}

    //! @name Operations
    //@{
    void Build();
    //@}

private:
    //! @name Member data
    //@{
    ADN_LogisticSupplyClasses_Data& data_;
    //@}
};

#endif // __ADN_LogisticSupplyClasses_GUI_h_
