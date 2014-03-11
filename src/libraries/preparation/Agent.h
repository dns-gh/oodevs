// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Agent_h_
#define __Agent_h_

#include "clients_gui/Drawable_ABC.h"
#include "clients_gui/EntityImplementation.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Displayable_ABC.h"
#include "clients_kernel/Serializable_ABC.h"
#include "clients_kernel/SubTypes.h"
#include "ENT/ENT_Enums.h"

namespace kernel
{
    class AgentType;
    class Automat_ABC;
    class Controller;
    class SymbolFactory;
}

namespace xml
{
    class xostream;
    class xistream;
}

namespace tools
{
    class IdManager;
}

// =============================================================================
/** @class  Agent
    @brief  Agent
*/
// Created: AGE 2006-02-13
// =============================================================================
class Agent : public gui::EntityImplementation< kernel::Agent_ABC >
            , public kernel::Extension_ABC
            , public kernel::Displayable_ABC
            , public gui::Drawable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Agent( const kernel::AgentType& type, kernel::Controller& controller, tools::IdManager& idManager );
             Agent( xml::xistream& xis, kernel::Controller& controller, tools::IdManager& idManager, const kernel::AgentType& type, const kernel::SymbolFactory& symbolFactory );
    virtual ~Agent();
    //@}

    //! @name Operations
    //@{
    virtual const kernel::AgentType& GetType() const;
    virtual E_NatureLevel GetLevel() const;
    virtual const std::string& GetNature() const;
    virtual const std::string& GetSymbol() const;
    virtual void SetLevel( E_NatureLevel level );
    virtual void SetSymbol( const std::string& symbol );
    virtual void SetNature( const std::string& nature );
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const;
    virtual void Pick( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const;
    virtual void DisplayInTooltip( kernel::Displayer_ABC& ) const;
    bool IsSymbolOverriden() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Agent( const Agent& );            //!< Copy constructor
    Agent& operator=( const Agent& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary();
    void InitializeSymbol() const;
    //@}

private:
    //! @name Member data
    //@{
    E_NatureLevel            level_;
    const kernel::AgentType& type_;
    mutable std::string      symbolPath_;
    mutable std::string      moveSymbol_;
    mutable std::string      staticSymbol_;
    mutable std::string      levelPath_;
    std::string              nature_;
    bool                     overridenSymbol_;
    const float              weight_;
    //@}
};

#endif // __Agent_h_
