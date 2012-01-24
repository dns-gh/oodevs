// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ADN_ClearButton_h_
#define __ADN_ClearButton_h_

#include <boost/noncopyable.hpp>

// =============================================================================
/** @class  ADN_ClearButton
    @brief  ADN_ClearButton
*/
// Created: ABR 2012-01-19
// =============================================================================
class ADN_ClearButton : public QAbstractButton
                      , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ADN_ClearButton( QWidget *parent = 0 );
    virtual ~ADN_ClearButton();
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

#endif // __ADN_ClearButton_h_
