// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-11 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Communications/Communication_Intervisibility.h $
// $Author: Sbo $
// $Modtime: 12/07/05 18:45 $
// $Revision: 6 $
// $Workfile: Communication_Intervisibility.h $
//
// *****************************************************************************

#ifndef __Communication_Intervisibility_h_
#define __Communication_Intervisibility_h_

#include "Types.h"
#include "Messages/ASN_Messages.h"
#include "Communication_ABC.h"

namespace DIN
{
    class DIN_Input;
}

namespace TIC
{
    class Pawn;
    class CommunicationManager;
    class CommunicationLink;

// =============================================================================
/** @class  Communication_Intervisibility
    @brief  Communication_Intervisibility
    @par    Using example
    @code
    Communication_Intervisibility;
    @endcode
*/
// Created: SBO 2005-05-24
// =============================================================================
class Communication_Intervisibility : public Communication_ABC
{

public:
    //! @name Message handlers
    //@{
    static void OnReceiveUnit  ( DIN::DIN_Input& input );
    static void OnReceiveObject( DIN::DIN_Input& input );
    //@}

    //! @name Accessors
    //@{
          uint         GetArity     () const;
    const std::string  GetSubject   () const;
    const std::string& GetMessage   () const;
          bool         IsSameSubject( T_EntityId nSubjectId ) const;
    //@}

    //! @name Modifiers
    //@{
    bool  ResetStatus();
    //@}

private:
    //! @name Enums
    //@{
    enum E_UnitVisType
    {
        eVisTypeInvisible  = 0,
        eVisTypeDetected   = 1,
        eVisTypeRecognized = 2,
        eVisTypeIdentified = 3
    };
    //@}

private:
    //! @name Constructors/Destructor
    //@{
    explicit Communication_Intervisibility( CommunicationLink&            link, 
                                            T_EntityId                    nSubjectId, 
                                            const CommunicationType&      type, 
                                            CommunicationManager::ESource eSource );
    virtual ~Communication_Intervisibility();
    //@}

    //! @name Static Operations
    //@{
    static Communication_Intervisibility* GetCommunicationBySubjectId( CommunicationLink&            link, 
                                                                       CommunicationManager::ESource eSource,
                                                                       T_EntityId                    nSubjectId );

    static Communication_Intervisibility* GetCommunicationByType     ( CommunicationLink&            link,
                                                                       CommunicationManager::ESource eSource,
                                                                       const CommunicationType&      type );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< T_EntityId, bool >      T_EntityIdMap;
    typedef T_EntityIdMap::const_iterator     CIT_EntityIdMap;
    typedef T_EntityIdMap::iterator           IT_EntityIdMap;
    //@}

private:
    //! @name Member data
    //@{
    T_EntityIdMap  subjects_;
    //@}
};

} // end namespace TIC

#include "Communication_Intervisibility.inl"

#endif // __Automat_h_
