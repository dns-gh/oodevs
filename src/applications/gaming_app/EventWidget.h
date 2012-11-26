// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef EventWidget_h
#define EventWidget_h

// =============================================================================
/** @class  EventWidget
    @brief  Event widget
*/
// Created: LGY 2012-11-26
// =============================================================================
class EventWidget : public QLabel
{
public:
    //! @name Constructors/Destructor
    //@{
             EventWidget( QWidget* parent, const QPixmap& pixmap, const QString& text );
    virtual ~EventWidget();
    //@}

    //! @name Operations
    //@{
    virtual void setText( const QString& text );
    virtual void paintEvent( QPaintEvent* event );
    //@}

private:
    //! @name Member data
    //@{
    QString label_;
    //@}
};

#endif // EventWidget_h
