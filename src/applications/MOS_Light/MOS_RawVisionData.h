//*****************************************************************************
//
// $Created: JVT 02-11-05 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_RawVisionData.h $
// $Author: Age $
// $Modtime: 2/03/05 16:30 $
// $Revision: 17 $
// $Workfile: MOS_RawVisionData.h $
//
//*****************************************************************************

#ifndef __MOS_RawVisionData_h_
#define __MOS_RawVisionData_h_

#include "MOS_Types.h"
#include "MOS_Perception_Def.h"
#include "MOS_Meteo.h"

#include "MT_Tools/MT_Ellipse.h"

class MT_InputArchive_ABC;
class MT_Sector;
class MOS_SensorType;
class MOS_Agent;
struct ASN1T_MsgStartFireEffect;

//*****************************************************************************
// Created: JVT 02-11-05
// Last modified: JVT 03-07-07
//*****************************************************************************


    //-------------------------------------------------------------------------
    /** @name DEBUG */ // $$$$ JVT : Virer le friend ci dessus
    //-------------------------------------------------------------------------
    //@{
struct vector2DLessXYScaled : public vector2DLessXY
{
    bool operator () ( const MT_Vector2D& rhs, const MT_Vector2D& lhs ) const;
};
    //@}

typedef std::map< MT_Vector2D, E_PerceptionResult, vector2DLessXYScaled >   T_VisionResultMap;
typedef T_VisionResultMap::iterator                                         IT_VisionResultMap;
typedef T_VisionResultMap::const_iterator                                   CIT_VisionResultMap;

class MOS_RawVisionData
{
    MT_COPYNOTALLOWED( MOS_RawVisionData )

public:
    enum E_AlignType
    {
        eTopLeft,
        eTopRight,
        eBottomLeft,
        eBottomRight,
        eCenter
    };

    // environnement visuel statique
    enum E_VisionObject
    {
        eVisionEmpty    = 0x0,
        eVisionForest   = 0x1,
        eVisionUrban    = 0x2,
        eVisionGround   = 0x4,

        eNbrVisionObjects = 4
    };

    typedef uint8 envBits;  // champ de bit

    // cellule de la matrice de vision
    struct sCell
    {
        int16      h     : 16;  // hauteur du sol
        uint8      dh    : 8;   // hauteur de la planimétrie
        envBits    e     : 8;   // champ de bit représentant l'environnement visuel statique
        MOS_Meteo* pMeteo;      // météo locale

        bool operator == ( const sCell& rhs ) const;
    };

    // Parcours brut des cellules traversées par un segment donné
    class Iterator
    {
        MT_COPYNOTALLOWED( Iterator )

    public:
        Iterator( const MT_Vector3D& beginPos, const MT_Vector3D& endPos );

        Iterator& operator ++ ();
    
        bool             End          () const;
        MT_Float         Length       () const;
        bool             IsInGround   () const;
        envBits          GetCurrentEnv() const;
        const MOS_Meteo& GetMeteo()      const;

        const MT_Vector2D          GetPos()               const;
        void                       GetPos( MT_Vector2D& ) const;
    
    protected:
        // Transformations entre l'espace réel et l'espace de l'algorithme
        void ToAlgorithmSpace ( MT_Float& rX, MT_Float& rY ) const;
        void ToAlgorithmSpace ( int& nX, int& nY ) const;
        void ToRealSpace      ( MT_Float& rX, MT_Float& rY ) const;
        void ToRealSpace      ( int& nX, int& nY ) const;
        void OffsetToRealSpace( int& dX, int& dY ) const;

    private:
        inline void AlignFirstPointOnGrid();

    protected:
        // Informations statiques environnementales
        const MOS_RawVisionData& data_;

        // gradients
        /*const*/ MT_Float rDz_;
        /*const*/ MT_Float rDl_;

        // Equations de la droite
        // y = a0 * x + b0
        // x = a1 * y + b1
        /*const*/ MT_Float rA0_;
        /*const*/ MT_Float rA1_;
        /*const*/ MT_Float rB0_;
        /*const*/ MT_Float rB1_;

        // Informations de tranformation de l'espace réel en l'espace idéal ( 0, 45° ) pour l'algo de parcours
        /*const*/ bool bSwap_;
        /*const*/ bool bNegX_;
        /*const*/ bool bNegY_;
        /*const*/ bool bSwapOffset_;
	    
        // Case réelle initiale
        /*const*/ int nCellColOffset_;
        /*const*/ int nCellRowOffset_;

	    // information de fin ( pour faire un end() d'itérateur à la STL )
	    enum E_IteratorState
	    {
		    eRunning,
		    eLastPoint,
		    eEnd
	    };
	    E_IteratorState eIteratorState_;

        // Distance parcourue dans la cellule courrante
        MT_Float rLenght_;

        // Distance totale restant à parcourir ( condition de fin )
        MT_Float rRemainingLength_;
    
        // Prochaine position de l'itérateur
        MT_Float    rAlreadyUsedDX_;        // Projection de la distance déjà parcourue sur l'axe des x dans la précédente itération
        int         nNextCellCol_;
        int         nNextCellRow_;
        MT_Vector3D vOutPoint_;             // X, Y dans l'espace de l'algorithme, Z non transformée
    
        // Informations sur l'environnement précedent
        MT_Float rGroundCoeff_;
        MT_Float rEnvCoeff_;

        // Environnement courrant
        envBits          nCurrentEnv_;
        bool             bIsInGround_;
        const MOS_Meteo* pCurrentMeteo_;
    };

public:
    explicit MOS_RawVisionData();
    ~MOS_RawVisionData();

    //-------------------------------------------------------------------------
    /** @name Init */
    //-------------------------------------------------------------------------
    //@{
    void InitializeHeight( const std::string& strFile );
    void InitializeMeteo( const std::string& strFile );
    void Terminate();
    //@}

    //-------------------------------------------------------------------------
    /** @name Tools */
    //-------------------------------------------------------------------------
    //@{   
    const sCell& operator () ( const MT_Vector2D& ) const;
    const sCell& operator () ( double, double ) const;

    void        AlignToCell( MT_Vector2D& pos, E_AlignType ) const;
    double      GetCellSize() const;
    MT_Float    GetHeight  ( const MT_Vector2D& pos ) const;
    MT_Float    GetHeight  ( MT_Float rX_, MT_Float rY_ ) const;
    MT_Float    GetVisionObjectHeight( const MT_Vector2D& ) const;

    MT_Float    GetMinAltitude() const;
    MT_Float    GetMaxAltitude() const;
    void        CalcMinMaxAltitude();
    bool        IsMeteoUpdated() const;

    static uint ConvertEnvironementToObjectIdx( E_VisionObject obj );
    static E_VisionObject ConvertObjectIdxToEnvironnement( uint );

    void                          RegisterMeteoPatch( const MT_Vector2D&, const MT_Vector2D&, MOS_Meteo* );
    const MOS_Meteo::sEphemeride& GetEphemeride() const;
    void                          UpdateEphemeride();
    void                          UpdateGlobalMeteo( const ASN1T_MeteoAttributs& );

    void  DrawAmmoMeteoEffects     () const;
    void  RegisterAmmoMeteoEffect  ( const ASN1T_MsgStartFireEffect& asnMsg );
    void  UnregisterAmmoMeteoEffect( uint );
    //@}

    //-------------------------------------------------------------------------
    /** @name IN/OUT */
    //-------------------------------------------------------------------------
    //@{
    bool Read( const std::string& strFile );
    //@}

    //-------------------------------------------------------------------------
    /** @name debug */
    //-------------------------------------------------------------------------
    //@{
    void UpdateVisionMatrice( T_VisionResultMap&, const MOS_SensorType&, const MT_Sector&, MT_Float,  const MOS_Agent& ) const;
    //@}

private:
    void               InitializeMeteo( MT_InputArchive_ABC& );
    E_PerceptionResult FindPerception( const MOS_Agent& agent, const MOS_SensorType&, const MT_Vector2D& target, MT_Float rHeight ) const;

private:
    friend class Iterator;
    friend struct vector2DLessXYScaled; // $$$$ JVT : A VIRER

    // Convertisseurs de coordonnées SIM en coordonnées du tableau
    uint            GetCol( double ) const;
    uint            GetRow( double ) const;
    const sCell&    operator () ( uint col, uint row ) const;


    double   rCellSize_; // taille (en metre) du côté de la cellule
    
    uint     nNbrCol_;
    uint     nNbrRow_;

    sCell**  ppCells_;

    MT_Float rMinAltitude_;
    MT_Float rMaxAltitude_;

    MOS_Meteo*             pGlobalMeteo_;
    MOS_Meteo::sEphemeride ephemeride_;

    uint nTimeStepMeteoUpdated_;

    static GFX_Color fumigeneColor_;
    static GFX_Color eclairantColor_;
    std::map< uint, std::pair< const GFX_Color*, const MT_Ellipse* > > ammoMeteoEffects_; 

public:
    const static sCell emptyCell_;
};

#include "MOS_RawVisionData.inl"
#include "MOS_RawVisionDataIterator.inl"


#endif // __MOS_RawVisionData_h_