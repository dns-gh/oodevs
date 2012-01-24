// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ADN_SearchButton_h_
#define __ADN_SearchButton_h_

#include <boost/noncopyable.hpp>
// =============================================================================
/** @class  ADN_SearchButton
    @brief  ADN_SearchButton
*/
// Created: ABR 2012-01-19
// =============================================================================
class ADN_SearchButton : public QPushButton
                       , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_SearchButton( QWidget* parent = 0 );
    virtual ~ADN_SearchButton();
    //@}

    //! @name Operations
    //@{
    virtual void paintEvent( QPaintEvent *event );
    virtual QSize sizeHint() const;
    //@}

private:
    //! @name Helpers
    //@{
    static QImage GenerateSearchImage();
    //@}

private:
    //! @name Member data
    //@{
    QImage image_;
    //@}
};

#endif // __ADN_SearchButton_h_
