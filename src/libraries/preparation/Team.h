// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Team_h_
#define __Team_h_

#include "clients_kernel/Team_ABC.h"

namespace kernel
{
    class Controller;
}

class KnowledgeGroupFactory_ABC;

// =============================================================================
/** @class  Team
    @brief  Team
*/
// Created: SBO 2006-08-29
// =============================================================================
class Team : public kernel::Team_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Team( const QString& name, kernel::Controller& controller, KnowledgeGroupFactory_ABC& factory );
    virtual ~Team();
    //@}

    //! @name Accessors
    //@{
    virtual QString GetName() const;
    virtual unsigned long GetId() const;
    //@}

    //! @name Operations
    //@{
    void CreateKnowledgeGroup();
    void Rename( const QString& name );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Team( const Team& );            //!< Copy constructor
    Team& operator=( const Team& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    unsigned long GenerateKnowledgeGroupId() const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    KnowledgeGroupFactory_ABC& factory_;
    QString name_;
    unsigned long id_;
    //@}
};

#endif // __Team_h_
