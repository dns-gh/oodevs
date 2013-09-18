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

#include "clients_kernel/ConsistencyChecker_ABC.h"
#include "ConsistencyErrorTypes.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Controllers;
    class Entity_ABC;
}

namespace tools
{
    class RealFileLoaderObserver_ABC;
}

class Model;
class StaticModel;

// =============================================================================
/** @class  ModelConsistencyChecker
    @brief  ModelConsistencyChecker
*/
// Created: ABR 2011-09-22
// =============================================================================
class ModelConsistencyChecker : public kernel::ConsistencyChecker< E_ConsistencyCheck, kernel::SafePointer< kernel::Entity_ABC > >
{
    typedef kernel::ConsistencyChecker< E_ConsistencyCheck, kernel::SafePointer< kernel::Entity_ABC > > T_Parent;

public:
    //! @name Constructors/Destructor
    //@{
             ModelConsistencyChecker( Model& model, const StaticModel& staticModel, kernel::Controllers& controllers, tools::RealFileLoaderObserver_ABC& fileLoaderObserver );
    virtual ~ModelConsistencyChecker();
    //@}

public:
    //! @name Typedef
    //@{
    typedef std::vector< const kernel::Entity_ABC* > T_Entities;
    typedef T_Entities::iterator                    IT_Entities;
    typedef T_Entities::const_iterator             CIT_Entities;
    //@}

    //! @name Operations
    //@{
    virtual bool CheckConsistency();
    //@}

private:
    //! @name Helpers
    //@{
    void AddError( E_ConsistencyCheck type, const kernel::Entity_ABC* entity, const std::string& optional = "" );
    void FillEntitiesCopy( E_ConsistencyCheck type );
    void CheckUniqueness( E_ConsistencyCheck type, bool ( *comparator )( const kernel::Entity_ABC&, const kernel::Entity_ABC& ) );
    void CheckStockInitialization();
    void CheckMaxStockExceeded();
    void CheckMissingStock();
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
    void CheckOrbat();
    void CheckLogisticFormation();
    void CheckLogisticSubordinates();
    void CheckUrban();
    void CheckFiles();
    void CheckEquipmentState();
    //@}

private:
    //! @name Member data
    //@{
    Model&               model_;
    const StaticModel&   staticModel_;
    kernel::Controllers& controllers_;
    T_Entities           entities_;
    tools::RealFileLoaderObserver_ABC& fileLoaderObserver_;
    //@}
};

#endif // __ModelConsistencyChecker_h_
