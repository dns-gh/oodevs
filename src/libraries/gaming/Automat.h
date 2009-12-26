// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Automat_h_
#define __Automat_h_

#include "clients_kernel/EntityImplementation.h"
#include "clients_kernel/Automat_ABC.h"
#include "tools/Resolver_ABC.h"
#include "clients_kernel/Drawable_ABC.h"
#include "clients_kernel/Extension_ABC.h"
#include "game_asn/Simulation.h"

namespace kernel
{
    class Controller;
    class PropertiesDictionary;
}

// =============================================================================
/** @class  Automat
    @brief  Automat
*/
// Created: AGE 2006-10-06
// =============================================================================
class Automat : public kernel::EntityImplementation< kernel::Automat_ABC >
              , public kernel::Extension_ABC
              , public kernel::Drawable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Automat( const ASN1T_MsgAutomatCreation& message, kernel::Controller& controller, 
                      const tools::Resolver_ABC< kernel::AutomatType >& resolver );
    virtual ~Automat();
    //@}

    //! @name Operations
    //@{
    virtual const kernel::AutomatType& GetType() const;
    virtual kernel::Team_ABC& GetTeam() const;
    virtual kernel::KnowledgeGroup_ABC& GetKnowledgeGroup() const;
    virtual void Register( kernel::Automat_ABC& automat );
    virtual void Remove( kernel::Automat_ABC& automat );
    virtual const tools::Resolver< kernel::Automat_ABC >& GetAutomats() const;
    virtual void Register( kernel::Agent_ABC& automat );
    virtual void Remove( kernel::Agent_ABC& automat );
    virtual const tools::Resolver< kernel::Agent_ABC >& GetAgents() const;
    virtual kernel::Automat_ABC* GetParentAutomat() const;
    virtual kernel::Formation_ABC* GetFormation() const;
    //@}

    //! @name Extensions
    //@{
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual void Accept( kernel::ModelVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Automat( const Automat& );            //!< Copy constructor
    Automat& operator=( const Automat& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::PropertiesDictionary& dico ) const;
    void InitializeSymbol() const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::AutomatType& type_;
    mutable std::string symbol_;
    //@}
};

#endif // __Automat_h_
