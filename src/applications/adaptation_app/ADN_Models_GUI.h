//*****************************************************************************
//
// $Created: JDY 03-07-24 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Models_GUI.h $
// $Author: Ape $
// $Modtime: 4/03/05 11:26 $
// $Revision: 5 $
// $Workfile: ADN_Models_GUI.h $
//
//*****************************************************************************

#ifndef __ADN_Models_GUI_h_
#define __ADN_Models_GUI_h_

#include "ADN_GUI_ABC.h"
#include "ADN_Models_Data.h"

// =============================================================================
/** @class  ADN_Models_GUI
    @brief  ADN_Models_GUI
*/
// Created: APE 2004-12-06
// =============================================================================
class ADN_Models_GUI
: public ADN_GUI_ABC
{
public:
    enum E_GuiElements
    {
        eName,
        eDiaType,
        eFile,
        eMissions,
        eFragOrders,
        eMasalife,
        eNbrGuiElements
    };

    enum E_MissionGuiElements
    {
        eOrders,
        eNbrMissionGuiElements
    };

public:
    //! @name Constructors/Destructor
    //@{
     ADN_Models_GUI( ADN_Models_Data& data );
    virtual ~ADN_Models_GUI();
    //@}

    //! @name Operations
    //@{
    void Build();
    void Enable( bool enable );
    //@}

private:
    //! @name Helpers
    //@{
    QWidget* BuildPage( Q3VGroupBox*& pGroup, QWidget* pParent, ADN_Models_Data::ModelInfos::E_ModelEntityType eEntityType );
    //@}

private:
    //! @name Member data
    //@{
    ADN_Models_Data& data_;
    Q3VGroupBox* pGroupPawn_;
    Q3VGroupBox* pGroupAutomat_;
    Q3VGroupBox* pGroupPopulation_;
    //@}
};


#endif // __ADN_Models_GUI_h_
