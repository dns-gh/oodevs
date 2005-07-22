// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-19 $
// $Archive: /MVW_v10/Build/SDK/TIC_XmlExporter/src/OutputXml.h $
// $Author: Sbo $
// $Modtime: 12/07/05 9:40 $
// $Revision: 11 $
// $Workfile: OutputXml.h $
//
// *****************************************************************************

#ifndef __OutputXml_h_
#define __OutputXml_h_

#include "Types.h"
#include "Callback_ABC.h"

class MT_XXmlOutputArchive;

namespace TICExport
{
    class Config;
    class Workspace;
    class Automat;
    class Pawn;
    class Platform;
    class Communication;
    class CommunicationLink;

// =============================================================================
/** @class  OutputXml
    @brief  OutputXml
    @par    Using example
    @code
    OutputXml;
    @endcode
*/
// Created: SBO 2005-05-19
// =============================================================================
class OutputXml : public Callback_ABC
{
public:
	//! @name Constructors/Destructor
    //@{
             OutputXml( const Config& config );
    virtual ~OutputXml();
    //@}

    //! @name Init
    //@{
    void SetWorkspace( const Workspace& workspace );
    //@}

private:
    //! @name Copy constructor
    //@{
    OutputXml( const OutputXml& outputXml );
    OutputXml& operator=( const OutputXml& outputXml );
    //@}

    //! @name Callbacks
    //@{
    virtual void OnReceiveEndTick();
    //@}

    //! @name Operations
    //@{
    void Save();
    //@}

    //! @name Tools
    //@{
    void OutputAutomat          ( const Automat&           automat       );
    void OutputPawn             ( const Pawn&              pawn          );

    void OutputPlatform         ( const Platform&          platform      );
    void OutputCommunication    ( const Communication&     communication );
    void OutputCommunicationLink( const CommunicationLink& link          );
    //@}

    //! @name Internal functions
    //@{
    const std::string GetNextIncrementalFilename( std::string& strCurrentTime );
    //@}

private:
	//! @name Member data
    //@{
    MT_XXmlOutputArchive&    archive_;
    std::string              strFilename_;
    std::string              strDirectory_;

    bool                     bIsIncremental_;
    bool                     bExportCommunications_;

    const Workspace*         pWorkspace_;
	//@}
};

} // end namespace TICExport


#endif // __OutputXml_h_
