// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __Score_h_
#define __Score_h_

#include <vector>

namespace sword
{
    class Indicator;
}

namespace xml
{
    class xistream;
}

namespace dispatcher
{
    class ClientPublisher_ABC;
}

namespace plugins
{
namespace score
{

// =============================================================================
/** @class  Score
    @brief  Score
*/
// Created: SBO 2009-04-29
// =============================================================================
class Score : public std::vector< double >
{
public:
    //! @name Constructors/Destructor
    //@{
             Score();
    virtual ~Score();
    //@}

    //! @name Operations
    //@{
    void Update( const sword::Indicator& message );
    void Send( dispatcher::ClientPublisher_ABC& publisher, int context ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Score( const Score& );            //!< Copy constructor
    Score& operator=( const Score& ); //!< Assignment operator
    //@}
};

}
}

#endif // __Score_h_
