// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DIALOG_h_
#define __DIALOG_h_

#pragma warning( disable : 4127 4512 )

#include <QtGui/QtGui>

// =============================================================================
/** @class  Dialog
    @brief  Dialog
*/
// Created: JSR 2014-05-20
// =============================================================================
class Dialog : public QDialog
{
    Q_OBJECT
public:
    //! @name Constructors/Destructor
    //@{
             Dialog();
    virtual ~Dialog();
    //@}

private slots:
    //! @name 
    void OnBrowseODB();
    void OnBrowseData();
    void OnGenerate();
    //@}

private:
    //! @name Helpers
    std::string GetUnitString( unsigned int id, const std::string& mgrs, const std::string& wound, bool& pc ) const;
    //@}

private:
    //! @Member data
    //@{
    QLineEdit* odb_;
    QLineEdit* data_;
    QLineEdit* unitType_;
    QLineEdit* unitName_;
    QLineEdit* automatType_;
    QLineEdit* automatName_;
    //@}
};

#endif // __DIALOG_h_
