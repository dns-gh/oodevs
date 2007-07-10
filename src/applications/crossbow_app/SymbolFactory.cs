using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using ESRI.ArcGIS.Display;
using ESRI.ArcGIS.MOLE;
using ESRI.ArcGIS.Geometry;
using ESRI.ArcGIS.esriSystem;

namespace crossbow
{
    [Guid("32b67808-68da-4aca-9f9e-8bb67d2df600")]
    [ClassInterface(ClassInterfaceType.None)]
    [ProgId("crossbow.SymbolFactory")]
    public class SymbolFactory
    {
        private ISpatialReference   m_SpatialReference;
        private IFEGraphicFactory   m_pGraphicFactory;
        
        public SymbolFactory( )
        {                         
            IMoleCoreHelper moleHelper = new MoleCoreHelperClass();
            IForceElement2525BRenderer renderer = (IForceElement2525BRenderer)moleHelper.ForceElementRenderer;
            m_pGraphicFactory = renderer.GraphicFactory;
            m_SpatialReference = null;
        }
        
        IPoint CreatePoint( double x, double y )
        {
            IPoint pPoint = new PointClass();
            pPoint.PutCoords(1.0, 1.0);
            if (m_SpatialReference != null)
                pPoint.SpatialReference = m_SpatialReference;
            return pPoint;
        }

        public ISpatialReference SpatialReference
        {
            get
            {
                return m_SpatialReference;
            }
            set
            {
                m_SpatialReference = value;
            }
        }

        public IForceElement CreateForceElement(string stSymbolID, string stName)
        {                        
            IForceElement   pForceElement = new ForceElementClass();
            
            // Initialize position
            pForceElement.Shape = CreatePoint(1.0, 1.0);
            // register symbol id            
            pForceElement.MessageString = stSymbolID;
            pForceElement.PropertySet.SetProperty( "Symbol_ID", stSymbolID );
            pForceElement.PropertySet.SetProperty( "Name", stName );
            pForceElement.PropertySet.SetProperty( "Info", "" );
            return pForceElement;    
        }
    
        // Create mole graphic with no scale
        public ICachedGraphic CreateGraphics(string stSymbolID, string stName)
        {
            IFEGraphic pFEGraphic;
            
            pFEGraphic = m_pGraphicFactory.Make(stSymbolID);
            pFEGraphic.Style.FrameOption = moleStyleFrameEnum.moleStyleFrameOn;
            pFEGraphic.Style.FillOption = moleStyleFillEnum.moleStyleFillSolid;
            pFEGraphic.Style.UseFonts = true;
            // IMPORTANT: when using CachedGraphic.Draw method, TTFs are rebuilt each .Draw() call so
            // adversely affects performance (but provide better text quality, flexibility/customizability)
            //((ICachedGraphicStyle)pFEGraphic.Style).UseGDI = true;
            // pFEGraphic.Style.FriendlyLine.Width = 1.5;
            pFEGraphic.ForceElement = CreateForceElement(stSymbolID, stName);
            return (ICachedGraphic)pFEGraphic;
        }
        
        // Create Mole unit with the rigth scale
        public ICachedGraphic CreateMoleUnit(IDisplay pDisplay, string stSymbolID, string stName, double size)
        {
            ICachedGraphic pGraphic = CreateGraphics(stSymbolID, stName);
            pGraphic.Size = size;
            return pGraphic;
        }

        // Export Picture from mole unit graphic
        public stdole.IPictureDisp CreatePicture(IDisplay pDisplay, ICachedGraphic pGraphic, int size)
        {
            ICreateBitmap pBitmap = (ICreateBitmap)pGraphic;
            IColor bgColor = Tools.MakeColor(255, 255, 255);
            return pBitmap.DrawToPicture(pDisplay, size, size, 1.2, bgColor);
        }

        // Export Bitmap from mole unit graphic
        public System.Drawing.Bitmap CreateBitmap(IDisplay pDisplay, ICachedGraphic pGraphic, int size)
        {
            ICreateBitmap   pBitmap = (ICreateBitmap)pGraphic;
            IColor          bgColor = Tools.MakeColor(255, 255, 255);
            int             hDIB = pBitmap.DrawToDIB(pDisplay, size, size, 1.2, bgColor);
            System.IntPtr   iPtr = new IntPtr(hDIB);

            return System.Drawing.Bitmap.FromHbitmap(iPtr);
        }

        // Create mole dynamic glyph
        public IDynamicGlyph CreateDynamicGlyph(IDisplay pDisplay, IDynamicDisplay pDynamicDisplay, ICachedGraphic pGraphic, int size)
        {
            ICreateBitmap pBitmap = pGraphic as ICreateBitmap;
            IPictureMarkerSymbol pMarker = new PictureMarkerSymbolClass();
            IColor bgColor = Tools.MakeColor(255, 255, 255);
            IDynamicGlyphFactory pFactory = pDynamicDisplay.DynamicGlyphFactory as IDynamicGlyphFactory;

            pMarker.Picture = pBitmap.DrawToPicture(pDisplay, size, size, 1.2, bgColor);
            pMarker.BitmapTransparencyColor = bgColor;
            pMarker.Size = size;            
            return pFactory.CreateDynamicGlyph(pMarker as ISymbol);
        }

        public System.Drawing.Bitmap GetSymbol(IDisplay pDisplay, string stSymbolID, string stName, int size)
        {
            ICachedGraphic pGraphic = CreateMoleUnit( pDisplay, stSymbolID, stName, 0.5);
            return CreateBitmap(pDisplay, pGraphic, size);
        }
    }
}
