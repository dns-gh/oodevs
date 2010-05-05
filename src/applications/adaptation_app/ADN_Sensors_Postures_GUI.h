//*****************************************************************************
//
// $Created: JDY 03-09-29 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Sensors_Postures_GUI.h $
// $Author: Ape $
// $Modtime: 20/04/05 16:53 $
// $Revision: 4 $
// $Workfile: ADN_Sensors_Postures_GUI.h $
//
//*****************************************************************************

#ifndef __ADN_Sensors_Postures_GUI_h_
#define __ADN_Sensors_Postures_GUI_h_

#include "ADN_CommonGfx.h"
#include "ENT/ENT_Enums_Gen.h"

//*****************************************************************************
// Created: JDY 03-09-29
//*****************************************************************************
class ADN_Sensors_Postures_GUI : public ADN_Table2
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Sensors_Postures_GUI( const QString& strColCaption, QWidget* pParent = 0 );
    virtual ~ADN_Sensors_Postures_GUI();
    //@}

signals:
    //! @name Signals
    //@{
    void PostureChanged( std::string posture, double coeff );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnCurrentChanged();
    //@}
};


#endif // __ADN_Sensors_Postures_GUI_h_