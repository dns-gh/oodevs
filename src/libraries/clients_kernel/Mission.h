// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Mission_h_
#define __Mission_h_

#include "Resolver.h"

namespace kernel
{
    class FragOrder;
    class MissionType;

// =============================================================================
/** @class  Mission
    @brief  Mission
*/
// Created: AGE 2006-03-14
// =============================================================================
class Mission : public Resolver< FragOrder >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Mission( const MissionType& type );
    virtual ~Mission();
    //@}

    //! @name Operations
    //@{
    unsigned int GetId() const;
    std::string GetName() const;
    const MissionType& GetType() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Mission( const Mission& );            //!< Copy constructor
    Mission& operator=( const Mission& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const MissionType& type_;
    //@}
};

}

#endif // __Mission_h_
