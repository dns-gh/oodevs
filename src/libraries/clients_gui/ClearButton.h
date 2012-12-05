// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ClearButton_h_
#define __ClearButton_h_

namespace gui
{

// =============================================================================
/** @class  ClearButton
    @brief  ClearButton
*/
// Created: ABR 2012-03-27
// =============================================================================
class ClearButton : public QAbstractButton
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit ClearButton( QWidget *parent = 0 );
    virtual ~ClearButton();
    //@}

public slots:
    //! @name Slots
    //@{
    void TextChanged( const QString& text );
    //@}

protected:
    //! @name Helpers
    //@{
    void paintEvent( QPaintEvent* event );
    //@}
};

} //! namespace gui

#endif // __ClearButton_h_
