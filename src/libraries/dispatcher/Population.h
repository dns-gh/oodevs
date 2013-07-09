 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Population_h_
#define __Population_h_

#include "Population_ABC.h"
#include "DecisionalState.h"

namespace sword
{
    class CrowdOrder;
    class DecisionalState;
    class CrowdFlowCreation;
    class CrowdFlowUpdate;
    class CrowdFlowDestruction;
    class CrowdCreation;
    class CrowdUpdate;
    class CrowdConcentrationCreation;
    class CrowdConcentrationUpdate;
    class CrowdConcentrationDestruction;
}

namespace dispatcher
{
    class KnowledgeGroup_ABC;
    class Model_ABC;
    class PopulationOrder;
    class Team_ABC;

// =============================================================================
/** @class  Population
    @brief  Population
    // $$$$ SBO 2010-06-10: refactor Flow/Concentration resolver to avoid downcasting
*/
// Created: NLD 2006-09-19
// =============================================================================
class Population : public dispatcher::Population_ABC
                 , public kernel::Extension_ABC
                 , public kernel::Updatable_ABC< sword::CrowdCreation >
                 , public kernel::Updatable_ABC< sword::CrowdUpdate >
                 , public kernel::Updatable_ABC< sword::CrowdConcentrationCreation >
                 , public kernel::Updatable_ABC< sword::CrowdConcentrationUpdate >
                 , public kernel::Updatable_ABC< sword::CrowdConcentrationDestruction >
                 , public kernel::Updatable_ABC< sword::CrowdFlowCreation >
                 , public kernel::Updatable_ABC< sword::CrowdFlowUpdate >
                 , public kernel::Updatable_ABC< sword::CrowdFlowDestruction >
                 , public kernel::Updatable_ABC< sword::CrowdOrder >
                 , public kernel::Updatable_ABC< sword::DecisionalState >
{
public:
    //! @name Constructors/Destructor
    //@{
             Population( Model_ABC& model, const sword::CrowdCreation& msg );
    virtual ~Population();
    //@}

    //! @name Operations
    //@{
    virtual void DoUpdate( const sword::CrowdCreation&                 msg );
    virtual void DoUpdate( const sword::CrowdUpdate&                   msg );
    virtual void DoUpdate( const sword::CrowdConcentrationCreation&    msg );
    virtual void DoUpdate( const sword::CrowdConcentrationUpdate&      msg );
    virtual void DoUpdate( const sword::CrowdConcentrationDestruction& msg );
    virtual void DoUpdate( const sword::CrowdFlowCreation&             msg );
    virtual void DoUpdate( const sword::CrowdFlowUpdate&               msg );
    virtual void DoUpdate( const sword::CrowdFlowDestruction&          msg );
    virtual void DoUpdate( const sword::CrowdOrder&                    msg );
    virtual void DoUpdate( const sword::DecisionalState&               msg );

    virtual void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    virtual void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    virtual void SendDestruction( ClientPublisher_ABC& publisher ) const;

    virtual void Accept( kernel::ModelVisitor_ABC& visitor ) const;

    virtual const kernel::PopulationType& GetType() const;
    virtual unsigned int GetHealthyHumans() const;
    virtual unsigned int GetWoundedHumans() const;
    virtual unsigned int GetContaminatedHumans() const;
    virtual unsigned int GetDeadHumans() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Population( const Population& );            //!< Copy constructor
    Population& operator=( const Population& ); //!< Assignment operator
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< unsigned long, float > T_Affinities;
    typedef T_Affinities::const_iterator   CIT_Affinities;
    //@}

private:
    //! @name Member data
    //@{
    Model_ABC& model_;
    const unsigned long nType_;
    const std::string strName_;
    dispatcher::Team_ABC& side_;
    dispatcher::KnowledgeGroup_ABC* knowledgeGroup_;
    float male_;
    float female_;
    float children_;
    unsigned int nDominationState_;
    std::auto_ptr< PopulationOrder > order_;
    DecisionalState decisionalInfos_;
    std::map< std::string, std::string > extensions_;
    T_Affinities affinities_;
    std::string criticalIntelligence_;
    float armedIndividuals_;
    //@}
};

}

#endif // __Population_h_
