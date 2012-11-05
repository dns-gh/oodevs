//*****************************************************************************
//
// $Created: JDY 03-09-01 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Launchers_ModifPhs_GUI.h $
// $Author: Ape $
// $Modtime: 20/04/05 16:50 $
// $Revision: 5 $
// $Workfile: ADN_Launchers_ModifPhs_GUI.h $
//
//*****************************************************************************

#ifndef __ADN_Launchers_ModifPhs_GUI_h_
#define __ADN_Launchers_ModifPhs_GUI_h_

#include "ADN_Table.h"

//*****************************************************************************
// Created: JDY 03-09-01
//*****************************************************************************
class ADN_Launchers_ModifPhs_GUI : public ADN_Table3
{
public:
    //! @name Constructors/Destructor
    //@{
             ADN_Launchers_ModifPhs_GUI( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent = 0 );
    virtual ~ADN_Launchers_ModifPhs_GUI();
    //@}

public:
    //! @name Operations
    //@{
    virtual void AddRow( int row, void* data );
    virtual void dataChanged( const QModelIndex& topLeft, const QModelIndex& bottomRight );
    //@}
};

#endif // __ADN_Launchers_ModifPhs_GUI_h_
