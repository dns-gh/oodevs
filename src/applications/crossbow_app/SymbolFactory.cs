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
        }
        
        private IPoint CreatePoint( double x, double y )
        {
            IPoint pPoint = new PointClass();
            pPoint.PutCoords(x, y);
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

        private IForceElement CreateForceElement(string symbolId, string name)
        {                        
            IForceElement pForceElement = new ForceElementClass();
            pForceElement.Shape = CreatePoint(1.0, 1.0);
            pForceElement.MessageString = symbolId;
            pForceElement.PropertySet.SetProperty("Symbol_ID", symbolId);
            pForceElement.PropertySet.SetProperty("Name", name);
            pForceElement.PropertySet.SetProperty("Info", "");
            return pForceElement;    
        }
    
        // Create mole graphic with no scale
        public ICachedGraphic CreateGraphics(string symbolId, string name)
        {
            IFEGraphic pFEGraphic = m_pGraphicFactory.Make(symbolId);
            pFEGraphic.ForceElement = CreateForceElement(symbolId, name);
            return pFEGraphic as ICachedGraphic;
        }
        
        // Create Mole unit with the rigth scale
        private ICachedGraphic CreateMoleUnit(string symbolId, string name, double size)
        {
            ICachedGraphic pGraphic = CreateGraphics(symbolId, name);
            pGraphic.Size = size;
            return pGraphic;
        }

        // Export Bitmap from mole unit graphic
        private static System.Drawing.Bitmap CreateBitmap(IDisplay pDisplay, ICachedGraphic pGraphic, int size)
        {
            ICreateBitmap pBitmap = pGraphic as ICreateBitmap;
            IColor        bgColor = Tools.MakeColor(255, 255, 255);
            int           hDIB = pBitmap.DrawToDIB(pDisplay, size, size, 1.2, bgColor);
            System.IntPtr iPtr = new IntPtr(hDIB);

            return System.Drawing.Bitmap.FromHbitmap(iPtr);
        }

        // Create mole dynamic glyph
        // textureSize in pixels
        // glyphSize in points
        public static IDynamicGlyph CreateDynamicGlyph(IDisplay pDisplay, IDynamicDisplay pDynamicDisplay, ICachedGraphic pGraphic, int textureSize, int glyphSize)
        {
            ICreateBitmap pBitmap = pGraphic as ICreateBitmap;
            IPictureMarkerSymbol pMarker = new PictureMarkerSymbolClass();
            IColor bgColor = Tools.MakeColor(255, 255, 255);
            IDynamicGlyphFactory pFactory = pDynamicDisplay.DynamicGlyphFactory as IDynamicGlyphFactory;

            pMarker.Picture = pBitmap.DrawToPicture(pDisplay, textureSize, textureSize, 1, bgColor);
            pMarker.BitmapTransparencyColor = bgColor;
            pMarker.Size = glyphSize;

            IDynamicGlyph glyph = pFactory.CreateDynamicGlyph(pMarker as ISymbol);
            float width = 0, height = 0;
            glyph.QueryDimensions(ref width, ref height);
            float ratio = height != 0 ? width / height : 1.0f;
            // $$$$ SBO 2007-08-08: hack to handle difference of size between command-post/leveled units...
            pMarker.Size = glyphSize / ratio;
            return pFactory.CreateDynamicGlyph(pMarker as ISymbol);
        }

        public System.Drawing.Bitmap GetSymbol(IDisplay pDisplay, string symbolId, string stName, int size)
        {
            ICachedGraphic pGraphic = CreateMoleUnit(symbolId, stName, 0.5);
            return CreateBitmap(pDisplay, pGraphic, size);
        }
    }
}
