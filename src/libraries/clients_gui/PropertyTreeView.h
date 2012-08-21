// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef gui_PropertyTreeView_h
#define gui_PropertyTreeView_h

namespace gui
{
// =============================================================================
/** @class  PropertyTreeView
    @brief  Property Tree view
*/
// Created: LGY 2012-08-09
// =============================================================================
class PropertyTreeView : public QTreeView
{
public:
    //! @name Constructors/Destructor
    //@{
             PropertyTreeView();
    virtual ~PropertyTreeView();
    //@}

    //! @name Operations
    //@{
    void Display();
    //@}

private:
    //! @name Helpers
    //@{
    void DisplayHeader( QModelIndex root );
    //@}
};

}

#endif // gui_PropertyTreeView_h
