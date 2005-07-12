// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-04-04 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_TableDialog.h $
// $Author: Ape $
// $Modtime: 20/04/05 15:56 $
// $Revision: 2 $
// $Workfile: ADN_TableDialog.h $
//
// *****************************************************************************

#ifndef __ADN_TableDialog_h_
#define __ADN_TableDialog_h_

#include <qdialog.h>

class ADN_Table;


// =============================================================================
/** @class  ADN_TableDialog
    @brief  ADN_TableDialog
*/
// Created: APE 2005-04-04
// =============================================================================
class ADN_TableDialog
: public QDialog
{
    Q_OBJECT
    MT_COPYNOTALLOWED( ADN_TableDialog )

public:
    //! @name Constructors/Destructor
    //@{
     ADN_TableDialog( QWidget* pParent, const QString& strCaption, ADN_Table* pTable );
    ~ADN_TableDialog();
    //@}

private slots:
    //! @name Helpers
    //@{
    void OnContextMenu();
    void PrintTable();
    //@}

private:
    //! @name Member data
    //@{
    ADN_Table* pTable_;
    //@}
};

#endif // __ADN_TableDialog_h_
