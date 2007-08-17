using System;
//using System.Collections.Generic;
//using System.Text;
using System.Runtime.InteropServices;
using ESRI.ArcGIS.Display;
using ESRI.ArcGIS.MOLE;
using ESRI.ArcGIS.Geometry;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.esriSystem;

namespace Crossbow
{
    public class SymbolFactory
    {
        private ISpatialReference m_spatialReference;
        private IForceElement2525BRenderer m_forceElementRenderer;
        private ITacticalGraphicRenderer m_tacticalElementRenderer;

        #region Factory initialization / configuration
        public SymbolFactory()
        {
            IMoleCoreHelper moleHelper = new MoleCoreHelperClass();
            m_forceElementRenderer = (IForceElement2525BRenderer)moleHelper.ForceElementRenderer;
            m_tacticalElementRenderer = new TacticalGraphic2525BRendererClass();
            m_tacticalElementRenderer.UsesAffiliationColor = true;
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
            if (IsTacticalElement(symbolId))
                return CreateTacticalElement(display, dynamicDisplay, feature, symbolId);
            else
                return CreateForceElement(display, dynamicDisplay, symbolId);
        }

        public System.Drawing.Bitmap GetSymbol(IDisplay pDisplay, string symbolId, string name, int size)
        {
            ICachedGraphic pGraphic = CreateForceElementGraphic(symbolId, name);
            pGraphic.Size = 0.5; // $$$$ SBO 2007-08-09: size ?
            return CreateBitmap(pDisplay, pGraphic, size);
        }
        #endregion

        #region Force Elements
        private IDynamicElement CreateForceElement(IDisplay display, IDynamicDisplay dynamicDisplay, string symbolId)
        {
            DynamicForceElement element = new DynamicForceElement();
            element.CachedGraphic = CreateForceElementGraphic(symbolId, "");
            element.Glyph = CreateDynamicGlyph(display, dynamicDisplay, element.CachedGraphic, 128, 64);
            return element as IDynamicElement;
        }

        private ICachedGraphic CreateForceElementGraphic(string symbolId, string name)
        {
            return m_forceElementRenderer.GraphicByForceElement(CreateMoleForceElement(symbolId, name));
        }

        private IForceElement CreateMoleForceElement(string symbolId, string name)
        {                        
            IForceElement element = new ForceElementClass();
            element.Shape = CreatePoint(1.0, 1.0);
            element.MessageString = symbolId;
            element.PropertySet.SetProperty("Symbol_ID", symbolId);
            element.PropertySet.SetProperty("Name", name);
            element.PropertySet.SetProperty("Info", "");
            return element;    
        }
    
        // Create dynamic glyph
        // textureSize in pixels
        // glyphSize in points
        private static IDynamicGlyph CreateDynamicGlyph(IDisplay pDisplay, IDynamicDisplay pDynamicDisplay, ICachedGraphic pGraphic, int textureSize, int glyphSize)
        {
            ICreateBitmap pBitmap = (ICreateBitmap)pGraphic;
            IPictureMarkerSymbol pMarker = new PictureMarkerSymbolClass();
            IColor bgColor = Tools.MakeColor(255, 255, 255);
            IDynamicGlyphFactory factory = (IDynamicGlyphFactory)pDynamicDisplay.DynamicGlyphFactory;

            pMarker.Picture = pBitmap.DrawToPicture(pDisplay, textureSize, textureSize, 1, bgColor);
            pMarker.BitmapTransparencyColor = bgColor;
            pMarker.Size = glyphSize;

            ISymbol symbol = (ISymbol)pMarker;
            IDynamicGlyph glyph = factory.CreateDynamicGlyph(symbol);
            float width = 0, height = 0;
            glyph.QueryDimensions(ref width, ref height);
            float ratio = height != 0 ? width / height : 1.0f;
            // $$$$ SBO 2007-08-08: hack to handle difference of size between command-post/leveled units...
            pMarker.Size = glyphSize / ratio;
            return factory.CreateDynamicGlyph(symbol);
        }
        #endregion

        #region Tactical Elements
        private static bool IsTacticalElement(string symbolId)
        {
            return symbolId.StartsWith("G") || symbolId.StartsWith("W");
        }

        private IDynamicElement CreateTacticalElement(IDisplay display, IDynamicDisplay dynamicDisplay, IFeature feature, string symbolId)
        {
            DynamicForceElement element = new DynamicForceElement();
            element.CachedGraphic = CreateTacticalElementGraphic(display, feature, symbolId);
            element.Glyph = CreateDynamicGlyph(display, dynamicDisplay, element.CachedGraphic, 256, 64);
            return element as IDynamicElement;
        }

        private ICachedGraphic CreateTacticalElementGraphic(IDisplay display, IFeature feature, string symbolId)
        {
            ITacticalElement element = CreateMoleTacticalElement(feature, symbolId);
            if (m_tacticalElementRenderer.CanRenderTacticalElement(element))
            {
                ICachedGraphic graphic = m_tacticalElementRenderer.GraphicByTacticalElement(display, element);
                graphic.Geometry = feature.Shape;
                return graphic;
            }
            return null;
        }

        private ITacticalElement CreateMoleTacticalElement(IFeature feature, string symbolId)
        {
            /*
            IConstructTacticalElement constructor = m_tacticalElementRenderer as IConstructTacticalElement;
            IGraphicDefSet definitions = m_tacticalElementRenderer as IGraphicDefSet;
            IGraphicDef definition = definitions.GraphicDefSet.Next;

            ITacticalElement element = constructor.ReturnSampleElement(mole2525BAffiliationsEnum.moleAffiliationFriend,
                                                                       mole2525BEchelonsEnum.moleEchelonDivision,
                                                                       graphicDef,
                                                                       envelope,
                                                                       Tools.MakePoint(1,1),
                                                                       false);
            return element;
             */
            ITacticalElement element = new TacticalElementClass();
            element.Shape = feature.Shape;
            element.MessageString = symbolId;
            element.PropertySet.SetProperty("Symbol_ID", symbolId);
            return element;
        }
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
            ICreateBitmap pBitmap = (ICreateBitmap)pGraphic;
            IColor bgColor = Tools.MakeColor(255, 255, 255);
            int hDIB = pBitmap.DrawToDIB(pDisplay, size, size, 1.2, bgColor);
            System.IntPtr iPtr = new IntPtr(hDIB);

            return System.Drawing.Bitmap.FromHbitmap(iPtr);
        }
        #endregion
    }
}
