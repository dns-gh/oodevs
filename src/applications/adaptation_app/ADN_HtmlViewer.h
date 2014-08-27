// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __ADN_HtmlViewer_h_
#define __ADN_HtmlViewer_h_

#include <boost/noncopyable.hpp>
#include "ADN_Gfx_ABC.h"

// =============================================================================
/** @class  ADN_HtmlViewer
    @brief  ADN_HtmlViewer
*/
// Created: NPT 2013-01-04
// =============================================================================
class ADN_HtmlViewer : public QWebView
                     , public ADN_Gfx_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ADN_HtmlViewer( QWidget* parent = 0 );
    virtual ~ADN_HtmlViewer();
    //@}

    //! @name Operations
    //@{
    QString text() const;
    void setText( const QString& text );
    //@}

    //! @name Helpers
    //@{
public slots:
    virtual void Warn( ADN_ErrorStatus errorStatus, const QString& errorMessage = "" );
    void OnContextMenu( const QPoint& pos );
    //@}
};

#endif // __ADN_HtmlViewer_h_