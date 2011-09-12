// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __HierarchyLevel_ABC_h_
#define __HierarchyLevel_ABC_h_

#include <boost/noncopyable.hpp>

#pragma warning( push, 0 )
#include <QtCore/qstring.h>
#pragma warning( pop )

class QString;

namespace kernel
{
// =============================================================================
/** @class  HierarchyLevel_ABC
    @brief  HierarchyLevel_ABC
*/
// Created: SBO 2006-09-28
// =============================================================================
class HierarchyLevel_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             HierarchyLevel_ABC() {}
    virtual ~HierarchyLevel_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual unsigned int GetId() const = 0;
    virtual QString GetName() const = 0;

    virtual const HierarchyLevel_ABC* GetPrevious() const = 0;
    virtual const HierarchyLevel_ABC* GetNext() const = 0;
    //@}

    //! @name Operations
    //@{
    virtual bool operator<( const HierarchyLevel_ABC& rhs ) const = 0;
    //@}
};

}

#endif // __HierarchyLevel_ABC_h_
