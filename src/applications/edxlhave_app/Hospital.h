// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Hospital_h_
#define __Hospital_h_

#include <boost/noncopyable.hpp>

#ifdef __GNUG__
#   pragma interface
#endif

namespace edxl
{

// =============================================================================
/** @class  Hospital
    @brief  Hospital
*/
// Created: BCI 2011-05-02
// =============================================================================
class Hospital : boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
    Hospital( unsigned int id, const std::string& name );
    virtual ~Hospital();
    //@}

    //! @name Accessors
    //@{
    unsigned int GetId() const;
    const std::string& GetName() const;
    //@}

private:
    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    unsigned int id_;
    std::string name_;
    //@}
};

}

#endif // __Hospital_h_
