// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_AutomatCreater_h
#define plugins_hla_AutomatCreater_h

#include "ResponseObserver_ABC.h"
#include "tools/Resolver_ABC.h"
#include <map>

namespace kernel
{
    class AutomatType;
}

namespace dispatcher
{
    class KnowledgeGroup_ABC;
}

namespace sword
{
    class FormationCreation;
    class AutomatCreation;
}

namespace plugins
{
namespace hla
{
    template< typename ResponseMessage > class ContextHandler_ABC;

// =============================================================================
/** @class  AutomatCreater
    @brief  Automat creater
*/
// Created: SLI 2011-09-14
// =============================================================================
class AutomatCreater : private ResponseObserver_ABC< sword::FormationCreation >
{
public:
    //! @name Constructors/Destructor
    //@{
             AutomatCreater( ContextHandler_ABC< sword::FormationCreation >& formationCreation,
                             ContextHandler_ABC< sword::AutomatCreation >& automatCreation,
                             const tools::Resolver_ABC< kernel::AutomatType >& automatTypes,
                             const tools::Resolver_ABC< dispatcher::KnowledgeGroup_ABC >& knowledgeGroups );
    virtual ~AutomatCreater();
    //@}

private:
    //! @name Operations
    //@{
    virtual void Notify( const sword::FormationCreation& message, const std::string& identifier );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< unsigned long, unsigned long > T_Identifiers;
    //@}

private:
    //! @name Member data
    //@{
    const unsigned long automatType_;
    ContextHandler_ABC< sword::FormationCreation >& formationCreation_;
    ContextHandler_ABC< sword::AutomatCreation >& automatCreation_;
    T_Identifiers identifiers_;
    //@}
};

}
}

#endif // plugins_hla_AutomatCreater_h
