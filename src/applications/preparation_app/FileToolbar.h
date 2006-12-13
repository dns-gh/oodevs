// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
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

public:
    //! @name Constructors/Destructor
    //@{
    explicit FileToolbar( QMainWindow* pParent );
    virtual ~FileToolbar();
    //@}
};

#endif // __FileToolbar_h_
