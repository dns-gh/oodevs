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
#include <boost/optional.hpp>

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
class ModelConsistencyChecker : public kernel::ConsistencyChecker_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ModelConsistencyChecker( const Model& model, const StaticModel& staticModel, kernel::Controllers& controllers, tools::RealFileLoaderObserver_ABC& fileLoaderObserver );
    virtual ~ModelConsistencyChecker();
    //@}

public:
    //! @name Types
    //@{
    typedef kernel::SafePointer< kernel::Entity_ABC > T_Pointer;
    typedef std::vector< T_Pointer > T_Items;

    struct ConsistencyError
    {
        ConsistencyError( E_ConsistencyCheck type )
            : type_( type )
        {}
        bool IsError() const
        {
            if( isError_ )
                return *isError_;
            return type_ == eNoLogisticBase || type_ ==  eNoCommandPost || type_ ==  eSeveralCommandPost
                || type_ ==  eNoKnowledgeGroup || type_ ==  eScoreError || type_ ==  eSuccessFactorError
                || type_ == eProfileNoRole || type_ == eNoOrbat || type_ == eSignature
                || type_ == eBadLogisticSubordinate || type_ == eUnknownInfrastructure || type_ == eUnknownResourceNetwork
                || type_ == eDiffusionList || type_ == eMelmil || type_ == eDeletedUrbanBlocks || type_ == eDeletedPopulationUrbanBlocks
                || type_ == eCityAreaLimitExceeded || type_ == eImpossibleObjectCreation || type_ == eInhabitantBlockRemoved;
        }
        E_ConsistencyCheck type_;
        T_Items items_;
        std::string optional_;
        boost::optional< bool > isError_;
    };
    //@}

    //! @name Typedef
    //@{
    typedef std::vector< ConsistencyError > T_ConsistencyErrors;
    //@}

    //! @name Operations
    //@{
    const T_ConsistencyErrors& GetConsistencyErrors() const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual bool CheckConsistency();
    void AddError( E_ConsistencyCheck type, T_Pointer item, const std::string& optional = "" );
    //@}

    //! @name Typedef
    //@{
    typedef std::vector< const kernel::Entity_ABC* > T_Entities;
    typedef T_Entities::iterator                    IT_Entities;
    typedef T_Entities::const_iterator             CIT_Entities;
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
    void CheckProfileNumberOfElements();
    void CheckGhosts();
    void CheckCommandPosts();
    void CheckKnowledgeGroups();
    void CheckLongNameSize();
    void CheckLoadingErrors();
    void CheckExternalErrors();
    void CheckScores();
    void CheckSuccessFactors();
    void CheckLogisticBase();
    void CheckLogisticFormation();
    void CheckLogisticSubordinates();
    void CheckUrban();
    void CheckOrbat();
    void CheckFiles();
    void CheckDiffusionList();
    //@}

private:
    //! @name Member data
    //@{
    const Model&         model_;
    const StaticModel&   staticModel_;
    kernel::Controllers& controllers_;
    T_Entities           entities_;
    tools::RealFileLoaderObserver_ABC& fileLoaderObserver_;
    T_ConsistencyErrors errors_;
    std::vector< ConsistencyError > externalErrors_;
    //@}
};

#endif // __ModelConsistencyChecker_h_
