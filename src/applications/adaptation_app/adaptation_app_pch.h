//*****************************************************************************
//
// $Created: JDY 03-06-19 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Pch.h $
// $Author: Ape $
// $Modtime: 15/04/05 15:47 $
// $Revision: 3 $
// $Workfile: ADN_Pch.h $
//
//*****************************************************************************

#ifndef __ADN_APPLICATION_PCH_H_
#define __ADN_APPLICATION_PCH_H_

#include "MT/MT_Tools/MT_Tools_lib.h"
#include "MT/MT_IO/MT_IO_lib.h"
#include "MT/MT_Logger/MT_Logger_lib.h"
#include "MT/MT_Qt/MT_Qt_lib.h"
#include "MT_Tools/MT_Tools_Types.h"

#include "xeumeuleu/xml.h"
#include "ADN_Type_ABC.h"

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
        xistream& operator()( xistream& xis ) const
        {
            T value = value_.GetData();
            xis.attribute( name_, value );
            value_ = value;
            return xis;
        }
        xostream& operator()( xostream& xos ) const
        {
            T value = value_.GetData();
            xos.attribute( name_, value );
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
        xistream& operator()( xistream& xis ) const
        {
            T value;
            xis >> start( tag_ ) >> value >> end();
            value_ = value;
            return xis;
        }
        xostream& operator()( xostream& xos ) const
        {
            return xos << start( tag_ ) << value_.GetData() << end();
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
}

#endif // __ADN_APPLICATION_PCH_H_