// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Model_Wizard_FirstPage_h_
#define __ADN_Model_Wizard_FirstPage_h_

#include "ADN_Wizard_FirstPage_Default.h"
#include "ADN_Models_Data.h"
#include "ADN_Missions_Data.h"

// =============================================================================
/** @class  ADN_Model_Wizard_FirstPage
    @brief  ADN_Model_Wizard_FirstPage
*/
// Created: SBO 2006-12-04
// =============================================================================
class ADN_Model_Wizard_FirstPage : public ADN_Wizard_FirstPage_Default< ADN_Models_Data::ModelInfos >
{
public:
    //! @name Constructors/Destructor
    //@{
             ADN_Model_Wizard_FirstPage( ADN_Missions_Data::T_Mission_Vector& missions, T_ItemVector& existingItemsList, Q3Wizard* pParent, const char* szName = 0, const char* szImageName = "adn_wizard.bmp" );
    virtual ~ADN_Model_Wizard_FirstPage();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ADN_Model_Wizard_FirstPage( const ADN_Model_Wizard_FirstPage& );            //!< Copy constructor
    ADN_Model_Wizard_FirstPage& operator=( const ADN_Model_Wizard_FirstPage& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual ADN_Models_Data::ModelInfos* NewT();
    //@}

private:
    //! @name Member data
    //@{
    ADN_Missions_Data::T_Mission_Vector& missions_;
    //@}
};

#endif // __ADN_Model_Wizard_FirstPage_h_
