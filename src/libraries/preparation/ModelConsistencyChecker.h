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
        eLongNameUniqueness     = 1,
        eTeamNameUniqueness     = 2,
        eObjectNameUniqueness   = 4,
        eLimaNameUniqueness     = 8,
        eLimitNameUniqueness    = 16,
        eStockInitialization    = 32,
        eLogisticInitialization = 64,
        eAllUniqueness          = 31,
        eAllInitialization      = 96,
        eAllChecks              = 127
    };
    struct ConsistencyError
    {
        ConsistencyError( E_ConsistencyCheck type ) : type_( type ) {}

        E_ConsistencyCheck                       type_;
        std::vector< const kernel::Entity_ABC* > entities_;
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
    void FillEntitiesCopy( E_ConsistencyCheck type );
    void CheckUniqueness( E_ConsistencyCheck type, bool ( *comparator )( const kernel::Entity_ABC&, const kernel::Entity_ABC& ) );
    void CheckStockInitialization();
    void CheckLogisticInitialization();
    //@}

private:
    //! @name Member data
    //@{
    const Model&        model_;
    const StaticModel&  staticModel_;

    T_ConsistencyErrors errors_;
    T_Entities          entities_;
    //@}
};

#endif // __ModelConsistencyChecker_h_
