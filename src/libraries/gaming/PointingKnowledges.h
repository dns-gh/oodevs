// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __PointingKnowledges_h_
#define __PointingKnowledges_h_

#include "clients_kernel/Extension_ABC.h"
#include <set>

namespace kernel
{
    class Knowledge_ABC;
}

// =============================================================================
/** @class  PointingKnowledges
    @brief  PointingKnowledges
*/
// Created: LDC 2013-07-02
// =============================================================================
class PointingKnowledges : public kernel::Extension_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             PointingKnowledges();
    virtual ~PointingKnowledges();
    //@}

    //! @name Operations
    //@{
    void Add( const kernel::Knowledge_ABC& knowledge );
    void Remove( const kernel::Knowledge_ABC& knowledge );
    template< typename Functor >
    void Apply( const Functor& functor ) const
    {
        for( auto it = knowledges_.begin(); it != knowledges_.end(); ++it )
            functor( *it );
    }

    bool IsKnown( unsigned int id ) const;
    const kernel::Knowledge_ABC* GetKnowledgeFromUnitId( unsigned int i ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PointingKnowledges( const PointingKnowledges& );            //!< Copy constructor
    PointingKnowledges& operator=( const PointingKnowledges& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::set< const kernel::Knowledge_ABC* > knowledges_;
    //@}
};

#endif // __PointingKnowledges_h_
