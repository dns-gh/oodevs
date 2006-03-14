// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamPopulationKnowledge_h_
#define __ParamPopulationKnowledge_h_

#include "ASN_Types.h"
#include "Param_ABC.h"
#include "ContextMenuObserver_ABC.h"

class PopulationKnowledge;

// =============================================================================
/** @class  ParamPopulationKnowledge
    @brief  ParamPopulationKnowledge
*/
// Created: AGE 2006-03-14
// =============================================================================
class ParamPopulationKnowledge : public QHBox
                               , public Param_ABC
                               , public ContextMenuObserver_ABC< PopulationKnowledge >
{

public:
    //! @name Constructors/Destructor
    //@{
             ParamPopulationKnowledge( ASN1T_KnowledgePopulation& asn, Agent_ABC& agent, const std::string& label, QWidget* pParent, bool bOptional );
    virtual ~ParamPopulationKnowledge();
    //@}

    //! @name Operations
    //@{
    virtual bool CheckValidity();
    virtual void Commit();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ParamPopulationKnowledge( const ParamPopulationKnowledge& );            //!< Copy constructor
    ParamPopulationKnowledge& operator=( const ParamPopulationKnowledge& ); //!< Assignement operator
    //@}

private slots:
    //! @name Slots
    //@{
    //@}

private:
    //! @name Member data
    //@{
    ASN1T_KnowledgePopulation& asn_;
    Agent_ABC& agent_;
    const PopulationKnowledge* selected_;
    //@}
};

#endif // __ParamPopulationKnowledge_h_
