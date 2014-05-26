// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef _Affinities_ABC_h
#define _Affinities_ABC_h

#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Serializable_ABC.h"
#include "clients_kernel/SubTypes.h"
#include <boost/noncopyable.hpp>

namespace xml
{
    class xistream;
}

class Model;

// =============================================================================
/** @class  Affinities
    @brief  Affinities declaration
*/
// Created: LGY 2011-03-17
// =============================================================================
class Affinities : public kernel::Extension_ABC
                 , public kernel::Serializable_ABC
                 , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Affinities( Model& model );
    explicit Affinities( xml::xistream& xis, Model& model );
    virtual ~Affinities();
    //@}

public:
    //! @name Types
    //@{
    typedef std::map< unsigned long, float > T_Affinities;
    //@}

    //! @name Operations
    //@{
    virtual void Clear() = 0;
    virtual void Add( unsigned long team, float value );
    virtual void Fill( std::map< unsigned long, float >& content );
    virtual bool HasAffinities() const;
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void ReadAffinity( xml::xistream& xis );
    //@}

protected:
    //! @name Member data
    //@{
    T_Affinities affinities_;
    Model& model_;
    //@}
};

#endif // _Affinities_ABC_h
