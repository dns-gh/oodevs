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
        private IFEGraphicFactory m_pGraphicFactory;

        public SymbolFactory()
        {
            IMoleCoreHelper moleHelper = new MoleCoreHelperClass();
            IForceElement2525BRenderer renderer = (IForceElement2525BRenderer)moleHelper.ForceElementRenderer;
            m_pGraphicFactory = renderer.GraphicFactory;
        }

        public IForceElement CreateForceElement(string stSymbolID, string stName)
        {                        
            IForceElement   pForceElement = new ForceElementClass();
            IPoint          pPoint = new PointClass();
        
            pPoint.PutCoords( 0, 0 );
            // Initialize position
            pForceElement.Shape = pPoint;
            // register symbol id
            pForceElement.MessageString = stSymbolID;
            pForceElement.PropertySet.SetProperty( "Symbol_ID", stSymbolID );
            pForceElement.PropertySet.SetProperty( "Name", stName );
            pForceElement.PropertySet.SetProperty( "Info", "" );
            return pForceElement;    
        }

        public ICachedGraphic CreateGraphics(string stSymbolID, string stName)
        {
            IFEGraphic pFEGraphic;

            pFEGraphic = m_pGraphicFactory.Make(stSymbolID);
            pFEGraphic.ForceElement = CreateForceElement(stSymbolID, stName);
            return (ICachedGraphic)pFEGraphic;
        }

        public stdole.IPictureDisp CreatePicture(IDisplay pDisplay, ICachedGraphic pGraphic, int size)
        {
            ICreateBitmap pBitmap = (ICreateBitmap)pGraphic;
            IColor bgColor = Tools.MakeColor(255, 255, 255);
            return pBitmap.DrawToPicture(pDisplay, size, size, 1.2, bgColor);
        }

        public System.Drawing.Bitmap CreateBitmap(IDisplay pDisplay, ICachedGraphic pGraphic, int size)
        {
            ICreateBitmap   pBitmap = (ICreateBitmap)pGraphic;
            IColor          bgColor = Tools.MakeColor(255, 255, 255);
            int             hDIB = pBitmap.DrawToDIB(pDisplay, size, size, 1, bgColor);
            System.IntPtr   iPtr = new IntPtr(hDIB);

            return System.Drawing.Bitmap.FromHbitmap(iPtr);
        }

        public IDynamicGlyph CreateDynamicGlyph(IDisplay pDisplay, IDynamicDisplay pDynamicDisplay, ICachedGraphic pGraphic, int size)
        {
            ICreateBitmap pBitmap = (ICreateBitmap)pGraphic;
            IPictureMarkerSymbol pMarker = new PictureMarkerSymbolClass();
            IColor bgColor = Tools.MakeColor(255, 255, 255);
            IDynamicGlyphFactory pFactory = (IDynamicGlyphFactory)pDynamicDisplay;

            pMarker.Picture = pBitmap.DrawToPicture(pDisplay, size, size, 1, bgColor);
            pMarker.BitmapTransparencyColor = bgColor;
            pMarker.Size = size;
            return pFactory.CreateDynamicGlyph((ISymbol)pMarker);
        }

        public System.Drawing.Bitmap GetSymbol(IDisplay pDisplay, string stSymbolID, string stName, int size)
        {
            ICachedGraphic pGraphic = CreateGraphics(stSymbolID, stName);

            pGraphic.Size = size;
            return CreateBitmap(pDisplay, pGraphic, 32);
        }
    }
}
