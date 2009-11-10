using System;
using System.Collections.Generic;
using ESRI.ArcGIS.Display;
using ESRI.ArcGIS.DefenseSolutions;
using ESRI.ArcGIS.Geometry;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.esriSystem;

namespace Sword
{
    namespace Crossbow
    {
        public class SymbolFactory
        {
            private ISpatialReference m_spatialReference;
            private IForceElement2525BRenderer m_forceElementRenderer;
            private ITacticalGraphicRenderer m_tacticalElementRenderer;

            // Mapping of Symbol Id's (string) to IDynamicGlyph's
            private Dictionary<string, IDynamicGlyph> m_Glyph = new Dictionary<string, IDynamicGlyph>();

            #region Factory initialization / configuration
            public SymbolFactory()
            {
                IMoleCoreHelper m_moleHelper = new MoleCoreHelperClass();
                m_forceElementRenderer = (IForceElement2525BRenderer)m_moleHelper.ForceElementRenderer;
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
                if (!IsValid(symbolId))
                    return null;
                if (IsEmergencyForceElement(symbolId))
                    return CreateEmergencyForceElement(display, dynamicDisplay, symbolId);
                if (IsTacticalElement(symbolId))
                    return CreateTacticalElement(display, dynamicDisplay, feature, symbolId);
                else
                    return CreateForceElement(display, dynamicDisplay, symbolId);
            }

            private bool IsValid(string symbolId)
            {
                return symbolId != null && symbolId.Trim().Length > 0;
            }

            public System.Drawing.Bitmap GetSymbol(IDisplay pDisplay, string symbolId, string name, int size)
            {
                if (IsEmergencyForceElement(symbolId))
                    return CreateBitmap(pDisplay, symbolId, size);

                ICachedGraphic pGraphic = CreateForceElementGraphic(symbolId, name);
                pGraphic.Size = 0.5; // $$$$ SBO 2007-08-09: size ?
                return CreateBitmap(pDisplay, pGraphic, size);
            }
            #endregion

            #region Emergency Elements
            private IDynamicElement CreateEmergencyForceElement(IDisplay display, IDynamicDisplay dynamicDisplay, string symbolId)
            {
                DynamicForceElement element = new DynamicForceElement();
                if (!m_Glyph.ContainsKey(symbolId))
                {
                    lock (m_Glyph)
                    {
                        m_Glyph[symbolId] = CreateDynamicGlyphFromFile(dynamicDisplay, symbolId, 32, 32);
                    }
                }
                element.Glyph = m_Glyph[symbolId];
                return element as IDynamicElement;
            }
            #endregion
            #region Force Elements
            private IDynamicElement CreateForceElement(IDisplay display, IDynamicDisplay dynamicDisplay, string symbolId)
            {
                DynamicForceElement element = new DynamicForceElement();
                element.CachedGraphic = CreateForceElementGraphic(symbolId, "");
                if (!m_Glyph.ContainsKey(symbolId))
                {
                    lock (m_Glyph)
                    {
                        m_Glyph[symbolId] = CreateDynamicGlyph(display, dynamicDisplay, element.CachedGraphic, 32, 32);
                    }
                }
                element.Glyph = m_Glyph[symbolId];
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
                // element.PropertySet.SetProperty("Parent", name);
                // element.PropertySet.SetProperty("Info", "");
                return element;
            }

            private static string GetFileName(string symbolID, bool large)
            {
                if (large)
                    return Tools.GetExtension().Config.BuildChildFile("\\Resources\\Symbols\\HSWG_Big\\" + symbolID.Substring(0, symbolID.IndexOf('-')) + ".png");
                return Tools.GetExtension().Config.BuildChildFile("\\Resources\\Symbols\\HSWG_Small\\" + symbolID.Substring(0, symbolID.IndexOf('-')) + ".png");
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

            private static IDynamicGlyph CreateDynamicGlyphFromFile(IDynamicDisplay pDynamicDisplay, string symbolID, int textureSize, int glyphSize)
            {
                IColor bgColor = Tools.MakeColor(255, 0, 255);
                bgColor.Transparency = 0;

                // bgColor.NullColor = true;
                // bgColor.Transparency = 0;
                IDynamicGlyphFactory factory = (IDynamicGlyphFactory)pDynamicDisplay.DynamicGlyphFactory;

                string filename = GetFileName(symbolID, false);
                // IPictureMarkerSymbol pMarker = new PictureMarkerSymbolClass();
                // pMarker.CreateMarkerSymbolFromFile(esriIPictureType.esriIPictureEMF, filename);                       
                // pMarker.BitmapTransparencyColor = bgColor;
                // pMarker.Size = glyphSize;
                // return factory.CreateDynamicGlyph(pMarker as ISymbol);
                return factory.CreateDynamicGlyphFromFile(esriDynamicGlyphType.esriDGlyphMarker, filename, bgColor);
            }
            #endregion

            #region Tactical Elements
            private static bool IsTacticalElement(string symbolId)
            {
                return symbolId.StartsWith("G") || symbolId.StartsWith("W");
            }

            private static bool IsEmergencyForceElement(string symbolId)
            {
                return symbolId.Length > 5 && symbolId[5] == 'E';
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

            private static System.Drawing.Bitmap CreateBitmap(IDisplay pDisplay, string symbolID, int size)
            {
                string filename = GetFileName(symbolID, true);

                try
                {
                    System.Drawing.Image image = System.Drawing.Image.FromFile(filename, true);
                    System.Drawing.Bitmap bmp = new System.Drawing.Bitmap(image);
                    bmp.MakeTransparent(System.Drawing.Color.FromArgb(255, 255, 255));
                    return bmp;
                }
                catch (Exception e)
                {
                    System.Diagnostics.Trace.WriteLine(e.Message);
                }
                return null;
            }
            #endregion
        }
    }
}
