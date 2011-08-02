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
             ADN_TableDialog( QWidget* pParent, const QString& strCaption, ADN_Table* pTable );
    virtual ~ADN_TableDialog();
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
