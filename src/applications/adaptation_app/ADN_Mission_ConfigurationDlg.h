// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Mission_ConfigurationDlg_h_
#define __ADN_Mission_ConfigurationDlg_h_

// =============================================================================
/** @class  ADN_Mission_ConfigurationDlg
    @brief  ADN_Mission_ConfigurationDlg
*/
// Created: AGN 2004-04-28
// =============================================================================
class ADN_Mission_ConfigurationDlg : public QDialog
{
public:
    //! @name Constructors/Destructor
    //@{
             ADN_Mission_ConfigurationDlg( QWidget* pParent );
    virtual ~ADN_Mission_ConfigurationDlg();
    //@}

    //! @name Accessors
    //@{
    QTreeWidget* GetMissionList();
    //@}

private:
    //! @name Member data
    //@{
    QTreeWidget* pListView_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: ADN_Mission_ConfigurationDlg::GetMissionList
// Created: AGN 2004-04-28
// -----------------------------------------------------------------------------
inline
QTreeWidget* ADN_Mission_ConfigurationDlg::GetMissionList()
{
    return pListView_;
}

#endif // __ADN_Mission_ConfigurationDlg_h_
