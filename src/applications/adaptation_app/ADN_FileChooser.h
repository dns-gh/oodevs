//*****************************************************************************
//
// $Created: JDY 03-07-01 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_FileChooser.h $
// $Author: Ape $
// $Modtime: 28/02/05 11:13 $
// $Revision: 4 $
// $Workfile: ADN_FileChooser.h $
//
//*****************************************************************************
#ifndef __ADN_FileChooser_h_
#define __ADN_FileChooser_h_

#include "ADN_Connector_ABC.h"
#include <tools/Path.h>

//*****************************************************************************
// Created: JDY 03-07-01
//*****************************************************************************
class ADN_FileChooser : public QWidget
{
    Q_OBJECT

public:
    enum E_Mode
    {
        eFile      = 0,
        eDirectory = 1
    };

public:
    explicit ADN_FileChooser( QWidget* parent, const QString& filter = szDefaultFilter_, const char* szName = 0 );
    virtual ~ADN_FileChooser();

    tools::Path GetFilename() const;
    const tools::Path&  GetDirectory() const;
    E_Mode GetMode() const;
    void SetFilter( const QString& filter );
    void SetRestrictions( const std::vector< std::wstring >& restrictions );
    ADN_Connector_ABC&  GetConnector( E_Mode type );

public slots:
    void SetMode( E_Mode m );
    void SetFilename( const tools::Path& file );
    void SetDirectory( const tools::Path& dir );

private slots:
    void ChooseFile();
    void FilenameChanged( const QString& file );

private:
    QLineEdit* pLineEdit_;
    QPushButton* pButton_;
    E_Mode eMode_;
    T_ConnectorVector vConnectors_;
    QString szFilter_;
    tools::Path szDirectory_;
    std::vector< std::wstring > restrictions_;
    static QString szDefaultFilter_;
};

//-----------------------------------------------------------------------------
// Name: ADN_FileChooser::GetDirectory
// Created: JDY 03-07-01
//-----------------------------------------------------------------------------
inline
const tools::Path& ADN_FileChooser::GetDirectory() const
{
    return szDirectory_;
}

//-----------------------------------------------------------------------------
// Name: ADN_FileChooser::E_Mode
// Created: JDY 03-07-01
//-----------------------------------------------------------------------------
inline
ADN_FileChooser::E_Mode  ADN_FileChooser::GetMode() const
{
    return eMode_;
}

//-----------------------------------------------------------------------------
// Name: ADN_FileChooser::SetFilter
// Created: JDY 03-07-01
//-----------------------------------------------------------------------------
inline
void ADN_FileChooser::SetFilter( const QString& filter )
{
    szFilter_ = filter;
}

//-----------------------------------------------------------------------------
// Name: ADN_FileChooser::GetConverter
// Created: JDY 03-07-01
//-----------------------------------------------------------------------------
inline
ADN_Connector_ABC& ADN_FileChooser::GetConnector( ADN_FileChooser::E_Mode type )
{
    return *vConnectors_[ type ];
}

#endif // __ADN_FileChooser_h_
