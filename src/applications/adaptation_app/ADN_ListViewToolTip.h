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

class ADN_ListView;

// =============================================================================
/** @class  ADN_ListViewToolTip
    @brief  ADN_ListViewToolTip
*/
// Created: RBA 2011-08-24
// =============================================================================
class ADN_ListViewToolTip : public QObject
{

public:
    //! @name Constructors/Destructor
    //@{
             ADN_ListViewToolTip( QWidget* parent, ADN_ListView& list );
    virtual ~ADN_ListViewToolTip();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ADN_ListViewToolTip( const ADN_ListViewToolTip& );            //!< Copy constructor
    ADN_ListViewToolTip& operator=( const ADN_ListViewToolTip& ); //!< Assignment operator
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
