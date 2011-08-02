// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __Primitive_h_
#define __Primitive_h_

#include <boost/shared_ptr.hpp>
#pragma warning( push, 0 )
#pragma warning( push, 0 )
#include <QtCore/qstring.h>
#pragma warning( pop )
#pragma warning( pop )

namespace kernel
{
    class Displayer_ABC;
}

namespace xml
{
    class xistream;
    class xostream;
}

namespace indicators
{
    class DataTypeFactory;
    class Element_ABC;
    class PrimitiveParameter;

// =============================================================================
/** @class  Primitive
    @brief  Primitive
*/
// Created: SBO 2009-04-06
// =============================================================================
class Primitive
{
public:
    //! @name Constructors/Destructor
    //@{
             Primitive( xml::xistream& xis, const DataTypeFactory& types );
    virtual ~Primitive();
    //@}

    //! @name Accessors
    //@{
    QString GetName() const;
    std::string GetCategory() const;
    QString GetPrototype() const;
    //@}

    //! @name Operations
    //@{
    boost::shared_ptr< Element_ABC > Instanciate( const std::string& input ) const;
    void DisplayInTooltip( kernel::Displayer_ABC& displayer ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Primitive( const Primitive& );            //!< Copy constructor
    Primitive& operator=( const Primitive& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadParameter( xml::xistream& xis );
    QString BuildParameterList() const;
    //@}

    //! @name Types
    //@{
    typedef std::vector< const PrimitiveParameter* > T_Parameters;
    //@}

private:
    //! @name Member data
    //@{
    const QString name_;
    const std::string category_;
    const QString comment_;
    const std::string type_;
    const DataTypeFactory& types_;
    QString prototype_;
    T_Parameters parameters_;
    //@}
};

}

#endif // __Primitive_h_
