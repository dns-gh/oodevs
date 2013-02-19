// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __StandardIconProxyStyle_h_
#define __StandardIconProxyStyle_h_

namespace gui
{

// =============================================================================
/** @class  StandardIconProxyStyle
    @brief  StandardIconProxyStyle
*/
// Created: ABR 2013-02-01
// =============================================================================
class StandardIconProxyStyle : public QCleanlooksStyle
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit StandardIconProxyStyle( int iconSize = 32 );
    virtual ~StandardIconProxyStyle();
    //@}

    //! @name Operations
    //@{
    virtual int pixelMetric( PixelMetric metric, const QStyleOption* option = 0, const QWidget * widget = 0 ) const;
    //@}

private:
    //! @name Member data
    //@{
    const int iconSize_;
    //@}
};

} //! namespace gui

#endif // __StandardIconProxyStyle_h_
