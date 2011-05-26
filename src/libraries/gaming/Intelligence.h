// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Intelligence_h_
#define __Intelligence_h_

#include "clients_kernel/EntityImplementation.h"
#include "clients_kernel/Intelligence_ABC.h"
#include "clients_kernel/Drawable_ABC.h"
#include "clients_kernel/Displayable_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "tools/Resolver_ABC.h"
#include "protocol/ServerPublisher_ABC.h"

namespace sword
{
    class IntelligenceCreation;
    class IntelligenceUpdate;
}

namespace kernel
{
    class Formation_ABC;
    class HierarchyLevel_ABC;
    class SymbolFactory;
}

class Publisher_ABC;

// =============================================================================
/** @class  Intelligence
    @brief  Intelligence
*/
// Created: SBO 2007-10-17
// =============================================================================
class Intelligence : public kernel::EntityImplementation< kernel::Intelligence_ABC >
                   , public kernel::Extension_ABC
                   , public kernel::Drawable_ABC
                   , public kernel::Displayable_ABC
                   , public kernel::Updatable_ABC< sword::IntelligenceUpdate >
{
public:
    //! @name Constructors/Destructor
    //@{
             Intelligence( const sword::IntelligenceCreation& message, kernel::Controller& controller
                         , const tools::Resolver_ABC< kernel::Formation_ABC >& formations
                         , const tools::Resolver_ABC< kernel::HierarchyLevel_ABC >& levels
                         , Publisher_ABC& publisher, kernel::SymbolFactory& factory );
    virtual ~Intelligence();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual void DisplayInTooltip( kernel::Displayer_ABC& ) const;
    virtual void DisplayInSummary( kernel::Displayer_ABC& ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Intelligence( const Intelligence& );            //!< Copy constructor
    Intelligence& operator=( const Intelligence& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::Controller& controller );
    virtual const kernel::Karma& GetKarma() const;
    virtual std::string GetSymbol() const;
    virtual const kernel::HierarchyLevel_ABC& GetLevel() const;
    virtual bool IsMounted() const;
    virtual void Delete();
    virtual void Rename( const QString& name );
    virtual void DoUpdate( const sword::IntelligenceUpdate& message );
    //@}

private:
    //! @name Member data
    //@{
    const tools::Resolver_ABC< kernel::HierarchyLevel_ABC >& levels_;
    const kernel::Formation_ABC& formation_;
    std::string symbol_;
    const kernel::HierarchyLevel_ABC* level_;
    const kernel::Karma* karma_;
    bool mounted_;
    Publisher_ABC& publisher_;
    const std::string levelSymbol_;
    //@}
};

#endif // __Intelligence_h_
