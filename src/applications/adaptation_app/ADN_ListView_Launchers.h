//*****************************************************************************
//
// $Created: JDY 03-07-15 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_ListView_Launchers.h $
// $Author: Ape $
// $Modtime: 1/03/05 10:48 $
// $Revision: 4 $
// $Workfile: ADN_ListView_Launchers.h $
//
//*****************************************************************************

#ifndef __ADN_ListView_Launchers_h_
#define __ADN_ListView_Launchers_h_

#include "ADN_ListView.h"

// =============================================================================
/** @class  ADN_ListView_Launchers
    @brief  ADN_ListView_Launchers
*/
// Created: APE 2004-12-29
// =============================================================================
class ADN_ListView_Launchers
: public ADN_ListView
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    ADN_ListView_Launchers( QWidget* pParent = 0, const char* szName = 0, Qt::WFlags f = 0 );
    virtual ~ADN_ListView_Launchers();
    //@}

private:
    //! @name Operations
    //@{
    void        ConnectItem  ( bool           bConnect );
    void        OnContextMenu( const QPoint&  pt       );
    std::string GetToolTipFor( Q3ListViewItem& item     );
    //@}
};


#endif // __ADN_ListView_Launchers_h_
