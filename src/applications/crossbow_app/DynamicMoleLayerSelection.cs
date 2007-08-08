using System;
using System.Timers;
using System.Collections.Generic;
using System.Collections;
using System.Text;
using System.Runtime.InteropServices;
using ESRI.ArcGIS.ADF.BaseClasses;
using ESRI.ArcGIS.ArcMapUI;
using ESRI.ArcGIS.Carto;
using ESRI.ArcGIS.Display;
using ESRI.ArcGIS.Framework;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.Geometry;

namespace crossbow
{
    public partial class DynamicMoleLayer
    {
        // IFeatureSelection attributes
        private IEnumerator             m_FeatureEnumerator = null;
        private ArrayList               m_featureSelection = new ArrayList();
        private ISelectionSet           m_selectionSet;
        private double                  m_bufferDistance = 0;
        private esriSelectionResultEnum m_combinationMethod = esriSelectionResultEnum.esriSelectionResultNew;
        private IColor                  m_selectionColor = null;
        private ISymbol                 m_selectionSymbol = null;
        private bool                    m_useSelectionSymbol = false;

        #region IDataset Members
        public IDataset Dataset
        {
            get
            {
                return m_featureClass as IDataset;
            }
        }
        #endregion    
            
        #region IFeatureSelection Members
        public void Add(IFeature Feature)
        {
            m_featureSelection.Add(Feature);
            m_selectionSet.Add(Feature.OID);
        }
        public double BufferDistance
        {
            get
            {
                return m_bufferDistance;
            }
            set
            {
                m_bufferDistance = value;
            }
        }
        public void Clear()
        {
            m_featureSelection.Clear();            
        }
        public esriSelectionResultEnum CombinationMethod
        {
            get
            {
                return m_combinationMethod;
            }
            set
            {
                m_combinationMethod = value;
            }
        }

        public void SelectFeatures(IQueryFilter Filter, esriSelectionResultEnum Method, bool justOne)
        {
            if (Filter is ISpatialFilter)            
                UpdateEnveloppe(Filter as ISpatialFilter);
            
            esriSelectionOption option = justOne ? esriSelectionOption.esriSelectionOptionOnlyOne : esriSelectionOption.esriSelectionOptionNormal;
            ISelectionSet result = m_featureClass.Select(Filter, esriSelectionType.esriSelectionTypeHybrid, option, Dataset.Workspace);
            UpdateSelection(result, Method);
            UpdateFeatureSelection();
        }

        private void UpdateEnveloppe(ISpatialFilter spatialFilter)
        {
            IDisplayTransformation transformation = Tools.GetMxDocument().ActiveView.ScreenDisplay.DisplayTransformation;
            
            double symbolSize = transformation.FromPoints(m_symbolSize);
            
            IEnvelope envelope = spatialFilter.Geometry.Envelope;
            double symbolWidth = (symbolSize > envelope.Width) ? symbolSize - envelope.Width : envelope.Width - symbolSize;
            double symbolHeight = (symbolSize > envelope.Height) ? symbolSize - envelope.Height : envelope.Height - symbolSize;
            envelope.Expand(symbolWidth / 2, symbolHeight / 2, false);
            spatialFilter.Geometry = envelope;
            m_selectionEnvelope = envelope;
        }

        private void UpdateSelection(ISelectionSet result, esriSelectionResultEnum Method)
        {
            switch (Method)
            {
                case esriSelectionResultEnum.esriSelectionResultAdd:
                    m_selectionSet.Combine(result, esriSetOperation.esriSetUnion, out m_selectionSet);
                    break;
                case esriSelectionResultEnum.esriSelectionResultAnd:
                    m_selectionSet.Combine(result, esriSetOperation.esriSetIntersection, out m_selectionSet);
                    break;
                case esriSelectionResultEnum.esriSelectionResultNew:
                    m_selectionSet = result;
                    break;
                case esriSelectionResultEnum.esriSelectionResultSubtract:
                    m_selectionSet.Combine(result, esriSetOperation.esriSetDifference, out m_selectionSet);
                    break;
                case esriSelectionResultEnum.esriSelectionResultXOR:
                    m_selectionSet.Combine(result, esriSetOperation.esriSetSymDifference, out m_selectionSet);
                    break;
                default:
                    break;
            }
        }

        private void UpdateFeatureSelection()
        {
            m_featureSelection.Clear();            
            IEnumIDs ids = m_selectionSet.IDs;
            ids.Reset();
            int id = ids.Next();
            while (id != -1)
            {
                m_featureSelection.Add(m_featureClass.GetFeature(id));
                id = ids.Next();
            }
            Reset();
        }

        public void SelectionChanged()
        {
            //throw new Exception("The method or operation is not implemented.");
        }
        
        public IColor SelectionColor
        {
            get
            {
                return m_selectionColor;
            }
            set
            {
                m_selectionColor = value;
            }
        }
        public ISelectionSet SelectionSet
        {
            get
            {
                return m_selectionSet;
            }
            set
            {
                m_selectionSet = value;
            }
        }
        public ISymbol SelectionSymbol
        {
            get
            {
                return m_selectionSymbol;
            }
            set
            {
                m_selectionSymbol = value;
            }
        }
        public bool SetSelectionSymbol
        {
            get
            {
                return m_useSelectionSymbol;
            }
            set
            {
                m_useSelectionSymbol = value;
            }
        }
        #endregion

        #region IEnumFeature Members
        public IFeature Next()
        {
            if (m_FeatureEnumerator == null)
                Reset();
            if ( m_FeatureEnumerator.MoveNext() )
                return m_FeatureEnumerator.Current as IFeature;            
            return null;
        }
        
        public void Reset()
        {
            if (m_FeatureEnumerator != null)
                m_FeatureEnumerator = null;
            m_FeatureEnumerator = m_featureSelection.GetEnumerator();            
        }
        #endregion
    }    
}
