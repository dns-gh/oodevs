// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentType_h_
#define __AgentType_h_

#include "Resolver_ABC.h"

namespace xml { class xistream; };

namespace kernel
{
    class ComponentType;
    class AgentNature;
    class DecisionalModel;
    class SymbolFactory;
    class GlTools_ABC;
    class Viewport_ABC;

// =============================================================================
/** @class  AgentType
    @brief  AgentType
*/
// Created: AGE 2006-02-14
// =============================================================================
class AgentType
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentType( xml::xistream& xis, const Resolver_ABC< ComponentType, QString >& componentResolver
                                          , const Resolver_ABC< DecisionalModel, QString >& modelResolver
                                          , const SymbolFactory& symbolFactory );
    virtual ~AgentType();
    //@}

    //! @name Operations
    //@{
    unsigned long GetId() const;
    QString GetName() const;
    const DecisionalModel& GetDecisionalModel() const;
    const AgentNature& GetNature() const;

    const std::string& GetSymbol() const;
    const std::string& GetLevelSymbol() const;
    const std::string& GetHQSymbol() const;

    void Draw( const geometry::Point2f& where, const Viewport_ABC& viewport, const GlTools_ABC& tools, bool pc ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentType( const AgentType& );            //!< Copy constructor
    AgentType& operator=( const AgentType& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< const ComponentType* > T_Components;
    //@}

    //! @name Helpers
    //@{
    void ReadEquipment( xml::xistream& xis, const Resolver_ABC< ComponentType, QString >& resolver  );
    //@}

private:
    //! @name Member data
    //@{
    QString name_;
    unsigned long id_;

    DecisionalModel* model_;
    AgentNature* nature_;
    T_Components equipments_;
    std::string symbol_; // $$$$ AGE 2006-10-24: devrait etre dans nature
    std::string levelSymbol_;
    std::string hqSymbol_;
    //@}
};

}

#endif // __AgentType_h_
