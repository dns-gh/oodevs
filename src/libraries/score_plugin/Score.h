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
    class ClientAnnouncer_ABC;

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
    float GetValue( std::size_t index ) const;
    //@}

    //! @name Operations
    //@{
    void Update( const sword::Indicator& message );
    void Send( dispatcher::ClientPublisher_ABC& publisher, int context ) const;
    void Serialize( xml::xostream& xos ) const;
    void Accept( ClientAnnouncer_ABC& visitor );
    //@}

private:
    //! @name Helpers
    //@{
    void ReadProfile( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< xml::xibufferstream > xml_;
    std::deque< float > values_;
    std::vector< std::string > profiles_;
    //@}
};

}
}

#endif // __Score_h_
