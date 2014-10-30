// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __gui_RichPathWidget_h_
#define __gui_RichPathWidget_h_

#include "RichWidget.h"

namespace gui
{
    class RichLineEdit;

// =============================================================================
/** @class  RichPathWidget
    @brief  RichPathWidget
*/
// Created: JSR 2014-10-29
// =============================================================================
    class RichPathWidget : public RichWidget< QWidget >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit RichPathWidget( const QString& objectName, const QString& caption, const tools::Path& dir, const QString& filter, QWidget* parent = 0 );
    virtual ~RichPathWidget();
    //@}

public:
    //! @name Operations
    //@{
    void SetPath( const tools::Path& path );
    tools::Path GetPath() const;
    //@}

signals:
    //! @name signals
    //@{
    void PathChanged( const tools::Path& path );
    //@}

private slots:
    //! @name slots
    //@{
    void TextChanged( const QString& string );
    void ButtonClicked();
    //@}

private:
    //! @Member data
    //@{
    RichLineEdit* edit_;
    const QString caption_;
    const QString filter_;
    const tools::Path dir_;
    //@}
};

} //! namespace gui

#endif // __gui_RichPathWidget_h_
