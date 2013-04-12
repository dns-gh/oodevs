// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef gui_PropertyItem_h
#define gui_PropertyItem_h

#include <boost/noncopyable.hpp>

namespace gui
{
    class Property_ABC;
    class PropertyDisplayer;
// =============================================================================
/** @class  PropertyItem
    @brief  Property item
*/
// Created: LGY 2012-08-13
// =============================================================================
class PropertyItem : public QStandardItem
                   , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             PropertyItem( Property_ABC& property, PropertyDisplayer& displayer );
    virtual ~PropertyItem();
    //@}

    //! @name Operations
    //@{
    void Update( QWidget* widget = 0 );
    //@}

private:
    //! @name Member data
    //@{
    PropertyDisplayer& displayer_;
    Property_ABC&      property_;
    //@}
};

}

#endif // gui_PropertyItem_h
