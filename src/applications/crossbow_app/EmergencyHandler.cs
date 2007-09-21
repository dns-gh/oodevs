using System;
using System.Collections;
using System.Collections.Generic;
using System.Xml;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.Geometry;

namespace Crossbow
{    
    public sealed class EmergencyHandler : IMultiItemContextMenuHandler
    {
        private StaticModel m_model;
        private int m_contextMenuX;
        private int m_contextMenuY;
        
        public EmergencyHandler(StaticModel model)
        {
            m_model = model;
        }

        public void OnContextMenu(int x, int y)
        {
            m_contextMenuX = x;
            m_contextMenuY = y;
        }

        #region IMultiItemContextMenuHandler Members

        public void BuildContextMenu(MultiItemContextMenu menu)
        {
            foreach (string type in m_model.ObjectTypes)
                menu.Add(type, null);
        }

        public void ActivateItem(string name, object value)
        {
            string tableName = Tools.GetCSwordExtension().Config.LayersConfiguration.Emergencies;
            IFeatureWorkspace workspace = Tools.OpenWorkspace(tableName);
            IFeatureClass featureClass = workspace.OpenFeatureClass(tableName);
            IFeature feature = featureClass.CreateFeature();
            Tools.SetValue<string>(feature, "type", name);
            feature.Shape = Tools.MakePoint(m_contextMenuX, m_contextMenuY);
            feature.Store();
        }

        public string GetCurrentMessage()
        {
            return "Select emergency type.";
        }

        #endregion
    }
}
