// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef kernel_PropertiesGroupDictionary_h
#define kernel_PropertiesGroupDictionary_h

#include "PropertiesDictionary.h"

namespace kernel
{
    class UrbanObject_ABC;
    class Displayer_ABC;
// =============================================================================
/** @class  PropertiesGroupDictionary
    @brief  Properties group dictionary
*/
// Created: LGY 2013-01-03
// =============================================================================
class PropertiesGroupDictionary : public kernel::PropertiesDictionary
{
public:
    //! @name Constructors/Destructor
    //@{
             PropertiesGroupDictionary( Controller& controller, Displayer_ABC& comparator );
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
    Displayer_ABC& comparator_;
    //@}
};

}

#endif // kernel_PropertiesGroupDictionary_h
