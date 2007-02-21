// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MedicalConsignsWidget_h_
#define __MedicalConsignsWidget_h_

#include "gaming/LogMedicalConsign.h"
#include "LogisticConsignsWidget_ABC.h"

class MedicalStates;

// =============================================================================
/** @class  MedicalConsignsWidget
    @brief  MedicalConsignsWidget
*/
// Created: SBO 2007-02-19
// =============================================================================
class MedicalConsignsWidget : public LogisticConsignsWidget_ABC< MedicalConsignsWidget, LogMedicalConsign, MedicalStates >
{

public:
    //! @name Constructors/Destructor
    //@{
             MedicalConsignsWidget( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~MedicalConsignsWidget();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MedicalConsignsWidget( const MedicalConsignsWidget& );            //!< Copy constructor
    MedicalConsignsWidget& operator=( const MedicalConsignsWidget& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DisplayRequested( const LogisticConsigns& consigns, gui::ListDisplayer< MedicalConsignsWidget >* list );
    virtual void DisplayHandled( const LogisticConsigns& consigns, gui::ListDisplayer< MedicalConsignsWidget >* list );
    //@}
};

#endif // __MedicalConsignsWidget_h_
