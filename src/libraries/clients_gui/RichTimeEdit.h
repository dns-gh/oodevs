// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __RichTimeEdit_h_
#define __RichTimeEdit_h_

namespace gui
{
// =============================================================================
/** @class  RichTimeEdit
    @brief  RichTimeEdit
*/
// Created: NPT 2013-03-19
// =============================================================================
class RichTimeEdit : public QTimeEdit
{

public:
    //! @name Constructors/Destructor
    //@{
             RichTimeEdit( const QString& objectName, QWidget* parent = 0 );
    virtual ~RichTimeEdit();
    //@}
};

}

#endif // __RichTimeEdit_h_
