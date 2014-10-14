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

#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <boost/optional/optional_fwd.hpp>
#include <deque>
#include <vector>

namespace sword
{
    class Indicator;
}

namespace dispatcher
{
    class ClientPublisher_ABC;
}

namespace xml
{
    class xibufferstream;
    class xistream;
    class xisubstream;
    class xostream;
}

namespace plugins
{
namespace score
{
    class ScoreAnnouncer;

// =============================================================================
/** @class  Score
    @brief  Score
*/
// Created: SBO 2009-04-29
// =============================================================================
class Score : private boost::noncopyable
            , public boost::enable_shared_from_this< Score >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Score( xml::xisubstream xis );
    virtual ~Score();
    //@}

    //! @name Accessors
    //@{
    std::size_t Size() const;
    boost::optional< float > GetValue( std::size_t index ) const;
    //@}

    //! @name Operations
    //@{
    void Update( unsigned int currentTick, const sword::Indicator& message );
    void Send( dispatcher::ClientPublisher_ABC& publisher, int context ) const;
    void Serialize( xml::xostream& xos ) const;
    void Accept( ScoreAnnouncer& visitor );
    //@}

private:
    //! @name Helpers
    //@{
    void ReadProfile( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    std::unique_ptr< xml::xibufferstream > xml_;
    unsigned int beginTick_;
    std::deque< float > values_;
    std::vector< std::string > profiles_;
    //@}
};

}
}

#endif // __Score_h_
