// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ModelConsistencyChecker_h_
#define __ModelConsistencyChecker_h_

#include <boost/noncopyable.hpp>

namespace kernel
{
    class Entity_ABC;
}

class Model;
class StaticModel;

// =============================================================================
/** @class  ModelConsistencyChecker
    @brief  ModelConsistencyChecker
*/
// Created: ABR 2011-09-22
// =============================================================================
class ModelConsistencyChecker : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ModelConsistencyChecker( const Model& model, const StaticModel& staticModel );
    virtual ~ModelConsistencyChecker();
    //@}

public:
    //! @name Types
    //@{
    enum E_ConsistencyCheck
    {
        eLongNameUniqueness              = 0x0001,
        eTeamNameUniqueness              = 0x0002,
        eObjectNameUniqueness            = 0x0004,
        eLimaNameUniqueness              = 0x0008,
        eLimitNameUniqueness             = 0x0010,
        eAllUniqueness                   = eLongNameUniqueness | eTeamNameUniqueness | eObjectNameUniqueness | eLimaNameUniqueness | eLimitNameUniqueness,
								         
        eStockInitialization             = 0x0020,
        eLogisticInitialization          = 0x0040,
        eAllInitialization               = eStockInitialization | eLogisticInitialization,
								         
        eAllCheckWithoutProfile          = eAllUniqueness | eAllInitialization,
								         
        eProfileUniqueness               = 0x0080,
        eProfileUnreadable               = 0x0100,
        eProfileUnwritable               = 0x0200,
        eAllProfile                      = eProfileUniqueness | eProfileUnreadable | eProfileUnwritable,
									     
        eGhostExistence                  = 0x0400,
        eGhostConverted                  = 0x0800,
        eAllGhost                        = eGhostExistence | eGhostConverted,

        eFormationWithSameLevelEmptiness = 0x1000,
        eAllFormation                    = eFormationWithSameLevelEmptiness,

        eAllChecks                       = eAllCheckWithoutProfile | eAllProfile | eAllGhost | eAllFormation
    };
    struct ConsistencyError
    {
        ConsistencyError( E_ConsistencyCheck type ) : type_( type ) {}

        E_ConsistencyCheck                       type_;
        std::vector< const kernel::Entity_ABC* > entities_;
        std::string                              optional_;
    };
    //@}

    //! @name Typedef
    //@{
    typedef std::vector< ConsistencyError >        T_ConsistencyErrors;
    typedef T_ConsistencyErrors::iterator         IT_ConsistencyErrors;
    typedef T_ConsistencyErrors::const_iterator  CIT_ConsistencyErrors;

    typedef std::vector< const kernel::Entity_ABC* >   T_Entities;
    typedef T_Entities::iterator                      IT_Entities;
    typedef T_Entities::const_iterator               CIT_Entities;
    //@}

    //! @name Operations
    //@{
    const T_ConsistencyErrors& GetConsistencyErrors() const;
    bool CheckConsistency( unsigned int filters = eAllChecks );
    //@}

private:
    //! @name Helpers
    //@{
    void AddError( E_ConsistencyCheck type, const kernel::Entity_ABC* entity, const std::string& optional = "" );
    void FillEntitiesCopy( E_ConsistencyCheck type );
    void CheckUniqueness( E_ConsistencyCheck type, bool ( *comparator )( const kernel::Entity_ABC&, const kernel::Entity_ABC& ) );
    void CheckStockInitialization();
    void CheckLogisticInitialization();
    void CheckProfileUniqueness();
    void CheckProfileInitialization();
    void CheckGhosts();
	void CheckFormationWithSameLevelAsParentEmptiness();
    //@}

private:
    //! @name Member data
    //@{
    const Model&        model_;
    const StaticModel&  staticModel_;
    unsigned int        filters_;

    T_ConsistencyErrors errors_;
    T_Entities          entities_;
    //@}
};

#endif // __ModelConsistencyChecker_h_
