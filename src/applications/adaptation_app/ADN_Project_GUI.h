//*****************************************************************************
//
// $Created: JDY 03-06-23 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Project_GUI.h $
// $Author: Ape $
// $Modtime: 25/04/05 12:00 $
// $Revision: 5 $
// $Workfile: ADN_Project_GUI.h $
//
//*****************************************************************************

#ifndef __ADN_Project_GUI_h_
#define __ADN_Project_GUI_h_

#include "ADN_GUI_ABC.h"

class ADN_Project_Data;


// =============================================================================
/** @class  ADN_Project_GUI
    @brief  ADN_Project_GUI
*/
// Created: APE 2004-12-06
// =============================================================================
class ADN_Project_GUI 
: public ADN_GUI_ABC
{
    MT_COPYNOTALLOWED( ADN_Project_GUI )

public:
    //! @name Constructors/Destructor
    //@{
    ADN_Project_GUI( ADN_Project_Data& data );
    ~ADN_Project_GUI();
    //@}

    //! @name Operations
    //@{
    void Build();

    void AddHLAWidget( QWidget& hlaWidget );  // Ugly...
    //@}

private:
    //! @name Member data
    //@{
    ADN_Project_Data& data_;
    QGridLayout* pLayout_;
    //@}
};


#endif // __ADN_Project_GUI_h_