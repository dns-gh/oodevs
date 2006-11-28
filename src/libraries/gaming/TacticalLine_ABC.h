// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TacticalLine_ABC_h_
#define __TacticalLine_ABC_h_

#include "clients_kernel/Types.h"
#include "clients_kernel/TacticalLine_ABC.h"
#include "clients_kernel/Serializable_ABC.h"
#include "clients_kernel/Drawable_ABC.h"
#include "ASN_Types.h"

namespace kernel
{
    class GlTools_ABC;
}

namespace xml
{
    class xostream;
}

class Publisher_ABC;

// =============================================================================
/** @class  TacticalLine_ABC
    @brief  Base class of the limits and limas.
*/
// Created: APE 2004-04-14
// =============================================================================
class TacticalLine_ABC : public kernel::TacticalLine_ABC
                       , public kernel::Extension_ABC
                       , public kernel::Drawable_ABC
                       , public kernel::Updatable_ABC< ASN1T_MsgLimaUpdate >
                       , public kernel::Updatable_ABC< ASN1T_MsgLimitUpdate >
{

public:
    //! @name Constructors/Destructor
    //@{
             TacticalLine_ABC( const QString& baseName, unsigned long id, Publisher_ABC& publisher );
    virtual ~TacticalLine_ABC();
    //@}

    //! @name Operations
    //@{
    void Delete();
    void Create();

    virtual void Serialize( xml::xostream& xos ) const;
    virtual void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

    //! @name Accessors
    //@{
    virtual unsigned long GetId  () const;
    virtual QString       GetName() const;
    //@}

protected:
    //! @name Types
    //@{
    enum E_State
    {   
        eStateOk            = 0x00,
        eStateCreated       = 0x01,
        eStateModified      = 0x02,
        eStateDeleted       = 0x04
    };
    //@}

    //! @name Helpers
    //@{
    void WriteGeometry( ASN1T_Line& line ) const;
    void WriteDiffusion( ASN1T_TacticalLinesDiffusion& diffusion ) const;
    virtual void UpdateToSim( E_State state ) = 0;
    template< typename Message >
    void Send( Message& message )
    {
        message.Send( publisher_, (unsigned long)this );
    }

    virtual void DoUpdate( const ASN1T_MsgLimaUpdate& message );
    virtual void DoUpdate( const ASN1T_MsgLimitUpdate& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TacticalLine_ABC( const TacticalLine_ABC& );
    TacticalLine_ABC& operator=( const TacticalLine_ABC& );
    //@}

private:
    //! @name Member data
    //@{
    Publisher_ABC& publisher_;
    unsigned long  id_;
    QString        name_;
    //@}
};

#endif // __TacticalLine_ABC_h_
