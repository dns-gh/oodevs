using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using ESRI.ArcGIS.ADF.BaseClasses;
using ESRI.ArcGIS.Display;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.Geometry;
using ESRI.ArcGIS.MOLE;

namespace crossbow
{
    [Guid("e5798908-98b7-466f-9eee-43e58516f700")]
    [ClassInterface(ClassInterfaceType.None)]
    [ProgId("crossbow.DynamicMoleLayer")]
    public class DynamicMoleLayer : BaseDynamicLayer //, IIdentify
    {
        private class DynamicElement
        {
            public ICachedGraphic graphic;
            public IDynamicGlyph glyph;
            public float sizeRatio;
        };

        private IFeatureClass m_featureClass;
        private System.Collections.Hashtable m_elements;
        private SymbolFactory m_symbolFactory;

        #region class constructor
        public DynamicMoleLayer()
        {
            m_featureClass = null;
            m_symbolFactory = new SymbolFactory();
            m_elements = new System.Collections.Hashtable();
        }
        #endregion

        #region BaseDynamicLayer overriden
        public override void DrawDynamicLayer(esriDynamicDrawPhase dynamicDrawPhase, IDisplay display, IDynamicDisplay dynamicDisplay)
        {
            if (dynamicDisplay == null || dynamicDrawPhase != esriDynamicDrawPhase.esriDDPCompiled)
                return;
            DrawFeatureClass(display, dynamicDisplay);
        }
        #endregion

        public IFeatureClass FeatureClass
        {
            get
            {
                return m_featureClass;
            }
            set
            {
                m_featureClass = value;
            }
        }

        private void DrawFeatureClass(IDisplay display, IDynamicDisplay dynamicDisplay)
        {
            if (m_featureClass == null)
                return;
            IFeatureCursor cursor = m_featureClass.Search(null, false);
            IFeature feature = cursor.NextFeature();
            while (feature != null)
            {
                DrawFeature(display, dynamicDisplay, feature);
                feature = cursor.NextFeature();
            }
        }

        private void DrawFeature(IDisplay display, IDynamicDisplay dynamicDisplay, IFeature feature)
        {
            if (feature == null)
                return;
            DynamicElement symbol = GetSymbol(display, dynamicDisplay, feature);
            if (symbol == null)
                return;

            IPoint position = (IPoint)feature.Shape;
            position.Project(display.DisplayTransformation.SpatialReference);

            double zoom = symbol.sizeRatio * display.DisplayTransformation.FromPoints(symbol.graphic.Size) / display.DisplayTransformation.FittedBounds.Width;
            IDynamicSymbolProperties properties = (IDynamicSymbolProperties)dynamicDisplay;
            properties.set_DynamicGlyph(esriDynamicSymbolType.esriDSymbolMarker, symbol.glyph);
            properties.SetScale(esriDynamicSymbolType.esriDSymbolMarker, (float)zoom, (float)zoom);
            dynamicDisplay.DrawMarker(position);
        }

        private DynamicElement GetSymbol(IDisplay display, IDynamicDisplay dynamicDisplay, IFeature feature)
        {
            String symbolId = Tools.GetFieldValueByName(feature, "Symbol_ID");
            DynamicElement element = (DynamicElement)m_elements[symbolId];
            if (element != null)
                return element;
            element = BuildSymbol(display, dynamicDisplay, symbolId); // throw something
            m_elements[symbolId] = element;
            return element;
        }

        private DynamicElement BuildSymbol(IDisplay display, IDynamicDisplay dynamicDisplay, String symbolId)
        {
            const int size = 100;
            SymbolFactory pFactory = new SymbolFactory();            
            DynamicElement element = new DynamicElement();
            element.graphic = pFactory.CreateGraphics(symbolId, "");
            element.glyph = pFactory.CreateDynamicGlyph(display, dynamicDisplay, element.graphic, size);

            float width = 0, height = 0;
            element.glyph.QueryDimensions(ref width, ref height);
            element.sizeRatio = (float)size / width;
            return element;
        }
    }
}
