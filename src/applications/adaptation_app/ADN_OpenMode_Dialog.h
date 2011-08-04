// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2004-05-25 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_OpenMode_Dialog.h $
// $Author: Ape $
// $Modtime: 28/01/05 12:05 $
// $Revision: 2 $
// $Workfile: ADN_OpenMode_Dialog.h $
//
// *****************************************************************************

#ifndef __ADN_OpenMode_Dialog_h_
#define __ADN_OpenMode_Dialog_h_

enum E_OpenMode;

// =============================================================================
/** @class  ADN_OpenMode_Dialog
    @brief  ADN_OpenMode_Dialog
    @par    Using example
    @code
    ADN_OpenMode_Dialog;
    @endcode
*/
// Created: AGN 2004-05-25
// =============================================================================
class ADN_OpenMode_Dialog
: public QDialog
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_OpenMode_Dialog( QWidget* pParent );
    virtual ~ADN_OpenMode_Dialog();
    //@}

    //! @name Accessors
    //@{
    E_OpenMode GetOpenMode() const;
    QString GetPassword() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ADN_OpenMode_Dialog( const ADN_OpenMode_Dialog& );            //!< Copy constructor
    ADN_OpenMode_Dialog& operator=( const ADN_OpenMode_Dialog& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    QLineEdit* pPassword_;
    QRadioButton* pNormalMode_;
    QRadioButton* pAdminMode_;
    //@}
};


#endif // __ADN_OpenMode_Dialog_h_
