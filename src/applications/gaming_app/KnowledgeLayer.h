// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __KnowledgeLayer_h_
#define __KnowledgeLayer_h_

#include "clients_gui/EntityLayer.h"
#include "clients_kernel/Profile_ABC.h"

// =============================================================================
/** @class  KnowledgeLayer
    @brief  Knowledge layer
*/
// Created: AGE 2006-05-18
// =============================================================================
template< typename KnowledgeType >
class KnowledgeLayer : public gui::EntityLayer< KnowledgeType >
{

public:
    //! @name Constructors/Destructor
    //@{
             KnowledgeLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy, gui::View_ABC& view, const kernel::Profile_ABC& profile );
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
    virtual bool ShouldDisplay( const kernel::Entity_ABC& );
    //@}
};

// -----------------------------------------------------------------------------
// Name: KnowledgeLayer constructor
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
template< typename KnowledgeType >
KnowledgeLayer< KnowledgeType >::KnowledgeLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy, gui::View_ABC& view, const kernel::Profile_ABC& profile )
    : gui::EntityLayer< KnowledgeType >( controllers, tools, strategy, view, profile )
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
bool KnowledgeLayer< KnowledgeType >::ShouldDisplay( const kernel::Entity_ABC& entity )
{
    const kernel::Knowledge_ABC& k = static_cast< const kernel::Knowledge_ABC& >( entity );
    const kernel::Entity_ABC* e = k.GetEntity();
    return e && ! profile_.IsVisible( *e )
             && profile_.IsInSameKnowledgeGroup( k.GetOwner() );
}

#endif // __KnowledgeLayer_h_
