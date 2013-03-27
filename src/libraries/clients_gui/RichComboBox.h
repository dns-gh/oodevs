// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __RichComboBox_h_
#define __RichComboBox_h_

#include <boost/noncopyable.hpp>

namespace gui
{
// =============================================================================
/** @class  RichComboBox
    @brief  RichComboBox
*/
// Created: NPT 2013-03-14
// =============================================================================
class RichComboBox : public QComboBox
                   , private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
    RichComboBox( const QString& objectName, QWidget* parent = 0 );
    virtual ~RichComboBox();
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

} //end namespace gui

#endif // __RichComboBox_h_
