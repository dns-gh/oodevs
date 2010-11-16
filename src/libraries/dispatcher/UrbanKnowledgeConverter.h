// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __UrbanKnowledgeConverter_h_
#define __UrbanKnowledgeConverter_h_

#include "clients_kernel/UrbanKnowledgeConverter_ABC.h"

namespace dispatcher
{
    class Model_ABC;

// =============================================================================
/** @class  UrbanKnowledgeConverter
    @brief  UrbanKnowledgeConverter
*/
// Created: SBO 2010-05-11
// =============================================================================
class UrbanKnowledgeConverter : public kernel::UrbanKnowledgeConverter_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit UrbanKnowledgeConverter( const dispatcher::Model_ABC& model );
    virtual ~UrbanKnowledgeConverter();
    //@}

    //! @name Operations
    //@{
    virtual const kernel::UrbanKnowledge_ABC* Find( unsigned long id, const kernel::Team_ABC& owner ) const;
    virtual const kernel::UrbanKnowledge_ABC* Find( unsigned long id, const kernel::Entity_ABC& owner ) const;
    virtual const kernel::UrbanKnowledge_ABC* Find( const kernel::Entity_ABC& base, const kernel::Team_ABC& owner ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    UrbanKnowledgeConverter( const UrbanKnowledgeConverter& );            //!< Copy constructor
    UrbanKnowledgeConverter& operator=( const UrbanKnowledgeConverter& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const dispatcher::Model_ABC& model_;
    //@}
};

}

#endif // __UrbanKnowledgeConverter_h_
