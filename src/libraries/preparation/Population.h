// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Population_h_
#define __Population_h_

#include "EnumTypes.h"
#include "PopulationRepartition.h"
#include "clients_kernel/Displayable_ABC.h"
#include "clients_kernel/EntityImplementation.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Units.h"
#include "clients_kernel/Serializable_ABC.h"

namespace kernel
{
    class Team_ABC;
    class Controller;
    class PopulationType;
}

namespace xml
{
    class xistream;
}

namespace tools
{
    class IdManager;
}

// =============================================================================
/** @class  Population
    @brief  Population
*/
// Created: SBO 2006-11-08
// =============================================================================
class Population : public kernel::EntityImplementation< kernel::Population_ABC >
                 , public kernel::Extension_ABC
                 , public kernel::Serializable_ABC
                 , public kernel::Displayable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Population( const kernel::PopulationType& type, int number, kernel::Controller& controller, tools::IdManager& idManager );
             Population( xml::xistream& xis, const kernel::PopulationType& type, kernel::Controller& controller, tools::IdManager& idManager );
    virtual ~Population();
    //@}

    //! @name Operations
    //@{
    virtual const kernel::PopulationType& GetType() const;
    virtual unsigned int GetHealthyHumans() const;
    virtual unsigned int GetWoundedHumans() const;
    virtual unsigned int GetContaminatedHumans() const;
    virtual unsigned int GetDeadHumans() const;
    unsigned int GetTotalLivingHumans() const;
    virtual void DisplayInTooltip( kernel::Displayer_ABC& ) const;
    void SetKnowledgeGroupForSerialization( unsigned long knowledgeGroupId );
    //@}

private:
    //! @name Helpers
    //@{
    void CreateDictionary( kernel::Controller& controller );
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::PopulationType& type_;
    unsigned long healthy_;
    unsigned long wounded_;
    unsigned long dead_;
    unsigned long contaminated_;
    unsigned long knowledgeGroupId_;
    kernel::UnitedValue< unsigned int > armedIndividuals_;
    PopulationRepartition* repartition_;
    Enum_PopulationAttitude attitude_;
    QString criticalIntelligence_;
    //@}
};

#endif // __Population_h_
