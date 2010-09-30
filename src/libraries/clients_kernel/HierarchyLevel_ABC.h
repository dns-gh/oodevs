// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __HierarchyLevel_ABC_h_
#define __HierarchyLevel_ABC_h_

#pragma warning( push, 0 )
#include <qstring.h>
#pragma warning( pop )

namespace kernel
{
// =============================================================================
/** @class  HierarchyLevel_ABC
    @brief  HierarchyLevel_ABC
*/
// Created: SBO 2006-09-28
// =============================================================================
class HierarchyLevel_ABC
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
    virtual std::string GetSymbol() const = 0;

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
