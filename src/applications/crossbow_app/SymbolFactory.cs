using System;
//using System.Collections.Generic;
//using System.Text;
using System.Runtime.InteropServices;
using ESRI.ArcGIS.Display;
using ESRI.ArcGIS.MOLE;
using ESRI.ArcGIS.Geometry;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.esriSystem;

namespace crossbow
{
    [Guid("32b67808-68da-4aca-9f9e-8bb67d2df600")]
    [ClassInterface(ClassInterfaceType.None)]
    [ProgId("crossbow.SymbolFactory")]
    public class SymbolFactory
    {
        private ISpatialReference   m_spatialReference;
        private IFEGraphicFactory   m_graphicFactory;

        #region Factory initialization / configuration
        public SymbolFactory()
        {
            IMoleCoreHelper moleHelper = new MoleCoreHelperClass();
            {
                IForceElement2525BRenderer renderer = (IForceElement2525BRenderer)moleHelper.ForceElementRenderer;
                m_graphicFactory = renderer.GraphicFactory;
            }
        }
        
        public ISpatialReference SpatialReference
        {
            get
            {
                return m_spatialReference;
            }
            set
            {
                m_spatialReference = value;
            }
        }
        #endregion

        #region Factory methods
        public IDynamicElement CreateElement(IDisplay display, IDynamicDisplay dynamicDisplay, IFeature feature, string symbolId)
        {
            return CreateForceElement(display, dynamicDisplay, feature, symbolId);
        }

        public System.Drawing.Bitmap GetSymbol(IDisplay pDisplay, string symbolId, string name, int size)
        {
            ICachedGraphic pGraphic = CreateForceElementGraphics(symbolId, name);
            pGraphic.Size = 0.5; // $$$$ SBO 2007-08-09: size ?
            return CreateBitmap(pDisplay, pGraphic, size);
        }
        #endregion

        #region Force Elements
        private IDynamicElement CreateForceElement(IDisplay display, IDynamicDisplay dynamicDisplay, IFeature feature, string symbolId)
        {
            DynamicForceElement element = new DynamicForceElement();
            element.CachedGraphic = CreateForceElementGraphics(symbolId, "");
            element.Glyph = CreateDynamicGlyph(display, dynamicDisplay, element.CachedGraphic, 128, 64);
            return element as IDynamicElement;
        }

        private ICachedGraphic CreateForceElementGraphics(string symbolId, string name)
        {
            IFEGraphic pFEGraphic = m_graphicFactory.Make(symbolId);
            pFEGraphic.ForceElement = CreateMoleForceElement(symbolId, name);
            return pFEGraphic as ICachedGraphic;
        }

        private IForceElement CreateMoleForceElement(string symbolId, string name)
        {                        
            IForceElement pForceElement = new ForceElementClass();
            pForceElement.Shape = CreatePoint(1.0, 1.0);
            pForceElement.MessageString = symbolId;
            pForceElement.PropertySet.SetProperty("Symbol_ID", symbolId);
            pForceElement.PropertySet.SetProperty("Name", name);
            pForceElement.PropertySet.SetProperty("Info", "");
            return pForceElement;    
        }
    
        // Create dynamic glyph
        // textureSize in pixels
        // glyphSize in points
        private static IDynamicGlyph CreateDynamicGlyph(IDisplay pDisplay, IDynamicDisplay pDynamicDisplay, ICachedGraphic pGraphic, int textureSize, int glyphSize)
        {
            ICreateBitmap pBitmap = pGraphic as ICreateBitmap;
            IPictureMarkerSymbol pMarker = new PictureMarkerSymbolClass();
            IColor bgColor = Tools.MakeColor(255, 255, 255);
            IDynamicGlyphFactory factory = pDynamicDisplay.DynamicGlyphFactory as IDynamicGlyphFactory;

            pMarker.Picture = pBitmap.DrawToPicture(pDisplay, textureSize, textureSize, 1, bgColor);
            pMarker.BitmapTransparencyColor = bgColor;
            pMarker.Size = glyphSize;

            IDynamicGlyph glyph = factory.CreateDynamicGlyph(pMarker as ISymbol);
            float width = 0, height = 0;
            glyph.QueryDimensions(ref width, ref height);
            float ratio = height != 0 ? width / height : 1.0f;
            // $$$$ SBO 2007-08-08: hack to handle difference of size between command-post/leveled units...
            pMarker.Size = glyphSize / ratio;
            return factory.CreateDynamicGlyph(pMarker as ISymbol);
        }
        #endregion

        #region Tactical Elements
        #endregion
        
        #region Tools
        private IPoint CreatePoint(double x, double y)
        {
            IPoint pPoint = new PointClass();
            pPoint.PutCoords(x, y);
            if (m_spatialReference != null)
                pPoint.SpatialReference = m_spatialReference;
            return pPoint;
        }
        
        // Export Bitmap from mole unit graphic
        private static System.Drawing.Bitmap CreateBitmap(IDisplay pDisplay, ICachedGraphic pGraphic, int size)
        {
            ICreateBitmap pBitmap = pGraphic as ICreateBitmap;
            IColor bgColor = Tools.MakeColor(255, 255, 255);
            int hDIB = pBitmap.DrawToDIB(pDisplay, size, size, 1.2, bgColor);
            System.IntPtr iPtr = new IntPtr(hDIB);

            return System.Drawing.Bitmap.FromHbitmap(iPtr);
        }
        #endregion
    }
}
