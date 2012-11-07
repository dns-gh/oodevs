// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_TableDialog_h_
#define __ADN_TableDialog_h_

#include <boost/noncopyable.hpp>

class ADN_Table;
class ADN_Table3;

// =============================================================================
/** @class  ADN_TableDialog
    @brief  ADN_TableDialog
*/
// Created: APE 2005-04-04
// =============================================================================
class ADN_TableDialog : public QDialog
                      , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ADN_TableDialog( QWidget* pParent, const QString& strCaption, ADN_Table& pTable );
    virtual ~ADN_TableDialog();
    //@}

private slots:
    //! @name Helpers
    //@{
    void PrintTable();
    void SaveTable();
    //@}

private:
    //! @name Member data
    //@{
    ADN_Table&     table_;
    const QString& caption_;
    //@}
};

class ADN_TableDialog2 : public QDialog
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ADN_TableDialog2( QWidget* pParent, const QString& strCaption, ADN_Table3& pTable );
    virtual ~ADN_TableDialog2();
    //@}

private slots:
    //! @name Helpers
    //@{
    void PrintTable();
    void SaveTable();
    //@}

private:
    //! @name Member data
    //@{
    ADN_Table3&     table_;
    const QString& caption_;
    //@}
};

#endif // __ADN_TableDialog_h_
