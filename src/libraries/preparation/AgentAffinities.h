// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __AgentAffinities_h_
#define __AgentAffinities_h_

#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Serializable_ABC.h"
#include "tools/ElementObserver_ABC.h"
#include "Types.h"
#include <map>

namespace kernel
{
    class Controllers;
}

namespace xml
{
    class xistream;
}


// =============================================================================
/** @class  AgentAffinities
    @brief  Agent affinities
*/
// Created: LGY 2011-03-14
// =============================================================================
class AgentAffinities : public kernel::Extension_ABC
                      , public kernel::Serializable_ABC
                      , public tools::Observer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit AgentAffinities( kernel::Controllers& controllers );
             AgentAffinities( xml::xistream& xis, kernel::Controllers& controllers );
    virtual ~AgentAffinities();
    //@}

    //! @name Operations
    //@{
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    void Clear();
    void Add( unsigned long team, float value );
    void Fill( std::map< unsigned long, float >& content );
    bool HasAffinities() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AgentAffinities( const AgentAffinities& );            //!< Copy constructor
    AgentAffinities& operator=( const AgentAffinities& ); //!< Assignment operator
    //@}

private:
    //! @name Helpers
    //@{
    void ReadAffinity( xml::xistream& xis );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< unsigned long, EntityAffinity > T_Affinities;
    typedef T_Affinities::const_iterator            CIT_Affinities;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    T_Affinities affinities_;
    //@}
};

#endif // __AgentAffinities_h_
