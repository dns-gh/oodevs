// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterObstacleType_h_
#define __ActionParameterObstacleType_h_

#include "Parameter.h"
#include <boost/function.hpp>

namespace sword
{
    enum ObstacleType_DemolitionTargetType;
}

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  ObstacleType
    @brief  ObstacleType
*/
// Created: SBO 2007-05-25
// =============================================================================
class ObstacleType : public Parameter< QString >
{
public:
    //! @name Functors
    //@{

    typedef boost::function< void ( const sword::ObstacleType_DemolitionTargetType& ) > T_Setter;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             ObstacleType( const kernel::OrderParameter& parameter, unsigned int value );
    virtual ~ObstacleType();
    //@}

    //! @name Operations
    //@{
    void CommitTo( T_Setter setter ) const;
    virtual void Accept( ParameterVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ObstacleType( const ObstacleType& );            //!< Copy constructor
    ObstacleType& operator=( const ObstacleType& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual std::string SerializeType() const;
    virtual void Serialize( xml::xostream& xos ) const;
    //@}

private:
    //! @name Member data
    //@{
    sword::ObstacleType_DemolitionTargetType value_;
    //@}
};

    }
}

#endif // __ActionParameterObstacleType_h_
