// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ADN_ListViewToolTip_h_
#define __ADN_ListViewToolTip_h_

#include <boost/noncopyable.hpp>

class ADN_ListView;

// =============================================================================
/** @class  ADN_ListViewToolTip
    @brief  ADN_ListViewToolTip
*/
// Created: RBA 2011-08-24
// =============================================================================
class ADN_ListViewToolTip : public QObject
                          , private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             ADN_ListViewToolTip( QWidget* parent, ADN_ListView& list );
    virtual ~ADN_ListViewToolTip();
    //@}

protected:
    //! @name Helpers
    //@{
    bool eventFilter( QObject *obj, QEvent *event );
    //@}

private:
    //! @name Member data
    //@{
    ADN_ListView& listView_;
    //@}
};

#endif // __ADN_ListViewToolTip_h_
