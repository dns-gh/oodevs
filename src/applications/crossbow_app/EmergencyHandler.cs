using System;
using System.Collections;
using System.Collections.Generic;
using System.Xml;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.Geometry;

namespace Sword
{
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
                IFeatureWorkspace workspace = (IFeatureWorkspace)Tools.OpenWorkspace(Tools.GetCSwordExtension().Config.SharedFile);
                ScopeLockEditor locker = new ScopeLockEditor(workspace);

                locker.Lock();
                try
                {
                    string tableName = Tools.GetCSwordExtension().Config.LayersConfiguration.TacticalObjectPoint;
                    IFeatureClass featureClass = workspace.OpenFeatureClass(tableName);
                    IFeatureBuffer feature = featureClass.CreateFeatureBuffer();
                    // IFeature feature = featureClass.CreateFeature();

                    Tools.SetValue<string>(feature, "Symbol_ID", "GFMPOEIF------X");
                    Tools.SetValue<string>(feature, "Info", name);
                    feature.Shape = CreatePosition(m_contextMenuX, m_contextMenuY);

                    IFeatureCursor featureCursor = featureClass.Insert(true);
                    featureCursor.InsertFeature(feature);
                    featureCursor.Flush();
                    // feature.Store();
                    //Release the Cursor
                    System.Runtime.InteropServices.Marshal.ReleaseComObject(featureCursor);
                    System.Runtime.InteropServices.Marshal.ReleaseComObject(featureClass);
                }
                catch (System.Exception e)
                {
                    locker.Abord();
                    System.Diagnostics.Trace.WriteLine(e.Message);
                }
                locker.Unlock();
            }

            private IGeometry CreatePosition(int x, int y)
            {
                IPoint point = Tools.MakePoint(x, y);
                IZAware zaware = point as IZAware;
                zaware.ZAware = true;
                point.Z = 0;
                return point;
            }

            public string GetCurrentMessage()
            {
                return "Select emergency type.";
            }

            #endregion
        }
    }
}