// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-11 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifndef __TypeManager_h_
#define __TypeManager_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Types.h"
#include "Messages/ASN_Messages.h"

namespace TEST {

    class AutomatType;
    class PawnType;
    class PawnModel;
    class AutomatModel;

// =============================================================================
/** @class  TypeManager
    @brief  TypeManager
    @par    Using example
    @code
    TypeManager;
    @endcode
*/
// Created: SBO 2005-08-11
// =============================================================================
class TypeManager
{

public:
    //! @name Constructors/Destructor
    //@{
             TypeManager( const std::string& strModelFile, const std::string& strPawnTypeFile, const std::string& strAutomatTypeFile );
    virtual ~TypeManager();
    //@}

    //! @name Operations
    //@{
    const AutomatType*  FindAutomatType ( const std::string& strName  ) const;
    const AutomatType*  FindAutomatType ( ASN1T_TypeAutomate asnId    ) const;
    const AutomatModel* FindAutomatModel( const std::string& strModel ) const;
    const PawnType*     FindPawnType    ( const std::string& strName  ) const;
    const PawnType*     FindPawnType    ( ASN1T_TypePion     asnId    ) const;
    const PawnModel*    FindPawnModel   ( const std::string& strModel ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TypeManager( const TypeManager& );            //!< Copy constructor
    TypeManager& operator=( const TypeManager& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void LoadAutomatTypes ( const std::string& strConfigFile );
    void LoadPawnTypes    ( const std::string& strConfigFile );
    void LoadModels       ( const std::string& strConfigFile );
    void LoadAutomatModels( const std::string& strConfigFile );
    void LoadPawnModels   ( const std::string& strConfigFile );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, const PawnType*, sCaseInsensitiveLess > T_PawnTypeMap;
    typedef T_PawnTypeMap::const_iterator                                  CIT_PawnTypeMap;

    typedef std::map< std::string, const AutomatType*, sCaseInsensitiveLess > T_AutomatTypeMap;
    typedef T_AutomatTypeMap::const_iterator                                  CIT_AutomatTypeMap;

    typedef std::map< std::string, PawnModel* >     T_PawnModelMap;
    typedef T_PawnModelMap::const_iterator          CIT_PawnModelMap;

    typedef std::map< std::string, AutomatModel* >  T_AutomatModelMap;
    typedef T_AutomatModelMap::const_iterator       CIT_AutomatModelMap;
    //@}

private:
    //! @name Member data
    //@{
    T_PawnTypeMap       pawnTypes_;
    T_PawnModelMap      pawnModels_;
    T_AutomatTypeMap    automatTypes_;
    T_AutomatModelMap   automatModels_;
    //@}
};

} // end namespace TEST

#endif // __TypeManager_h_
