// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __RichRadioButton_h_
#define __RichRadioButton_h_

#include <boost/noncopyable.hpp>

namespace gui
{

// =============================================================================
/** @class  RichRadioButton
    @brief  RichRadioButton
*/
// Created: NPT 2013-03-14
// =============================================================================
class RichRadioButton : public QRadioButton
{

public:
    //! @name Constructors/Destructor
    //@{
    RichRadioButton( const QString& objectName, const QString& text, QWidget* parent  = 0 );
    virtual ~RichRadioButton();
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

}

#endif // __RichRadioButton_h_
