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

#include "clients_kernel/EntityImplementation.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Drawable_ABC.h"
#include "clients_kernel/Serializable_ABC.h"

namespace kernel
{
    class Controller;
    class AgentType;
    class AgentTypes;
    class Automat_ABC;
}

namespace xml
{
    class xostream;
    class xistream;
}

class IdManager;

// =============================================================================
/** @class  Agent
    @brief  Agent
*/
// Created: AGE 2006-02-13
// =============================================================================
class Agent : public kernel::EntityImplementation< kernel::Agent_ABC >
            , public kernel::Extension_ABC
            , public kernel::Drawable_ABC
            , public kernel::Serializable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Agent( const kernel::AgentType& type, kernel::Controller& controller, IdManager& idManager, bool commandPost = false );
             Agent( xml::xistream& xis, kernel::Controller& controller, IdManager& idManager, const kernel::AgentTypes& agentTypes ); // $$$$ AGE 2006-10-25: resolver, par agentTypes concret...
    virtual ~Agent();
    //@}

    //! @name Operations
    //@{
    virtual const kernel::AgentType& GetType() const;
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    virtual void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const kernel::GlTools_ABC& tools ) const;

    virtual bool IsCommandPost() const;
    void Rename( const QString& name );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Agent( const Agent& );            //!< Copy constructor
    Agent& operator=( const Agent& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::Controller& controller );
    static unsigned long ReadId  ( xml::xistream& xis );
    static QString       ReadName( xml::xistream& xis );
    void InitializeSymbol() const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::AgentType* type_;
    mutable std::string symbol_;
    bool commandPost_;
    //@}
};

#endif // __Agent_h_
