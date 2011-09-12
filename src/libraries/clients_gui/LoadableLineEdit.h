// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __LoadableLineEdit_h_
#define __LoadableLineEdit_h_

#include "LoadableField.h"

namespace gui
{

// =============================================================================
/** @class  LoadableLineEdit
    @brief  Line edit that can be filled from a file or manually.
*/
// Created: BCI 2011-05-09
// =============================================================================
class LoadableLineEdit : public LoadableFieldTemplate< QLineEdit >
{
public:

    //! @name Constructors/Destructor
    //@{
             LoadableLineEdit( QWidget* parent, const QString& probableName = QString() );
    virtual ~LoadableLineEdit();
    //@}

    //! @name Operations
    //@{
    void clear();
    QString text() const;
    void setText( const QString& );
    void setValidator( const QValidator* );
    //@}
};

} // end namespace gui

#endif // __LoadableLineEdit_h_
