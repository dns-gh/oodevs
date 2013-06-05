// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __RichDateTimeEdit_h_
#define __RichDateTimeEdit_h_

#include <QtGui/qdatetimeedit.h>

namespace gui
{
// =============================================================================
/** @class  RichDateTimeEdit
    @brief  RichDateTimeEdit
*/
// Created: NPT 2013-03-20
// =============================================================================
class RichDateTimeEdit : public QDateTimeEdit
{

public:
    //! @name Constructors/Destructor
    //@{
             RichDateTimeEdit( const QString& objectName, QWidget* parent = 0 );
    virtual ~RichDateTimeEdit();
    //@}
};

}

#endif // __RichDateTimeEdit_h_
