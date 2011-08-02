//*****************************************************************************
//
// $Created: JDY 03-07-03 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_ListView_Sensors.h $
// $Author: Ape $
// $Modtime: 25/04/05 10:42 $
// $Revision: 4 $
// $Workfile: ADN_ListView_Sensors.h $
//
//*****************************************************************************

#ifndef __ADN_ListView_Sensors_h_
#define __ADN_ListView_Sensors_h_

#include "ADN_ListView.h"


// =============================================================================
/** @class  ADN_ListView_Sensors
    @brief  ADN_ListView_Sensors
*/
// Created: APE 2005-01-06
// =============================================================================
class ADN_ListView_Sensors : public ADN_ListView
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_ListView_Sensors( QWidget* pParent = 0, const char* szName = 0, Qt::WFlags f = 0 );
    virtual ~ADN_ListView_Sensors();
    //@}

private:
    //! @name Operations
    //@{
    void ConnectItem( bool bConnect );
    void OnContextMenu( const QPoint& pt);

    std::string GetToolTipFor( Q3ListViewItem& item );
    //@}
};


#endif // __ADN_ListView_Sensors_h_