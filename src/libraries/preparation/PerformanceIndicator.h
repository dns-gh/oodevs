// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __PerformanceIndicator_h_
#define __PerformanceIndicator_h_

#include <boost/noncopyable.hpp>

namespace tools
{
    class ExerciseConfig;
}

namespace kernel
{
    class Agent_ABC;
    class Automat_ABC;
    class Entity_ABC;
    class Inhabitant_ABC;
    class KnowledgeGroup_ABC;
    class Object_ABC;
    class Population_ABC;
    class Team_ABC;
}

namespace xml
{
    class xistream;
}

class Model;

// =============================================================================
/** @class  PerformanceIndicator
    @brief  PerformanceIndicator
*/
// Created: MMC 2012-02-02
// =============================================================================
class PerformanceIndicator : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             PerformanceIndicator( const Model& model );
    virtual ~PerformanceIndicator();
    //@}

    //! @name Types
    //@{
    class KnowledgeGroupData
    {
    public:
        KnowledgeGroupData() : automats_( 0 ), units_( 0 ) {}
        unsigned int automats_;
        unsigned int units_;
    };

    typedef std::map< std::string, KnowledgeGroupData >   T_KGDatas;
    typedef T_KGDatas::const_iterator                   CIT_KGDatas;

    typedef std::map< std::string, unsigned int >    T_EntityTypeCount;
    typedef T_EntityTypeCount::const_iterator      CIT_EntityTypeCount;

    class TeamData
    {
    public:

        TeamData() : formations_( 0 ), units_( 0 ), automats_( 0 ), crowds_( 0 ), populations_( 0 ), knowledgeGroups_( 0 ), objects_( 0 ) {}

        std::string name_;
        unsigned int formations_;
        unsigned int units_;
        unsigned int automats_;
        unsigned int crowds_;
        unsigned int populations_;
        unsigned int knowledgeGroups_;
        unsigned int objects_;
        T_EntityTypeCount automatTypes_;
        T_EntityTypeCount unitTypes_;
        T_EntityTypeCount objectTypes_;
        T_KGDatas datasKG_;
    };

    typedef std::map< unsigned int, TeamData > T_TeamsDatas;
    typedef T_TeamsDatas::const_iterator     CIT_TeamsDatas;

    struct Values 
    {
        Values(): limit_( 0 ), profiles_( 0 )
                         , units_( 0 ), automats_( 0 ), blocs_( 0 ), objects_( 0 ), crowds_( 0 ), populations_( 0 )
                         , terrainWidth_( 0 ), terrainHeight_( 0 )   
                         , terrainLoad_( 0.f ), teams_( 0 ), knowledgeGroups_( 0 )
                         , maxAutomatsKG_( 0 ), maxUnitsKG_( 0 )
                         , avgAutomatsKG_( 0 ), avgUnitsKG_( 0 )
                         , automatsKG_( 0 ), unitsKG_( 0 )
                         , performance_( 0.f ) {}

        std::string  exercise_;
        unsigned int limit_;
        unsigned int units_;
        unsigned int automats_;
        unsigned int blocs_;
        unsigned int objects_;
        unsigned int crowds_;
        unsigned int populations_;
        float terrainLoad_;
        unsigned int teams_;
        unsigned int formations_;
        unsigned int knowledgeGroups_;
        unsigned int terrainWidth_;
        unsigned int terrainHeight_;
        unsigned int profiles_;
        unsigned int maxAutomatsKG_;
        unsigned int maxUnitsKG_;
        unsigned int avgAutomatsKG_;
        unsigned int avgUnitsKG_;
        unsigned int automatsKG_;
        unsigned int unitsKG_;
        T_TeamsDatas teamsDatas_;
        float performance_;
    };
    //@}

    //! @name Operations
    //@{
    void Load( const tools::ExerciseConfig& config, const std::string& file );
    const PerformanceIndicator::Values& ComputeValues();
    //@}

private:

    //! @name Types
    //@{
    class TeamGetInfoFunctor : private boost::noncopyable
    {
    public:
        TeamGetInfoFunctor( Values& values ) : values_( values ) {}
        void operator()( const kernel::Team_ABC& team ) const;
        void Compute( TeamData& teamData, const kernel::Entity_ABC& entity ) const;
        Values& values_;
    };

    class ObjectGetInfoFunctor : private boost::noncopyable
    {
    public:
        ObjectGetInfoFunctor( Values& values ) : values_( values ) {}
        void operator()( const kernel::Object_ABC& object ) const;
        Values& values_;
    };

    class KGGetInfoFunctor : private boost::noncopyable
    {
    public:
        KGGetInfoFunctor( Values& values ) : values_( values ) {}
        void operator()( const kernel::KnowledgeGroup_ABC& kg ) const;
        Values& values_;
    };
    //@}

    //! @name Helpers
    //@{
    void Read( xml::xistream& xis );
    void Update();
    float ComputeFormulaIndicator( Values& values );
    void UpdatePopulation( Values& values ) const;
    void UpdateCrowds( Values& values ) const;
    //@}

    const Model& model_;
    float limit_;
    float globalFactor_;
    float unit_;
    float urban_;
    float object_;
    float terrain_;
    float knowledge_;
    float unitknowledge_;
    Values values_;
};

#endif // __PerformanceIndicator_h_
