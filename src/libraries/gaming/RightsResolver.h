// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __RightsResolver_h_
#define __RightsResolver_h_

#include "clients_kernel/UserProfile_ABC.h"
#include "clients_kernel/UserRights.h"
#include "protocol/AuthenticationSenders.h"
#include <tools/ElementObserver_ABC.h>
#include <tools/Resolver_ABC.h>

namespace kernel
{
    class Automat_ABC;
    class CommunicationHierarchies;
    class Hierarchies;
    class Formation_ABC;
    class Knowledge_ABC;
    class Population_ABC;
    class Team_ABC;
    class Entity_ABC;
}

class Model;

// =============================================================================
/** @class  RightsResolver
    @brief  Rights resolver
*/
// Created: LGY 2011-11-25
// =============================================================================
class RightsResolver : public kernel::UserProfile_ABC
                     , public tools::Observer_ABC
                     , public tools::ElementObserver_ABC< kernel::Automat_ABC >
                     , public tools::ElementObserver_ABC< kernel::Population_ABC >
                     , public tools::ElementObserver_ABC< kernel::Team_ABC >
                     , public tools::ElementObserver_ABC< kernel::Formation_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             RightsResolver();
             RightsResolver( const RightsResolver& resolver );
    virtual ~RightsResolver();
    //@}

    //! @name From Profile_ABC
    //@{
    virtual bool IsVisible( const kernel::Entity_ABC& entity ) const;
    virtual bool IsKnowledgeVisible( const kernel::Knowledge_ABC& knowledge ) const;
    virtual bool IsKnowledgeVisibleNoSupervision( const kernel::Knowledge_ABC& knowledge ) const;
    virtual bool IsPerceived( const kernel::Entity_ABC& ) const;
    virtual bool CanBeOrdered( const kernel::Entity_ABC& entity ) const;
    virtual bool CanDoMagic( const kernel::Entity_ABC& entity ) const;
    //@}

private:
    //! @name Types
    //@{
    //@{
    typedef std::vector< unsigned long > T_Ids;
    typedef T_Ids::const_iterator      CIT_Ids;

    typedef std::set< const kernel::Entity_ABC* > T_Entities;
    typedef T_Entities::const_iterator          CIT_Entities;
    //@}

protected:
    //! @name Operations
    //@{
    virtual void Update( const Model& model );
    void Update( const sword::Profile& profile );
    //@}

private:
    //! @name Operations
    //@{
    virtual void NotifyCreated( const kernel::Automat_ABC& automat );
    virtual void NotifyDeleted( const kernel::Automat_ABC& automat );
    virtual void NotifyCreated( const kernel::Population_ABC& popu );
    virtual void NotifyDeleted( const kernel::Population_ABC& popu );
    virtual void NotifyCreated( const kernel::Team_ABC& team );
    virtual void NotifyDeleted( const kernel::Team_ABC& team );
    virtual void NotifyCreated( const kernel::Formation_ABC& formation );
    virtual void NotifyDeleted( const kernel::Formation_ABC& formation );
    //@}

private:
    //! @name Helpers
    //@{
    void Add( const kernel::Entity_ABC& entity, const T_Ids& readIds, const T_Ids& readWriteIds );
    void Remove( const kernel::Entity_ABC& entity );
    template< typename Entity >
    void ResolveEntities( const tools::Resolver_ABC< Entity >& resolver, const T_Ids& readIds, const T_Ids& readWriteIds );
    static bool IsChildOfCommunicationHierarchy( const kernel::CommunicationHierarchies& target, const kernel::Entity_ABC& entity );
    static bool IsInHierarchy( const kernel::Entity_ABC& entityToTest, const T_Entities& entities, bool childOnly );
    static bool IsInHierarchy( const kernel::Entity_ABC& entity, const kernel::Hierarchies& hierarchy, const kernel::Entity_ABC& other, bool childOnly );
    static bool AreInSameKnowledgeGroup( const kernel::Entity_ABC& entity1, const kernel::Entity_ABC& entity2, bool compareTop );
    static bool IsInSpecificHierarchy( const kernel::Entity_ABC& entity, const kernel::Hierarchies* hierarchy, const T_Entities& entities, bool childOnly );
    //@}

protected:
    //! @name Member data
    //@{
    T_Entities readEntities_;
    T_Entities readWriteEntities_;
    kernel::UserRights rights_;
    //@}
};

#endif // __RightsResolver_h_
