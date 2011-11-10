// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FormationLevels_h_
#define __FormationLevels_h_

#include "Resolver2.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Level;
    class HierarchyLevel_ABC;

// =============================================================================
/** @class  FormationLevels
    @brief  FormationLevels
*/
// Created: SBO 2006-09-21
// =============================================================================
class FormationLevels : public Resolver2< HierarchyLevel_ABC, unsigned long >
                      , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             FormationLevels();
    virtual ~FormationLevels();
    //@}

    //! @name Accessors
    //@{
    const HierarchyLevel_ABC* Resolve( const QString& name ) const;
    const HierarchyLevel_ABC* Resolve( unsigned int id ) const;
    const HierarchyLevel_ABC* GetRoot() const;
    //@}

private:
    //! @name Helpers
    //@{
    Level* AddLevel( Level& root, const QString& name );
    //@}

private:
    //! @name Member data
    //@{
    Level* root_;
    //@}
};

}

#endif // __FormationLevels_h_
