using System;
using ESRI.ArcGIS.Display;
using ESRI.ArcGIS.MOLE;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.Geometry;

namespace crossbow
{
    sealed class PointFeatureDrawer : IFeatureDrawer
    {
        private IFeature m_feature;
        private IPoint m_position;
        private DynamicMoleLayer m_layer;

        public PointFeatureDrawer(DynamicMoleLayer layer, IFeature feature)
        {
            m_layer = layer;
            m_feature = feature;
            m_position = feature.Shape as IPoint;
        }

        #region IFeatureDrawer implementation
        public void InitializeDisplay(IDynamicElement symbol, float factor, IDynamicSymbolProperties properties)
        {
            properties.set_DynamicGlyph(esriDynamicSymbolType.esriDSymbolMarker, symbol.Glyph);
        }

        public void DrawEnvelope(IDynamicDisplay dynamicDisplay, double fromPoint, IDynamicSymbolProperties properties)
        {
            // selected element envelope
            IEnvelope envelope = m_position.Envelope.Envelope;
            double selectionHalfSize = fromPoint / 2;
            envelope.Expand(selectionHalfSize, selectionHalfSize, false);
            IDynamicGlyph glyph = m_layer.UpdateGlyphSelection(dynamicDisplay as IDynamicGlyphFactory);

            properties.set_DynamicGlyph(esriDynamicSymbolType.esriDSymbolLine, glyph);
            properties.SetColor(esriDynamicSymbolType.esriDSymbolFill, 0.6f, 0.75f, 1.0f, 0.8f);
            properties.SetColor(esriDynamicSymbolType.esriDSymbolLine, 0.25f, 0.5f, 0.75f, 1.0f);
            dynamicDisplay.DrawRectangle(envelope);
            if (m_layer.Enveloppe != null)
            {
                // draw selection envelope
                properties.SetColor(esriDynamicSymbolType.esriDSymbolFill, 1.0f, 0.75f, 0.6f, 0.8f);
                properties.SetColor(esriDynamicSymbolType.esriDSymbolLine, 0.75f, 0.5f, 0.25f, 1.0f);
                dynamicDisplay.DrawRectangle(m_layer.Enveloppe);
            }
        }

        public void Draw(IDynamicDisplay dynamicDisplay, FieldsProperty m_Fields)
        {
            IDynamicCompoundMarker compoundMarker = dynamicDisplay as IDynamicCompoundMarker;
            compoundMarker.DrawCompoundMarker4(m_position, "", "", (string)m_feature.get_Value(m_Fields.Element[(int)FieldsProperty.EnumFields.eName]), "");
        }
        #endregion
    }
}