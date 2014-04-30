// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __SearchLineEdit_h_
#define __SearchLineEdit_h_

#include "Filter_ABC.h"
#include "RichLineEdit.h"

namespace gui
{

// =============================================================================
/** @class  SearchLineEdit
    @brief  SearchLineEdit
*/
// Created: ABR 2012-03-27
// =============================================================================
class SearchLineEdit : public RichLineEdit
                     , public Filter_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit SearchLineEdit( const QString& objectName, QWidget* parent = 0 );
    virtual ~SearchLineEdit();
    //@}

    //! @name Filter_ABC implementation
    //@{
    virtual bool Apply( QStandardItem& item ) const;
    virtual void Clear();
    //@}
};

} //! namespace gui

#endif // __SearchLineEdit_h_
