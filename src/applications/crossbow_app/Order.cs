using System;
using System.Collections.Generic;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.Framework;

namespace Sword
{
    namespace Crossbow
    {
        public interface IOrder : IDisposable
        {
            string Name { get; }
            string Target { get; }
            int Id { get; }
            void RegisterParameter(IOrderParameter parameter);
            void OnContextMenu(MultiItemContextMenu menu, int x, int y, IFeature selected);
            void Validate();
        }

        public class Order : IOrder
        {
            private bool m_disposed;
            private ParameterLimits m_limits = new ParameterLimits();
            private ParameterLimas m_limas = new ParameterLimas();
            private ParameterDirection m_direction = new ParameterDirection();
            private ParameterIntelligences m_intelligences = new ParameterIntelligences();
            private Dictionary<string, IOrderParameter> m_parameters = new Dictionary<string, IOrderParameter>();
            private int m_Id;
            private string m_name;
            private string m_target;
            private OrderHandler m_handler;

            private MissionOrderForm m_orderForm;
            private IFeature m_feature;
            public string Name
            {
                get
                {
                    return m_name;
                }
            }
            public string Target
            {
                get
                {
                    return m_target;
                }
            }
            public int Id
            {
                get
                {
                    return m_Id;
                }
                set
                {
                    m_Id = value;
                }
            }

            public Order(string name, OrderHandler handler, IParameterFactory factory)
            {
                m_handler = handler;
                m_feature = handler.TargetFeature;
                m_name = name;
                m_Id = Tools.GetValue<int>(m_feature, "Public_OID");
                m_target = Tools.GetValue<string>(m_feature, "Name");
                m_orderForm = new MissionOrderForm(this);
                factory.CreateParameters(this);
            }

            ~Order()
            {
                Dispose(false);
            }

            public void RegisterParameter(IOrderParameter parameter)
            {
                m_parameters.Add(parameter.Name, parameter);
                parameter.NotifyUpdate(m_orderForm);
            }

            public void SelectParameter(IOrderParameter parameter, string value)
            {
                if (parameter == null)
                    return;
                parameter.SetValue(value);
                parameter.NotifyUpdate(m_orderForm);
            }

            public void OnContextMenu(MultiItemContextMenu menu, int x, int y, IFeature selected)
            {
                m_limits.OnContextMenu(menu, x, y, selected);
                m_limas.OnContextMenu(menu, x, y, selected);
                foreach (KeyValuePair<string, IOrderParameter> param in m_parameters)
                    param.Value.OnContextMenu(menu, x, y, selected);
            }

            public void Validate()
            {
                IFeatureWorkspace workspace = (IFeatureWorkspace)Tools.OpenWorkspace(Tools.GetCSwordExtension().Config.SharedFile);
                if (workspace != null)
                    Serialize(workspace);
            }

            void Serialize(IFeatureWorkspace featureWorkspace)
            {
                ScopeLockEditor editor = new ScopeLockEditor(featureWorkspace);

                editor.Lock();
                int id = SerializeOrder(featureWorkspace);
                SerializeParameters(featureWorkspace, id);
                editor.Unlock();
            }

            int SerializeOrder(IFeatureWorkspace featureWorkspace)
            {
                ITable table = featureWorkspace.OpenTable("Orders"); // $$$$ SBO 2007-07-20: keep it maybe...
                IRow row = table.CreateRow();
                Tools.SetValue<string>(row, "Name", m_name);
                Tools.SetValue<int>(row, "TargetId", m_Id);
                Tools.SetValue<bool>(row, "Checked", false);
                row.Store();
                return row.OID;
            }
            
            void SerializeParameters(IFeatureWorkspace featureWorkspace, int orderId)
            {
                ITable table = featureWorkspace.OpenTable("OrderParameters"); // $$$$ SBO 2007-07-20: keep it maybe...

                // parameters must be serializable following this order
                m_direction.Serialize(table, orderId);
                m_limas.Serialize(table, orderId);
                m_limits.Serialize(table, orderId);
                m_intelligences.Serialize(table, orderId);
                foreach (KeyValuePair<string, IOrderParameter> param in m_parameters)
                    param.Value.Serialize(table, orderId);
            }

            #region IDisposable Members

            public void Dispose()
            {
                Dispose(true);
                GC.SuppressFinalize(this);
            }

            protected virtual void Dispose(bool usercall)
            {
                if (!m_disposed)
                {
                    m_disposed = true;
                    if (usercall)
                        m_orderForm.Dispose();
                    m_handler.Reset();
                }
            }

            #endregion
        }
    }
}