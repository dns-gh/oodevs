// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2004-04-28 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Mission_ConfigurationDlg.h $
// $Author: Ape $
// $Modtime: 8/02/05 16:49 $
// $Revision: 2 $
// $Workfile: ADN_Mission_ConfigurationDlg.h $
//
// *****************************************************************************

#ifndef __ADN_Mission_ConfigurationDlg_h_
#define __ADN_Mission_ConfigurationDlg_h_

#include <QtGui/qdialog.h>

class Q3ListView;

// =============================================================================
/** @class  ADN_Mission_ConfigurationDlg
    @brief  ADN_Mission_ConfigurationDlg
    @par    Using example
    @code
    ADN_Mission_ConfigurationDlg;
    @endcode
*/
// Created: AGN 2004-04-28
// =============================================================================
class ADN_Mission_ConfigurationDlg
: public QDialog
{
public:
    //! @name Constructors/Destructor
    //@{
             ADN_Mission_ConfigurationDlg( QWidget* pParent );
    virtual ~ADN_Mission_ConfigurationDlg();
    //@}

    //! @name Accessors
    //@{
    Q3ListView* GetMissionList();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ADN_Mission_ConfigurationDlg( const ADN_Mission_ConfigurationDlg& );            //!< Copy constructor
    ADN_Mission_ConfigurationDlg& operator=( const ADN_Mission_ConfigurationDlg& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    Q3ListView* pListView_;
    //@}
};


// -----------------------------------------------------------------------------
// Name: ADN_Mission_ConfigurationDlg::GetMissionList
// Created: AGN 2004-04-28
// -----------------------------------------------------------------------------
inline
Q3ListView* ADN_Mission_ConfigurationDlg::GetMissionList()
{
    return pListView_;
}

#endif // __ADN_Mission_ConfigurationDlg_h_
