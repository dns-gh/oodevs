// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef aar_PowerIndicator_h
#define aar_PowerIndicator_h

#include <string>
#include <boost/noncopyable.hpp>

namespace xml 
{ 
    class xistream; 
}

namespace aar
{

// =============================================================================
/** @class  PowerIndicator
    @brief  Power indicator
*/
// Created: ABR 2011-02-09
// =============================================================================
class PowerIndicator : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit PowerIndicator( xml::xistream& xis );
    virtual ~PowerIndicator();
    //@}

    //! @name Operations
    //@{
    unsigned long GetId() const;
    const std::string& GetName() const;

    unsigned int GetDirectFire() const;
    unsigned int GetIndirectFire() const;
    unsigned int GetCloseCombat() const;
    unsigned int GetEngineering() const;
    //@}

private:
    //! @name Member data
    //@{
    std::string name_;
    unsigned long id_;

    unsigned int nDirectFire_;
    unsigned int nIndirectFire_;
    unsigned int nCloseCombat_;
    unsigned int nEngineering_;
    //@}
};

} // namespace aar

#endif // aar_PowerIndicator_h
