// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2004-07-01 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_XmlInput_Helper.h $
// $Author: Ape $
// $Modtime: 22/04/05 15:36 $
// $Revision: 8 $
// $Workfile: ADN_XmlInput_Helper.h $
//
// *****************************************************************************

#ifndef __ADN_XmlInput_Helper_h_
#define __ADN_XmlInput_Helper_h_

#include "MT/MT_Archive/MT_InputArchive_ABC.h"
#include "MT_Tools/MT_InputArchive_Logger.h"

#include "ADN_Types.h"

// =============================================================================
/** @class  ADN_XmlInput_Helper
    @brief  ADN_XmlInput_Helper
    @par    Using example
    @code
    ADN_XmlInput_Helper;
    @endcode
*/
// Created: AGN 2004-07-01
// =============================================================================
class ADN_XmlInput_Helper : public MT_InputArchive_Logger<MT_XXmlInputArchive>
{
typedef MT_InputArchive_Logger<MT_XXmlInputArchive> Base;
public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_XmlInput_Helper();
    virtual ~ADN_XmlInput_Helper();
    //@}

    //! @name Operations
    //@{
    template< class T >
        bool ReadAttribute( const std::string& name, T& value, ErrorPolicyType nOptional = eThrow )
    {
        return Base::ReadAttribute( name, value, nOptional );
    }

    template< class T, int NB >
    bool ReadAttribute( const std::string& name, ADN_Type_Enum<T,NB>& value, T ConvertFct( const std::string& ), ErrorPolicyType nOptional )
    {
        std::string strTmp;
        bool bResult = this->ReadAttribute( name, strTmp, nOptional );
        if( ! bResult )
            return false;
        
        T nValue = ConvertFct( strTmp );
        if( nValue == (T)-1 )
            ThrowError( "La valeur " + strTmp + " n'est pas reconnue." );
        
        value = nValue;
        return true;
    }



    template< class T >
    bool ReadField( const std::string& name, T& value, ErrorPolicyType nOptional = eThrow )
    {
        return Base::ReadField( name, value, nOptional );
    }
    
    /*
    bool ReadAttribute( const std::string& name, ADN_Type_Bool& value, ErrorPolicyType nOptional )
    {
        bool bTmp;
        bool bResult = this->ReadAttribute( name, bTmp, nOptional );
        if( bResult )
            value = bTmp;
        return bResult;
    }


    bool ReadAttribute( const std::string& name, ADN_Type_Int& value, ErrorPolicyType nOptional )
    {
        int nTmp;
        bool bResult = Base::ReadAttribute( name, nTmp, nOptional );
        if( bResult )
            value = nTmp;
        return bResult;
    }


    bool ReadAttribute( const std::string& name, ADN_Type_Double& value, ErrorPolicyType nOptional )
    {
        double rTmp;
        bool bResult = Base::ReadAttribute( name, rTmp, nOptional );
        if( bResult )
            value = rTmp;
        return bResult;
    }


    bool ReadAttribute( const std::string& name, ADN_Type_String& value, ErrorPolicyType nOptional )
    {
        std::string strTmp;
        bool bResult = Base::ReadAttribute( name, strTmp, nOptional );
        if( bResult )
            value = strTmp;
        return bResult;
    }
*/
    template< class T, int NB >
    bool ReadField( const std::string& name, ADN_Type_Enum<T,NB>& value, T ConvertFct( const std::string& ), ErrorPolicyType nOptional )
    {
        std::string strTmp;
        bool bResult = Base::ReadField( name, strTmp, nOptional );
        if( ! bResult )
            return false;

        T nValue = ConvertFct( strTmp );
        if( nValue == (T)-1 )
            ThrowError( "La valeur " + strTmp + " n'est pas reconnue." );

        value = nValue;
        return true;
    }

/*
    bool ReadField( const std::string& name, ADN_Type_Bool& value, ErrorPolicyType nOptional )
    {
        bool bTmp;
        bool bResult = Base::ReadField( name, bTmp, nOptional );
        if( bResult )
            value = bTmp;
        return bResult;
    }


    bool ReadField( const std::string& name, ADN_Type_Int& value, ErrorPolicyType nOptional )
    {
        int nTmp;
        bool bResult = Base::ReadField( name, nTmp, nOptional );
        if( bResult )
            value = nTmp;
        return bResult;
    }


    bool ReadField( const std::string& name, ADN_Type_Double& value, ErrorPolicyType nOptional )
    {
        double rTmp;
        bool bResult = Base::ReadField( name, rTmp, nOptional );
        if( bResult )
            value = rTmp;
        return bResult;
    }


    bool ReadField( const std::string& name, ADN_Type_String& value, ErrorPolicyType nOptional )
    {
        std::string strTmp;
        bool bResult = Base::ReadField( name, strTmp, nOptional );
        if( bResult )
            value = strTmp;
        return bResult;
    }

*/
    bool ReadTimeAttribute( const std::string& name, ADN_Type_Double& value, ErrorPolicyType nOptional = eThrow )
    {
        double rTmp;
        bool bResult = Base::ReadTimeAttribute( name, rTmp, nOptional );
        if( bResult )
            value = rTmp;
        return bResult;
    }


    bool ReadTimeField( const std::string& name, ADN_Type_Double& value, ErrorPolicyType nOptional = eThrow )
    {
        double rTmp;
        bool bResult = Base::ReadTimeField( name, rTmp, nOptional );
        if( bResult )
            value = rTmp;
        return bResult;
    }


    bool ReadTime( ADN_Type_Double& value, ErrorPolicyType nOptional = eThrow )
    {
        double rTmp;
        bool bResult = Base::ReadTime( rTmp, nOptional );
        if( bResult )
            value = rTmp;
        return bResult;
    }

    void ThrowError( const std::string& strMessage ) const;
    //@}



private:
    //! @name Copy/Assignement
    //@{
    ADN_XmlInput_Helper( const ADN_XmlInput_Helper& );            //!< Copy constructor
    ADN_XmlInput_Helper& operator=( const ADN_XmlInput_Helper& ); //!< Assignement operator
    //@}
};


#endif // __ADN_XmlInput_Helper_h_
