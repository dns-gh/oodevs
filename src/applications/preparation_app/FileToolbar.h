// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __FileToolbar_h_
#define __FileToolbar_h_

#include <qtoolbar.h>

// =============================================================================
/** @class  FileToolbar
    @brief  FileToolbar
*/
// Created: SBO 2006-12-13
// =============================================================================
class FileToolbar : public QToolBar
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
    explicit FileToolbar( QMainWindow* pParent );
    virtual ~FileToolbar();
    //@}

    //! @name Operations
    //@{
    void EnableSaveItem( bool status );
    //@}

    private:
    //! @name Member data
    //@{
    QToolButton* saveButton_;
    //@}
};

#endif // __FileToolbar_h_
