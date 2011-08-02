// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ListItemToolTip_h_
#define __ListItemToolTip_h_

// =============================================================================
/** @class  ListItemToolTip
    @brief  ListItemToolTip
*/
// Created: SBO 2007-01-08
// =============================================================================
class ListItemToolTip
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ListItemToolTip( Q3ListView& list );
    virtual ~ListItemToolTip();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ListItemToolTip(ListItemToolTip& );            //!< Copy constructor
    ListItemToolTip& operator=( const ListItemToolTip& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void maybeTip( const QPoint& pos );
    //@}

private:
    //! @name Member data
    //@{
    Q3ListView& listView_;
    //@}
};

#endif // __ListItemToolTip_h_
