// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __ADN_XmlStreamOperators_h_
#define __ADN_XmlStreamOperators_h_

#include <xeumeuleu/xml.hpp>
#include "ADN_Type_ABC.h"
#include "ADN_TypePtr_InVector_ABC.h"
#include "ADN_Type_LocalizedString.h"

namespace xml
{
    template< typename T >
    class attribute_manipulator< ADN_Type_ABC< T > >
    {
    public:
        //! @name Constructors/Destructor
        //@{
        attribute_manipulator( const std::string& name, ADN_Type_ABC< T >& value )
            : name_ ( name )
            , value_( value )
        {}
        //@}

        //! @name Operators
        //@{
        friend xistream& operator>>( xml::xistream& xis, const attribute_manipulator& m )
        {
            T value = m.value_.GetData();
            xis >> xml::attribute( m.name_, value );
            m.value_ = value;
            return xis;
        }
        friend xostream& operator<<( xostream& xos, const attribute_manipulator& m )
        {
            return xos << xml::attribute( m.name_, m.value_.GetData() );
        }
        //@}

    private:
        //! @name Copy/Assignment
        //@{
        attribute_manipulator& operator=( const attribute_manipulator& ); //!< Assignment operator
        //@}

    private:
        //! @name Member data
        //@{
        std::string name_;
        ADN_Type_ABC< T >& value_;
        //@}
    };

    template< typename T >
    class content_manipulator< ADN_Type_ABC< T > >
    {
    public:
        //! @name Constructors/Destructor
        //@{
        content_manipulator( const std::string& tag, ADN_Type_ABC< T >& value )
            : tag_  ( tag )
            , value_( value )
        {}
        //@}

        //! @name Operators
        //@{
        friend xistream& operator>>( xml::xistream& xis, const content_manipulator& m )
        {
            T value;
            xis >> xml::start( m.tag_ ) >> value >> xml::end;
            m.value_ = value;
            return xis;
        }
        friend xostream& operator<<( xostream& xos, const content_manipulator& m )
        {
            return xos << xml::start( m.tag_ ) << m.value_.GetData() << xml::end;
        }
        //@}

    private:
        //! @name Copy/Assignment
        //@{
        content_manipulator& operator=( const content_manipulator& ); //!< Assignment operator
        //@}

    private:
        //! @name Member data
        //@{
        std::string tag_;
        ADN_Type_ABC< T >& value_;
        //@}
    };

    template< typename T >
    class attribute_manipulator< ADN_TypePtr_InVector_ABC< T > >
    {
    public:
        //! @name Constructors/Destructor
        //@{
        attribute_manipulator( const std::string& name, ADN_TypePtr_InVector_ABC< T >& value )
            : name_ ( name )
            , value_( value )
        {}
        //@}

        //! @name Operators
        //@{
        friend xistream& operator>>( xml::xistream& xis, const attribute_manipulator& m )
        {
            m.value_.SetRefName( xis.attribute< std::string >( m.name_ ) );
            m.value_.Initialize();
            return xis;
        }
        friend xostream& operator<<( xostream& xos, const attribute_manipulator& m )
        {
            T* value = m.value_.GetData();
            if( value )
                xos << xml::attribute( m.name_, value->strName_ );
            else
                xos << xml::attribute( m.name_, "" );
            return xos;
        }
        //@}

    private:
        //! @name Copy/Assignment
        //@{
        attribute_manipulator& operator=( const attribute_manipulator& ); //!< Assignment operator
        //@}

    private:
        //! @name Member data
        //@{
        std::string name_;
        ADN_TypePtr_InVector_ABC< T >& value_;
        //@}
    };

    template<>
    class attribute_manipulator< ADN_Type_LocalizedString >
    {
    public:
        //! @name Constructors/Destructor
        //@{
        attribute_manipulator( const std::string& name, ADN_Type_LocalizedString& value )
            : name_ ( name )
            , value_( value )
        {}
        //@}

        //! @name Operators
        //@{
        friend xistream& operator>>( xml::xistream& xis, const attribute_manipulator& m )
        {
            std::string value;
            xis >> xml::attribute( m.name_, value );
            m.value_ = value;
            return xis;
        }
        friend xostream& operator<<( xostream& xos, const attribute_manipulator& m )
        {
            return xos << xml::attribute( m.name_, m.value_.GetKey() );
        }
        //@}

    private:
        //! @name Copy/Assignment
        //@{
        attribute_manipulator& operator=( const attribute_manipulator& ); //!< Assignment operator
        //@}

    private:
        //! @name Member data
        //@{
        std::string name_;
        ADN_Type_LocalizedString& value_;
        //@}
    };

    template<>
    class content_manipulator< ADN_Type_LocalizedString >
    {
    public:
        //! @name Constructors/Destructor
        //@{
        content_manipulator( const std::string& tag, ADN_Type_LocalizedString& value )
            : tag_  ( tag )
            , value_( value )
        {}
        //@}

        //! @name Operators
        //@{
        friend xistream& operator>>( xml::xistream& xis, const content_manipulator& m )
        {
            std::string value;
            xis >> xml::start( m.tag_ ) >> value >> xml::end;
            m.value_ = value;
            return xis;
        }
        friend xostream& operator<<( xostream& xos, const content_manipulator& m )
        {
            return xos << xml::start( m.tag_ ) << m.value_.GetKey() << xml::end;
        }
        //@}

    private:
        //! @name Copy/Assignment
        //@{
        content_manipulator& operator=( const content_manipulator& ); //!< Assignment operator
        //@}

    private:
        //! @name Member data
        //@{
        std::string tag_;
        ADN_Type_LocalizedString& value_;
        //@}
    };
}

#endif // __ADN_XmlStreamOperators_h_
