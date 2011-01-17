// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef dispatcher_UsagesAttribute_h
#define dispatcher_UsagesAttribute_h

#include "UrbanObjectAttribute_ABC.h"
#include <map>

namespace dispatcher
{
// =============================================================================
/** @class  UsagesAttribute
    @brief  Usages attribute
*/
// Created: LGY 2011-01-17
// =============================================================================
class UsagesAttribute : public UrbanObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit UsagesAttribute( const sword::UrbanAttributes& message );
    virtual ~UsagesAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update( const sword::UrbanAttributes& message );
    virtual void Send( sword::UrbanAttributes& message ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, unsigned int > T_Motivations;
    //@}
private:
    //! @name Data member
    //@{
    T_Motivations motivations_;
    //@}
};

}

#endif // dispatcher_UsagesAttribute_h
