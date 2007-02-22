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

namespace xml { class xistream; };

namespace kernel
{
    class DecisionalModel;
    class AgentType;
    class AutomatComposition;
    template< typename Container > class Iterator;

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
             AutomatType( xml::xistream& xis, const Resolver_ABC< AgentType, QString >& agentResolver
                                            , const Resolver_ABC< DecisionalModel, QString >& modelResolver );
    virtual ~AutomatType();
    //@}

    //! @name Operations
    //@{
    unsigned long GetId();
    QString GetName() const;
    AgentType* GetTypePC() const;
    std::string GetSymbol() const;
    const DecisionalModel& GetDecisionalModel() const;
    void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const;
    Iterator< const AutomatComposition& > CreateIterator() const;
    
    bool HasLogistics() const;
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

    //! @name Helpers
    //@{
    void ReadAgent( xml::xistream& xis, const Resolver_ABC< AgentType, QString >& agentResolver );
    //@}

    //! @name Types
    //@{
    typedef std::vector< AutomatComposition* > T_UnitConstitution;
    //@}

private:
    //! @name Member data
    //@{
    unsigned long id_;
    QString name_;
    std::string type_;
    std::string symbol_;

    DecisionalModel* model_;
    AgentType* pcType_;
    T_UnitConstitution units_;
    //@}
};

}

#endif // __AutomatType_h_
