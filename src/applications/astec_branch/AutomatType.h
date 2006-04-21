// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AutomatType_h_
#define __AutomatType_h_

#include "Resolver_ABC.h"
#include "Drawable_ABC.h"

class DecisionalModel;
class AgentType;
class SymbolFactory;

namespace xml { class xistream; };

// =============================================================================
/** @class  AutomatType
    @brief  AutomatType
*/
// Created: AGE 2006-02-14
// =============================================================================
class AutomatType : public Drawable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AutomatType( xml::xistream& xis, const Resolver_ABC< AgentType, std::string >& agentResolver
                                            , const Resolver_ABC< DecisionalModel, std::string >& modelResolver
                                            , const SymbolFactory& symbolFactory );
    virtual ~AutomatType();
    //@}

    //! @name Operations
    //@{
    unsigned long GetId();
    AgentType* GetTypePC();
    const DecisionalModel& GetDecisionalModel() const;
    void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const;

    bool IsTC2() const;
    bool IsLogisticSupply() const;
    bool IsLogisticMaintenance() const;
    bool IsLogisticMedical() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AutomatType( const AutomatType& );            //!< Copy constructor
    AutomatType& operator=( const AutomatType& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    unsigned long id_;
    std::string name_;
    std::string type_;

    DecisionalModel* model_;
    AgentType* pcType_;
    std::string symbol_;
    //@}
};

#endif // __AutomatType_h_
