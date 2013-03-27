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

namespace gui
{
// =============================================================================
/** @class  RichComboBox
    @brief  RichComboBox
*/
// Created: NPT 2013-03-14
// =============================================================================
class RichComboBox : public QComboBox
{

public:
    //! @name Constructors/Destructor
    //@{
    RichComboBox( const QString& objectName, QWidget* parent = 0 );
    virtual ~RichComboBox();
    //@}
};

} //end namespace gui

#endif // __RichComboBox_h_
