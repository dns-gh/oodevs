// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-03-07 $
// $Archive: /MVW_v10/Build/SDK/HLA/src/HLA_CreateFederationBox.h $
// $Author: Age $
// $Modtime: 7/03/05 15:43 $
// $Revision: 1 $
// $Workfile: HLA_CreateFederationBox.h $
//
// *****************************************************************************

#ifndef __HLA_CreateFederationBox_h_
#define __HLA_CreateFederationBox_h_

#include <qdialog.h>
class QPushButton;
class QLineEdit;

// =============================================================================
/** @class  HLA_CreateFederationBox
    @brief  HLA_CreateFederationBox
*/
// Created: AGE 2005-03-07
// =============================================================================
class HLA_CreateFederationBox : public QDialog
{
    Q_OBJECT
public:
    //! @name Constructors/Destructor
    //@{
             HLA_CreateFederationBox( QWidget* pParent, const std::string& strDefaultName );
    virtual ~HLA_CreateFederationBox();
    //@}

signals:
    //! @name Signals
    //@{
    void FederationSettings( const std::string& strName, const std::string& strFomFile );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnOk();
    void OnCancel();
    void OnBrowse();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    HLA_CreateFederationBox( const HLA_CreateFederationBox& );            //!< Copy constructor
    HLA_CreateFederationBox& operator=( const HLA_CreateFederationBox& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    QLineEdit*   pNameEdit_;
    QLineEdit*   pFOMEdit_;
    QPushButton* pBrowse_;

    QPushButton* pOk_;
    QPushButton* pCancel_;
    //@}
};

#endif // __HLA_CreateFederationBox_h_
