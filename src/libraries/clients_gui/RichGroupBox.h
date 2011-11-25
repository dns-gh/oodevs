// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __RichGroupBox_h_
#define __RichGroupBox_h_

#include <boost/noncopyable.hpp>

namespace gui
{

// =============================================================================
/** @class  RichGroupBox
    @brief  RichGroupBox
*/
// Created: ABR 2011-11-21
// =============================================================================
class RichGroupBox : public QGroupBox
                   , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit RichGroupBox( QWidget* parent );
             RichGroupBox( const QString& title, QWidget* parent );
    virtual ~RichGroupBox();
    //@}

    //! @name Operations
    //@{
    void Warn();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnBlink();
    void OnDone();
    //@}

private:
    //! @name Helpers
    //@{
    void Light( const QColor& textColor, const QColor& lightColor );
    //@}

private:
    //! @name Member data
    //@{
    QTimer* timer_;
    QColor  originalTextColor_;
    QColor  originalLightColor_;
    bool    blink_;
    //@}
};

}

#endif // __RichGroupBox_h_
