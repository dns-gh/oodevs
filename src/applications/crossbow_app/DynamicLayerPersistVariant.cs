using System;
using ESRI.ArcGIS.esriSystem;

namespace Sword
{
    namespace Crossbow
    {
        public partial class DynamicLayer
        {
            #region IPersistVariant overriden
            public override UID ID
            {
                get
                {
                    UID id = new UIDClass();
                    id.Value = GetType().GUID.ToString("B");
                    return id;
                }
            }

            public override void Load(IVariantStream Stream)
            {
                if (Stream == null)
                    return;
                base.Load(Stream);
                m_extensions = new System.Collections.ArrayList() as ESRI.ArcGIS.esriSystem.IArray;
                int count = (int)Stream.Read();
                for (int i = 0; i < count; ++i)
                    m_extensions.Add(Stream.Read());

                m_selectable = (bool)Stream.Read();
                m_featureClassId = (int)Stream.Read(); m_featureClass = null;
                m_displayField = Stream.Read() as String;
                m_scaleSymbols = (bool)Stream.Read();
                // m_bufferDistance = (double)Stream.Read();
            }

            public override void Save(IVariantStream Stream)
            {
                if (Stream == null)
                    return;
                base.Save(Stream);
                Stream.Write(m_extensions.Count);
                for (int i = 0; i < m_extensions.Count; ++i)
                    Stream.Write(m_extensions.get_Element(i));

                Stream.Write(m_selectable);
                Stream.Write(m_featureClassId);
                Stream.Write(m_displayField);
                Stream.Write(m_scaleSymbols);
                // Stream.Write(m_bufferDistance);
            }

            #endregion
        }
    }
}