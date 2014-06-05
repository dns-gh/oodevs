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

#include "Score_ABC.h"

namespace kernel
{
    class Controller;
    class Controllers;
}

namespace indicators
{
    class Gauge;
    class GaugeFactory_ABC;
    class Primitives;
    class Variables;
}

namespace xml
{
    class xistream;
}

class ProfilesModel;

// =============================================================================
/** @class  Score
    @brief  Score
*/
// Created: SBO 2009-04-16
// =============================================================================
class Score : public Score_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Score( const Score& );
             Score( xml::xistream& xis, kernel::Controllers& controllers, const indicators::Primitives& indicators, const indicators::GaugeFactory_ABC& gaugeFactory, const ProfilesModel& profilesModel );
             Score( const QString& name, kernel::Controllers& controllers, const indicators::Primitives& indicators, const indicators::GaugeFactory_ABC& gaugeFactory );
    virtual ~Score();
    //@}

    //! @name Accessors
    //@{
    virtual QString GetName() const;
    virtual QString GetFormula() const;
    virtual const indicators::Gauge& GetGauge() const;
    virtual const indicators::Variables& GetVariables() const;
    virtual const ProfileSelection& GetProfiles() const;
    //@}

    //! @name Setters
    //@{
    void SetName( const QString& name );
    void SetFormula( const QString& formula );
    void SetGauge( const indicators::Gauge& gauge );
    void SetVariables( const indicators::Variables& variables );
    void SetProfiles( std::unique_ptr< ProfileSelection > profiles );
    //@}

    //! @name Operations
    //@{
    virtual void CheckValidity() const;
    virtual void Serialize( xml::xostream& xos ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Score& operator=( const Score& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void SerializeIndicators( xml::xostream& xos ) const;
    void SerializeIndicator( xml::xostream& xos, const QString& formula ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller* controller_;
    const indicators::Primitives& indicators_;
    QString name_;
    QString formula_;
    std::unique_ptr< indicators::Gauge > gauge_;
    std::unique_ptr< indicators::Variables > variables_;
    std::unique_ptr< ProfileSelection > profiles_;
    //@}
};

#endif // __Score_h_
