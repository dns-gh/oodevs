// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-17 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Types/AutomatType.h $
// $Author: Sbo $
// $Modtime: 7/06/05 9:58 $
// $Revision: 1 $
// $Workfile: AutomatType.h $
//
// *****************************************************************************

#ifndef __AutomatType_h_
#define __AutomatType_h_

#include "Types.h"
#include "Messages/ASN_Messages.h"

namespace TEST
{
    class PawnType;
    class AutomatModel;

// =============================================================================
/** @class  AutomatType
    @brief  AutomatType
    @par    Using example
    @code
    AutomatType;
    @endcode
*/
// Created: SBO 2005-05-11
// =============================================================================
class AutomatType
{
    MT_COPYNOTALLOWED( AutomatType );

public:
    //! @name Manager
    //@{
    static void               Initialize( const std::string& strScipioAutomatConfigFile );
    static void               Terminate ();

    static const AutomatType* Find      ( const std::string& strName );
    static const AutomatType* Find      ( ASN1T_TypeAutomate asnId );
    //@}

    //! @name Accessors
    //@{
    const std::string&  GetName  () const;
          uint          GetId    () const;
    const PawnType&     GetPcType() const;
    const AutomatModel& GetModel () const;
    //@}

private:
    //! @name Constructors/Destructor
    //@{
             AutomatType( const std::string& strName, XmlInputArchive& archive );
    virtual ~AutomatType();
    //@}

    //! @name Types
    //@{
    typedef std::map< std::string, const AutomatType*, sCaseInsensitiveLess > T_AutomatTypeMap;
    typedef T_AutomatTypeMap::const_iterator                                  CIT_AutomatTypeMap;
    //@}

private:
    //! @name Member data
    //@{
    std::string             strName_;
	uint                    nId_;
    const PawnType*         pPcPawnType_;
    const AutomatModel*     pModel_;

    // global type list
    static T_AutomatTypeMap automatTypes_;
    //@}
};

} // end namespace TEST

#include "AutomatType.inl"

#endif // __AutomatType_h_
