// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __SuccessFactor_h_
#define __SuccessFactor_h_

#include <boost/noncopyable.hpp>

namespace kernel
{
    class Controller;
    class Controllers;
}

namespace xml
{
    class xistream;
    class xostream;
}

class ProfilesModel;
class ProfileSelection;
class ScoresModel;
class SuccessFactorActions;
class SuccessFactorActionTypes;
class SuccessFactorConditions;

// =============================================================================
/** @class  SuccessFactor
    @brief  SuccessFactor
*/
// Created: SBO 2009-06-15
// =============================================================================
class SuccessFactor : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             SuccessFactor( const QString& name, kernel::Controllers& controllers );
             SuccessFactor( xml::xistream& xis, kernel::Controllers& controllers, const ProfilesModel& model, const ScoresModel& scores, const SuccessFactorActionTypes& actionTypes );
    virtual ~SuccessFactor();
    //@}

    //! @name Accessors
    //@{
    QString GetName() const;
    ProfileSelection& GetProfiles() const;
    SuccessFactorConditions& GetConditions() const;
    SuccessFactorActions& GetActions() const;
    //@}

    //! @name Operations
    //@{
    void SetName( const QString& name );
    void Serialize( xml::xostream& xos ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    QString name_;
    std::unique_ptr< ProfileSelection > profiles_;
    std::unique_ptr< SuccessFactorConditions > conditions_;
    std::unique_ptr< SuccessFactorActions > actions_;
    //@}
};

#endif // __SuccessFactor_h_
