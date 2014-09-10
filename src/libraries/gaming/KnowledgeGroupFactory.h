// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __KnowledgeGroupFactory_h_
#define __KnowledgeGroupFactory_h_

#include "KnowledgeGroupFactory_ABC.h"

namespace actions
{
    class ActionsModel;
}

namespace kernel
{
    class Controllers;
    class Profile_ABC;
}

class Model;

// =============================================================================
/** @class  KnowledgeGroupFactory
    @brief  KnowledgeGroupFactory
*/
// Created: HBD 2010-06-23
// =============================================================================
class KnowledgeGroupFactory : public KnowledgeGroupFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             KnowledgeGroupFactory( kernel::Controllers& controllers,
                                    Model& model,
                                    const kernel::Profile_ABC& profile,
                                    actions::ActionsModel& actionsModel );
    virtual ~KnowledgeGroupFactory();
    //@}

    //! @name Operations
    //@{
    virtual kernel::KnowledgeGroup_ABC* CreateKnowledgeGroup( const sword::KnowledgeGroupCreation& message ); // LTO
    //@}

private:
    //! @name Copy/Assignment
    //@{
    KnowledgeGroupFactory( const KnowledgeGroupFactory& );            //!< Copy constructor
    KnowledgeGroupFactory& operator=( const KnowledgeGroupFactory& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    Model& model_;
    const kernel::Profile_ABC& profile_;
    actions::ActionsModel& actionsModel_;
    //@}
};

#endif // __KnowledgeGroupFactory_h_
