// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __KnowledgeLayer_h_
#define __KnowledgeLayer_h_

#include "EntityLayer.h"

namespace gui
{

// =============================================================================
/** @class  KnowledgeLayer
    @brief  Knowledge layer
*/
// Created: AGE 2006-05-18
// =============================================================================
template< typename KnowledgeType >
class KnowledgeLayer : public EntityLayer< KnowledgeType >
{

public:
    //! @name Constructors/Destructor
    //@{
             KnowledgeLayer( Controllers& controllers, const GlTools_ABC& tools, ColorStrategy_ABC& strategy, View_ABC& view );
    virtual ~KnowledgeLayer();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    KnowledgeLayer( const KnowledgeLayer& );            //!< Copy constructor
    KnowledgeLayer& operator=( const KnowledgeLayer& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual bool ShouldDisplay( const Entity_ABC& );
    //@}
};

// -----------------------------------------------------------------------------
// Name: KnowledgeLayer constructor
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
template< typename KnowledgeType >
KnowledgeLayer< KnowledgeType >::KnowledgeLayer( Controllers& controllers, const GlTools_ABC& tools, ColorStrategy_ABC& strategy, View_ABC& view )
    : EntityLayer< KnowledgeType >( controllers, tools, strategy, view )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: KnowledgeLayer destructor
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
template< typename KnowledgeType >
KnowledgeLayer< KnowledgeType >::~KnowledgeLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: KnowledgeLayer::ShouldDisplay
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
template< typename KnowledgeType >
bool KnowledgeLayer< KnowledgeType >::ShouldDisplay( const Entity_ABC& entity )
{
    const KnowledgeType& k = static_cast< const KnowledgeType& >( entity );
    return currentTeam_
        && k.IsInTeam( *currentTeam_ )
        && ! k.KnowledgeIsInTeam( *currentTeam_ );
}

}

#endif // __KnowledgeLayer_h_
