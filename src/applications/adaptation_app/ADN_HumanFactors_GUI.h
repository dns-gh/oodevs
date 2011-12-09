// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_HumanFactors_GUI_h_
#define __ADN_HumanFactors_GUI_h_

#include "ADN_GUI_ABC.h"
#include "ADN_HumanFactors_Data.h"

// =============================================================================
/** @class  ADN_HumanFactors_GUI
    @brief  ADN_HumanFactors_GUI
*/
// Created: APE 2004-12-06
// =============================================================================
class ADN_HumanFactors_GUI : public ADN_GUI_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_HumanFactors_GUI( ADN_HumanFactors_Data& data );
    virtual ~ADN_HumanFactors_GUI();
    //@}

    //! @name Operations
    //@{
    void Build();
    //@}

private:
    //! @name Helpers
    //@{
    QWidget* BuildModifiers( QWidget* pParent, ADN_HumanFactors_Data::ModifiersInfo& modifiers, const QString& szName );
    QWidget* BuildThresholds( QWidget* pParent, ADN_HumanFactors_Data::ThresholdsInfo& thresholds, const QString& szName, const QString& firstThresholdName, const QString& secondThresholdName );
    //@}

private:
    //! @name Member data
    //@{
    ADN_HumanFactors_Data& data_;
    //@}
};


#endif // __ADN_HumanFactors_GUI_h_
