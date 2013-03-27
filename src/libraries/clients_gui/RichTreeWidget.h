// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __RichTreeWidget_h_
#define __RichTreeWidget_h_

#include <boost/noncopyable.hpp>

namespace gui
{

// =============================================================================
/** @class  RichTreeWidget
    @brief  RichTreeWidget
*/
// Created: NPT 2013-03-18
// =============================================================================
class RichTreeWidget : public QTreeWidget
{

public:
    //! @name Constructors/Destructor
    //@{
             RichTreeWidget( const QString& objectName, QWidget* parent = 0 );
    virtual ~RichTreeWidget();
    //@}
};

}

#endif // __RichTreeWidget_h_
