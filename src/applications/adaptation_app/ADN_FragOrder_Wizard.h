// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_FragOrder_Wizard_h_
#define __ADN_FragOrder_Wizard_h_

#include "ADN_Missions_Data.h"
#include "ADN_Wizard_ABC.h"

class ADN_FragOrder_WizardSecondPage;

typedef ADN_Wizard_ABC< ADN_Missions_Data::FragOrder > ADN_Wizard_ABC_ADN_Missions_Data_FragOrder_;

// =============================================================================
/** @class  ADN_FragOrder_Wizard
    @brief  ADN_FragOrder_Wizard
*/
// Created: SBO 2006-12-04
// =============================================================================
class ADN_FragOrder_Wizard : public ADN_Wizard_ABC_ADN_Missions_Data_FragOrder_
{
public:
    //! @name Constructors/Destructor
    //@{
             ADN_FragOrder_Wizard( ADN_Missions_Data::T_FragOrder_Vector& fragOrders, QWidget* pParent = 0, const char* szName = 0 );
    virtual ~ADN_FragOrder_Wizard();
    //@}

private:
    //! @name Operations
    //@{
    virtual bool ValidateAll();
    virtual void Polish();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ADN_FragOrder_Wizard( const ADN_FragOrder_Wizard& );            //!< Copy constructor
    ADN_FragOrder_Wizard& operator=( const ADN_FragOrder_Wizard& ); //!< Assignment operator
   //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< ADN_FragOrder_WizardSecondPage > secondPage_;
    std::string name_;
    //@}
};

#endif // __ADN_FragOrder_Wizard_h_
