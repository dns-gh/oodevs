// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CommunicationHierarchies_h_
#define __CommunicationHierarchies_h_

#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Updatable_ABC.h"

namespace kernel
{
    class Controller;
    class Entity_ABC;
    class InstanciationComplete;
}

// =============================================================================
/** @class  CommunicationHierarchies
    @brief  CommunicationHierarchies
*/
// Created: SBO 2006-09-25
// =============================================================================
class CommunicationHierarchies : public kernel::CommunicationHierarchies
                               , public kernel::Updatable_ABC< kernel::InstanciationComplete >
{

public:
    //! @name Constructors/Destructor
    //@{
             CommunicationHierarchies( kernel::Controller& controller, kernel::Entity_ABC& holder, kernel::Entity_ABC* superior );
    virtual ~CommunicationHierarchies();
    //@}

    //! @name Operations
    //@{
    virtual const kernel::Entity_ABC* GetSuperior() const;
    virtual const kernel::Entity_ABC& GetEntity() const;

    void ChangeSuperior( kernel::Entity_ABC& superior );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    CommunicationHierarchies( const CommunicationHierarchies& );            //!< Copy constructor
    CommunicationHierarchies& operator=( const CommunicationHierarchies& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const kernel::InstanciationComplete& );
    void RegisterToSuperior();
    void RemoveFromSuperior();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    kernel::Entity_ABC& holder_;
    kernel::Entity_ABC* superior_;
    //@}
};

#endif // __CommunicationHierarchies_h_
