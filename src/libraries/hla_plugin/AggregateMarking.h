// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AggregateMarking_h_
#define __AggregateMarking_h_

#include <boost/noncopyable.hpp>

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  AggregateMarking
    @brief  Aggregate marking
*/
// Created: AGE 2008-02-21
// =============================================================================
class AggregateMarking : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit AggregateMarking( const std::string& name );
    virtual ~AggregateMarking();
    //@}

    //! @name Operations
    //@{
    template< typename Archive >
    void Serialize( Archive& archive )
    {
        archive << markingType_;
        archive << markingData_;
    }
    //@}

private:
    //! @name Member data
    //@{
    unsigned char markingType_;
    unsigned char markingData_[31];
    //@}
};

}
}

#endif // __AggregateMarking_h_
