// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ArchitectureAttribute_h_
#define __ArchitectureAttribute_h_

#include "UrbanObjectAttribute_ABC.h"

namespace MsgsSimToClient
{
    class MsgUrbanAttributes;
}

namespace dispatcher
{
class Model;

// =============================================================================
/** @class  ArchitectureAttribute
@brief  ArchitectureAttribute
*/
// Created: SLG 2009-12-04
// =============================================================================
class ArchitectureAttribute : public UrbanObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    ArchitectureAttribute( const Model& model, const MsgsSimToClient::MsgUrbanAttributes& message );
    virtual ~ArchitectureAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update   ( const MsgsSimToClient::MsgUrbanAttributes& message );
    virtual void Send     (       MsgsSimToClient::MsgUrbanAttributes& message ) const;
    virtual void AsnDelete(       MsgsSimToClient::MsgUrbanAttributes& message ) const;
    //@}

private:
    //! @name Data members
    //@{
    float           height_;
    unsigned int    floorNumber_;
    unsigned int    basementLevelNumber_;
    std::string     roofShape_;
    std::string     material_;
    float           innerCluttering_;
    float           facadeOpacity_;
    //@}
};

}

#endif // __ArchitectureAttribute_h_
