//*****************************************************************************
//
// $Created: JDY 03-07-03 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_ListView_Objects.h $
// $Author: Ape $
// $Modtime: 1/03/05 11:45 $
// $Revision: 4 $
// $Workfile: ADN_ListView_Objects.h $
//
//*****************************************************************************

#ifndef __ADN_ListView_Objects_h_
#define __ADN_ListView_Objects_h_

#include "ADN_Types.h"
#include "ADN_ListView.h"

class ADN_ComboBox;

//*****************************************************************************
// Created: JDY 03-07-03
//*****************************************************************************
class ADN_ListView_Objects
    : public ADN_ListView
{
public:
    explicit ADN_ListView_Objects( QWidget* pParent = 0, const char* szName = 0, WFlags f = 0, ADN_ComboBox* pComboListUnitType = 0, ADN_ComboBox* pComboListPropagation = 0 );
    virtual ~ADN_ListView_Objects();

    void OnContextMenu( const QPoint& pt );

private:
    //! @name Helpers
    //@{
    void ConnectItem( bool bConnect );
    void UpdateComboList( ADN_ComboBox* pCombo, const std::string& value );
    //@}

    ADN_ComboBox* pComboListUnitType_;
    ADN_ComboBox* pComboListPropagation_;
};


#endif // __ADN_ListView_Objects_h_