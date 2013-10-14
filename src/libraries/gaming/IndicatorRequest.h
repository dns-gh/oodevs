// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __IndicatorRequest_h_
#define __IndicatorRequest_h_

#include <boost/noncopyable.hpp>

namespace kernel
{
    class Controller;
}

namespace sword
{
    class Indicator;
    class PlotResult;
}

class IndicatorDefinition_ABC;
class Publisher_ABC;

// =============================================================================
/** @class  IndicatorRequest
    @brief  IndicatorRequest
*/
// Created: AGE 2007-09-25
// =============================================================================
class IndicatorRequest : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef std::vector< double > T_Data;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    IndicatorRequest( kernel::Controller& controller, const IndicatorDefinition_ABC& definition, Publisher_ABC& publisher, const QString& displayName );
    IndicatorRequest( xml::xistream& xis, kernel::Controller& controller, const IndicatorDefinition_ABC& definition, Publisher_ABC& publisher );
    virtual ~IndicatorRequest();
    //@}

    //! @name Operations
    //@{
    void SetParameter( const std::string& name, const std::string& value );
    void SetTimeRange( unsigned int firstTick, unsigned int duration );
    void Commit() const;
    void Save( xml::xostream& xos ) const;

    QString GetName() const;
    QString GetDisplayName() const;
    unsigned int GetFirstTick() const;
    void Update( const sword::PlotResult& message );
    void Update( const sword::Indicator& message );
    bool IsPending() const;
    bool IsDone() const;
    bool IsFailed() const;

    QString ErrorMessage() const;
    const T_Data& Result() const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, std::string > T_Parameters;
    //@}

    //! @name Helpers
    //@{
    void ReadParameter( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const IndicatorDefinition_ABC& definition_;
    Publisher_ABC& publisher_;
    const QString displayName_;
    T_Parameters parameters_;
    bool done_;
    unsigned int firstTick_;
    unsigned int duration_;
    T_Data result_;
    T_Data newValues_;
    std::string error_;
    //@}
};

#endif // __IndicatorRequest_h_
