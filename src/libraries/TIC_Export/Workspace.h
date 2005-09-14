// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-18 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Export/Workspace.h $
// $Author: Sbo $
// $Modtime: 21/07/05 17:18 $
// $Revision: 24 $
// $Workfile: Workspace.h $
//
// *****************************************************************************

#ifndef __Workspace_h_
#define __Workspace_h_

#include "TIC_Export.h"
#include <string>
#include <vector>

namespace TIC
{
    class TicManager;
}

namespace TICExport
{
    class Automat;
    class CommunicationLink;
    class Callback_ABC;

// =============================================================================
/** @class  Workspace
    @brief  Workspace
    @par    Using example
    @code
    Workspace;
    @endcode
*/
// Created: SBO 2005-05-18
// =============================================================================
class ENGINE_API Workspace
{
public:
    //! @name enum
    //@{
    enum EFormation
    {
        eFormationRoad,
        eFormationLine,
        eFormationDiamond
    };
    //@}

public:
    //! @name Types
    //@{
    typedef std::vector< std::pair< double, double > > T_DisaggregationArea;
    //@}

public:
	//! @name Constructors/Destructor
    //@{
             Workspace( Callback_ABC&      callback, 
                        const std::string& strServer, 
                        unsigned int       nPort, 
                        const std::string& strMainConfigFile,
                        const std::string& strPlatformNatureFile );
    virtual ~Workspace();
    //@}


    //! @name Operations
    //@{
          void              Update                   ();
          unsigned int      GetAutomatCount          () const;
    const Automat           GetAutomatAt             ( unsigned int nIdx ) const;
          unsigned int      GetCommunicationLinkCount() const;
    const CommunicationLink GetCommunicationLinkAt   ( unsigned int nIdx ) const;
    //@}

    //! @name Accessors
    //@{
    const std::string GetSimIsoTime() const;
    //@}

    //! @name Disaggregation management
    //@{
    void AddDisaggregatedArea          ( const T_DisaggregationArea& area );
    void AddDisaggregatedKnowledgeGroup( unsigned int nId );
    void AddDisaggregatedAutomat       ( unsigned int nId );
    void AddDisaggregatedPawn          ( unsigned int nId );

    // platform type blacklist
    void SetDisaggregationFilterFile   ( const std::string& strFile );

    // formation parameters
    void SetDistanceInterPlatform      ( EFormation eFormation, double rDistance );
    //@}
};

} // end namespace TICExport

#endif // __Workspace_h_
