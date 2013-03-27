// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __RichListView_h_
#define __RichListView_h_

#include <boost/noncopyable.hpp>

namespace gui
{

// =============================================================================
/** @class  RichListView
    @brief  RichListView
*/
// Created: NPT 2013-03-14
// =============================================================================
class RichListView : public QListView
{

public:
    //! @name Constructors/Destructor
    //@{
    RichListView( const QString& objectName , QWidget* parent /* = 0 */ );
    virtual ~RichListView();
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

#endif // __RichListView_h_
