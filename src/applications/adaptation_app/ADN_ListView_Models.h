//*****************************************************************************
//
// $Created: JDY 03-07-25 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_ListView_Models.h $
// $Author: Ape $
// $Modtime: 1/03/05 10:25 $
// $Revision: 4 $
// $Workfile: ADN_ListView_Models.h $
//
//*****************************************************************************

#ifndef __ADN_ListView_Models_h_
#define __ADN_ListView_Models_h_

#include "ADN_Types.h"
#include "ADN_ListView.h"
#include "ADN_Models_Data.h"

//*****************************************************************************
// Created: JDY 03-07-25
//*****************************************************************************
class ADN_ListView_Models
: public ADN_ListView
{
public:
    //! @name Constructors/Destructor
    //@{
    ADN_ListView_Models( ADN_Models_Data::ModelInfos::E_ModelEntityType eEntityType, QWidget* pParent = 0, const char* szName = 0, Qt::WFlags f = 0 );
    virtual ~ADN_ListView_Models();
    //@}

private:
    //! @name Operations
    //@{
    void ConnectItem( bool bConnect );
    void OnContextMenu( const QPoint& pt );
    std::string GetToolTipFor( Q3ListViewItem& item );
    //@}

private:
    ADN_Models_Data::ModelInfos::E_ModelEntityType eEntityType_;
};


#endif // __ADN_ListView_Models_h_
