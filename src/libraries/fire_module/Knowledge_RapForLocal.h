// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef fire_module_Knowledge_RapForLocal_h
#define fire_module_Knowledge_RapForLocal_h

#include <wrapper/View.h>
#include <vector>
#include <boost/noncopyable.hpp>

namespace xml
{
    class xisubstream;
}

namespace sword
{
namespace fire
{
// =============================================================================
/** @class  Knowledge_RapForLocal
    @brief  Knowledge local RapFor
*/
// Created: NLD 2004-04-07
// =============================================================================
class Knowledge_RapForLocal : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef std::vector< const SWORD_Model* >   T_KnowledgeAgents;
    typedef T_KnowledgeAgents::const_iterator CIT_KnowledgeAgents;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    Knowledge_RapForLocal();
    //@}

    //! @name Accessors
    //@{
    void Update( const wrapper::View& model, const wrapper::View& entity );
    double GetValue() const;
    const T_KnowledgeAgents& GetDangerousEnemies() const;
    //@}

    //! @name Tools
    //@{
    static void Initialize( xml::xisubstream xis, double tickDuration );
    //@}

private:
    //! @name Operations
    //@{
    void ApplyValue( double rNewRapForValue, double rFeedbackTime );
    //@}

private:
    //! @name Member data
    //@{
    unsigned int nLastCacheUpdateTick_;
    double rRapForValue_;
    T_KnowledgeAgents dangerousEnemies_;
    static double rRapForIncreasePerTimeStepDefaultValue_;
    //@}
};

}
}

#endif // fire_module_Knowledge_RapForLocal_h
