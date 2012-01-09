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

#include "clients_kernel/SafePointer.h"
#include "ConsistencyErrorTypes.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Controllers;
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
             ModelConsistencyChecker( const Model& model, const StaticModel& staticModel, kernel::Controllers& controllers );
    virtual ~ModelConsistencyChecker();
    //@}

public:
    //! @name Types
    //@{
    typedef std::vector< kernel::SafePointer< kernel::Entity_ABC >* > T_SafeEntities;
    typedef T_SafeEntities::iterator                                 IT_SafeEntities;
    typedef T_SafeEntities::const_iterator                          CIT_SafeEntities;

    struct ConsistencyError
    {
        ConsistencyError( E_ConsistencyCheck type ) : type_( type ) {}

        E_ConsistencyCheck type_;
        T_SafeEntities     entities_;
        std::string        optional_;
    };
    //@}

    //! @name Typedef
    //@{
    typedef std::vector< ConsistencyError >       T_ConsistencyErrors;
    typedef T_ConsistencyErrors::iterator        IT_ConsistencyErrors;
    typedef T_ConsistencyErrors::const_iterator CIT_ConsistencyErrors;

    typedef std::vector< const kernel::Entity_ABC* > T_Entities;
    typedef T_Entities::iterator                    IT_Entities;
    typedef T_Entities::const_iterator             CIT_Entities;
    //@}

    //! @name Operations
    //@{
    const T_ConsistencyErrors& GetConsistencyErrors() const;
    bool CheckConsistency();

    //@}

private:
    //! @name Helpers
    //@{
    void ClearErrors();
    void AddError( E_ConsistencyCheck type, const kernel::Entity_ABC* entity, const std::string& optional = "" );
    void FillEntitiesCopy( E_ConsistencyCheck type );
    void CheckUniqueness( E_ConsistencyCheck type, bool ( *comparator )( const kernel::Entity_ABC&, const kernel::Entity_ABC& ) );
    void CheckStockInitialization();
    void CheckMaxStockExceeded();
    void CheckLogisticInitialization();
    void CheckProfileUniqueness();
    void CheckProfileInitialization();
    void CheckGhosts();
    void CheckCommandPosts();
    void CheckKnowledgeGroups();
    void CheckLoadingErrors();
    void CheckScores();
    void CheckSuccessFactors();
    void CheckLogisticBase();
    //@}

private:
    //! @name Member data
    //@{
    const Model&         model_;
    const StaticModel&   staticModel_;
    kernel::Controllers& controllers_;
    T_ConsistencyErrors  errors_;
    T_Entities           entities_;
    //@}
};

#endif // __ModelConsistencyChecker_h_
