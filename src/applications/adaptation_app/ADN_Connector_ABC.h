//*****************************************************************************
//
// $Created: JDY 03-07-01 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Connector_ABC.h $
// $Author: Ape $
// $Modtime: 3/03/05 14:24 $
// $Revision: 5 $
// $Workfile: ADN_Connector_ABC.h $
//
//*****************************************************************************

#ifndef __ADN_Connector_ABC_h_
#define __ADN_Connector_ABC_h_

#include "ADN_Ref_ABC.h"

class ADN_Connector_Vector_ABC;


// =============================================================================
/** @class  ADN_Connector_ABC
*/
// Created: APE 2004-12-02
// =============================================================================
class ADN_Connector_ABC 
: public ADN_Ref_ABC
{
    Q_OBJECT

public:
    explicit ADN_Connector_ABC( const char* szName = 0 );
    virtual ~ADN_Connector_ABC();
    
    //! @name Connection
    //@{
    virtual void    Connect( ADN_Ref_ABC* pTarget, bool bConnect );

    virtual void    Connect( ADN_Ref_ABC* pTarget );
    virtual void Disconnect( ADN_Ref_ABC* pTarget );

    virtual void Initialize( ADN_Connector_ABC& dest ) const;
    //@}

    //! @name AutoClear property ($$$$ Get rid of it!)
    //@{
    void SetAutoClear( bool );
    bool IsAutoClear();
    //@}

public slots:
   void SetData( void* pData );

signals:
    void DataChanged( void* pData );

protected:
    //! @name Double dispatch connexion mechanism
    virtual void ConnectPrivate( ADN_Ref_ABC* pTarget );
    virtual void ConnectPrivate( ADN_Connector_ABC* pTarget );
    virtual void ConnectPrivate( ADN_Connector_Vector_ABC* pTarget );
    virtual void DisconnectPrivate( ADN_Ref_ABC* pTarget );
    virtual void DisconnectPrivate( ADN_Connector_ABC* pTarget );
    virtual void DisconnectPrivate( ADN_Connector_Vector_ABC* pTarget );

    virtual void ConnectPrivateSub( ADN_Connector_ABC* pTarget );
    virtual void DisconnectPrivateSub( ADN_Connector_ABC* pTarget );
    //@}

    virtual void SetDataPrivate( void* pData ) = 0;

private:
    bool bConnecting_;
    bool bAutoClear_;
};


typedef std::vector<ADN_Connector_ABC*>          T_ConnectorVector;
typedef T_ConnectorVector::iterator             IT_ConnectorVector;
typedef T_ConnectorVector::const_iterator      CIT_ConnectorVector;


//-----------------------------------------------------------------------------
// Name: ADN_Connector_ABC::SetAutoClear
// Created: JDY 03-09-03
//-----------------------------------------------------------------------------
inline
void  ADN_Connector_ABC::SetAutoClear( bool b )
{
    bAutoClear_ = b;
}


//-----------------------------------------------------------------------------
// Name: ADN_Connector_ABC::IsAutoClear
// Created: JDY 03-09-03
//-----------------------------------------------------------------------------
inline
bool  ADN_Connector_ABC::IsAutoClear()
{
    return bAutoClear_;
}


#endif // __ADN_Connector_ABC_h_