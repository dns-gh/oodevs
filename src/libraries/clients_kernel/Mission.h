// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Mission_h_
#define __Mission_h_

#include "Resolver.h"

namespace kernel
{
    class FragOrder;

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
             Mission( const QString& name, unsigned int id, bool automat );
    virtual ~Mission();
    //@}

    //! @name Accessors
    //@{
    unsigned long GetId() const;
    QString GetName() const;
    bool IsAutomat() const;
    //@}

private:
    //! @name Member data
    //@{
    QString name_;
    unsigned int id_;
    bool automat_;
    //@}
};

}

#endif // __Mission_h_
