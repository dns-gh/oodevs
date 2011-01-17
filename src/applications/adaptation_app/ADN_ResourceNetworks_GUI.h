// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ADN_ResourceNetworks_GUI_h_
#define __ADN_ResourceNetworks_GUI_h_

#include "ADN_GUI_ABC.h"
#include "ADN_ComboBox_Vector.h"
#include "ADN_ResourceNetworks_Data.h"

class ADN_ListView_ResourceNetworks;
class ADN_Equipement_Data::ResourceInfos;
class ADN_Equipement_Data::CategoryInfo;

// =============================================================================
/** @class  ADN_ResourceNetworks_GUI
    @brief  ADN_ResourceNetworks_GUI
*/
// Created: JSR 2010-09-13
// =============================================================================
class ADN_ResourceNetworks_GUI : public ADN_GUI_ABC
{
    Q_OBJECT
public:
    enum E_GuiElements
    {
        eName,
        eDotation,
        eCategory,
        eProduction,
        eColor,
        eNbrGuiElements
    };

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_ResourceNetworks_GUI( ADN_ResourceNetworks_Data& data );
    virtual ~ADN_ResourceNetworks_GUI();
    //@}

    //! @name Operations
    //@{
    void Build();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ADN_ResourceNetworks_GUI( const ADN_ResourceNetworks_GUI& );            //!< Copy constructor
    ADN_ResourceNetworks_GUI& operator=( const ADN_ResourceNetworks_GUI& ); //!< Assignment operator
    //@}

private slots:
    //! @name Slots
    //@{
    void OnDotationChanged();
    //@}

private:
    //! @name Member data
    //@{
    ADN_ResourceNetworks_Data& data_;
    ADN_ListView_ResourceNetworks* pResourceNetworks_;
    ADN_ComboBox_Vector< ADN_Equipement_Data::ResourceInfos >* dotationsCombo_;
    ADN_ComboBox_Vector< ADN_Equipement_Data::CategoryInfo >* categoriesCombo_;
    //@}
};

#endif // __ADN_ResourceNetworks_GUI_h_
