// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef gui_PropertiesGroupDictionary_h
#define gui_PropertiesGroupDictionary_h

#include "PropertiesDictionary.h"

namespace kernel
{
    class UrbanObject_ABC;
    class Displayer_ABC;
}

namespace gui
{
// =============================================================================
/** @class  PropertiesGroupDictionary
    @brief  Properties group dictionary
*/
// Created: LGY 2013-01-03
// =============================================================================
class PropertiesGroupDictionary : public PropertiesDictionary
{
public:
    //! @name Constructors/Destructor
    //@{
             PropertiesGroupDictionary( kernel::Controller& controller, kernel::Displayer_ABC& comparator );
    virtual ~PropertiesGroupDictionary();
    //@}

    //! @name Operations
    //@{
    void Fill( const std::vector< const kernel::UrbanObject_ABC* >& urbanBlocks );
    //@}

private:
    //! @name Helpers
    //@{
    void Clear();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Displayer_ABC& comparator_;
    //@}
};

}

#endif // gui_PropertiesGroupDictionary_h
