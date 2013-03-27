// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __RichTextEdit_h_
#define __RichTextEdit_h_

#include <boost/noncopyable.hpp>
#include <QtGui/qtextedit.h>

namespace gui
{
// =============================================================================
/** @class  RichTextEdit
    @brief  RichTextEdit
*/
// Created: NPT 2013-03-12
// =============================================================================
class RichTextEdit : public QTextEdit
                   , private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
    RichTextEdit( const QString& objectName, QWidget* parent = 0 );
    virtual ~RichTextEdit();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

} //! namespace gui

#endif // __RichTextEdit_h_
