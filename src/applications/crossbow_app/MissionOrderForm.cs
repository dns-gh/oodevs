using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using ESRI.ArcGIS.Framework;
using ESRI.ArcGIS.esriSystem;

namespace crossbow
{
    public interface IMissionForm
    {
        string OrderName { get; }
        void BindParameter(IOrderParameter param);        
    }

    public partial class MissionOrderForm : Form, IMissionForm
    {
        private OrderManager m_orderManager;
        
        public MissionOrderForm(IApplication app)
        {
            InitializeExtension(app);
            InitializeComponent();
            UnitName = m_orderManager.SelectionName();
        }

        #region Get CSword Extension
        private void InitializeExtension(IApplication app)
        {            
            CSwordExtension extension = Tools.GetCSwordExtension(app);
            m_orderManager = extension.OrderManager;
            m_orderManager.Register(this);
        }
        #endregion

        public void BindParameter(IOrderParameter param)
        {
            m_ParamterTree.Nodes.Add(param.Name);
        }
        
        public string OrderName
        {
            get
            {
                return base.Text;
            }

            set
            {
                base.Text = value;
            }
        }
        public string UnitName
        {
            get
            {
                return m_UnitName.Text;
            }

            set
            {
                m_UnitName.Text = value;
            }
        }
    }
}