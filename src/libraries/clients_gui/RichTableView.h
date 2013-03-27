// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __RichTableView_h_
#define __RichTableView_h_

#include <boost/noncopyable.hpp>

namespace gui
{
// =============================================================================
/** @class  RichTableView
    @brief  RichTableView
*/
// Created: NPT 2013-03-14
// =============================================================================
class RichTableView : public QTableView
{

public:
    //! @name Constructors/Destructor
    //@{
             RichTableView( const QString& objectName , QWidget* parent = 0 );
    virtual ~RichTableView();
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

} //! namespace gui

#endif // __RichTableView_h_
