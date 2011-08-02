// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ADN_FragOrder_WizardLastPage_h_
#define __ADN_FragOrder_WizardLastPage_h_

#include "ADN_WizardPage_ABC.h"

// =============================================================================
/** @class  ADN_FragOrder_WizardSecondPage
    @brief  ADN_FragOrder_WizardSecondPage
*/
// Created: HBD 2010-09-01
// =============================================================================
class ADN_FragOrder_WizardSecondPage : public ADN_WizardPage_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
                 ADN_FragOrder_WizardSecondPage( Q3Wizard* pParent, const char* szName = 0, const char* szImageName = "adn_wizard.bmp" );
        virtual ~ADN_FragOrder_WizardSecondPage();
    //@}

    //! @name Operations
    //@{
        void AddFragOrderForAll( const std::string& name );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ADN_FragOrder_WizardSecondPage( const ADN_FragOrder_WizardSecondPage& );            //!< Copy constructor
    ADN_FragOrder_WizardSecondPage& operator=( const ADN_FragOrder_WizardSecondPage& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    QCheckBox* addForAllUnits_;
    QCheckBox* addForAllAutomata_;
    QCheckBox* addForAllPops_;
    //@}
};

#endif // __ADN_FragOrder_WizardLastPage_h_
