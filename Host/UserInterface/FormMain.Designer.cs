namespace BicycleHost
{
    partial class FormMain
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要修改
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.listBoxDevices = new System.Windows.Forms.ListBox();
            this.groupBoxDeviceList = new System.Windows.Forms.GroupBox();
            this.groupBoxData = new System.Windows.Forms.GroupBox();
            this.labelDeviceSensorSpeed = new System.Windows.Forms.Label();
            this.labelDeviceIP = new System.Windows.Forms.Label();
            this.labelDeviceName = new System.Windows.Forms.Label();
            this.listBoxConsole = new System.Windows.Forms.ListBox();
            this.labelDeviceState = new System.Windows.Forms.Label();
            this.RefreshTimer = new System.Windows.Forms.Timer(this.components);
            this.labelDeviceVersion = new System.Windows.Forms.Label();
            this.groupBoxDeviceList.SuspendLayout();
            this.groupBoxData.SuspendLayout();
            this.SuspendLayout();
            // 
            // listBoxDevices
            // 
            this.listBoxDevices.FormattingEnabled = true;
            this.listBoxDevices.ItemHeight = 12;
            this.listBoxDevices.Location = new System.Drawing.Point(6, 20);
            this.listBoxDevices.Name = "listBoxDevices";
            this.listBoxDevices.Size = new System.Drawing.Size(138, 364);
            this.listBoxDevices.TabIndex = 2;
            // 
            // groupBoxDeviceList
            // 
            this.groupBoxDeviceList.Controls.Add(this.listBoxDevices);
            this.groupBoxDeviceList.Location = new System.Drawing.Point(2, 3);
            this.groupBoxDeviceList.Name = "groupBoxDeviceList";
            this.groupBoxDeviceList.Size = new System.Drawing.Size(151, 394);
            this.groupBoxDeviceList.TabIndex = 3;
            this.groupBoxDeviceList.TabStop = false;
            this.groupBoxDeviceList.Text = "采集设备列表";
            // 
            // groupBoxData
            // 
            this.groupBoxData.Controls.Add(this.labelDeviceVersion);
            this.groupBoxData.Controls.Add(this.labelDeviceState);
            this.groupBoxData.Controls.Add(this.labelDeviceSensorSpeed);
            this.groupBoxData.Controls.Add(this.labelDeviceIP);
            this.groupBoxData.Controls.Add(this.labelDeviceName);
            this.groupBoxData.Location = new System.Drawing.Point(159, 3);
            this.groupBoxData.Name = "groupBoxData";
            this.groupBoxData.Size = new System.Drawing.Size(561, 394);
            this.groupBoxData.TabIndex = 4;
            this.groupBoxData.TabStop = false;
            this.groupBoxData.Text = "设备数据";
            // 
            // labelDeviceSensorSpeed
            // 
            this.labelDeviceSensorSpeed.AutoSize = true;
            this.labelDeviceSensorSpeed.Location = new System.Drawing.Point(6, 92);
            this.labelDeviceSensorSpeed.Name = "labelDeviceSensorSpeed";
            this.labelDeviceSensorSpeed.Size = new System.Drawing.Size(101, 12);
            this.labelDeviceSensorSpeed.TabIndex = 2;
            this.labelDeviceSensorSpeed.Text = "设备传感器速度：";
            // 
            // labelDeviceIP
            // 
            this.labelDeviceIP.AutoSize = true;
            this.labelDeviceIP.Location = new System.Drawing.Point(6, 68);
            this.labelDeviceIP.Name = "labelDeviceIP";
            this.labelDeviceIP.Size = new System.Drawing.Size(53, 12);
            this.labelDeviceIP.TabIndex = 1;
            this.labelDeviceIP.Text = "设备IP：";
            // 
            // labelDeviceName
            // 
            this.labelDeviceName.AutoSize = true;
            this.labelDeviceName.Location = new System.Drawing.Point(6, 44);
            this.labelDeviceName.Name = "labelDeviceName";
            this.labelDeviceName.Size = new System.Drawing.Size(65, 12);
            this.labelDeviceName.TabIndex = 0;
            this.labelDeviceName.Text = "设备名称：";
            // 
            // listBoxConsole
            // 
            this.listBoxConsole.FormattingEnabled = true;
            this.listBoxConsole.ItemHeight = 12;
            this.listBoxConsole.Location = new System.Drawing.Point(2, 402);
            this.listBoxConsole.Name = "listBoxConsole";
            this.listBoxConsole.ScrollAlwaysVisible = true;
            this.listBoxConsole.Size = new System.Drawing.Size(718, 88);
            this.listBoxConsole.TabIndex = 3;
            // 
            // labelDeviceState
            // 
            this.labelDeviceState.AutoSize = true;
            this.labelDeviceState.Location = new System.Drawing.Point(6, 21);
            this.labelDeviceState.Name = "labelDeviceState";
            this.labelDeviceState.Size = new System.Drawing.Size(65, 12);
            this.labelDeviceState.TabIndex = 3;
            this.labelDeviceState.Text = "设备状态：";
            // 
            // RefreshTimer
            // 
            this.RefreshTimer.Enabled = true;
            this.RefreshTimer.Interval = 500;
            this.RefreshTimer.Tick += new System.EventHandler(this.RefreshTimer_Tick);
            // 
            // labelDeviceVersion
            // 
            this.labelDeviceVersion.AutoSize = true;
            this.labelDeviceVersion.Location = new System.Drawing.Point(6, 115);
            this.labelDeviceVersion.Name = "labelDeviceVersion";
            this.labelDeviceVersion.Size = new System.Drawing.Size(65, 12);
            this.labelDeviceVersion.TabIndex = 4;
            this.labelDeviceVersion.Text = "设备版本：";
            // 
            // FormMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(724, 495);
            this.Controls.Add(this.listBoxConsole);
            this.Controls.Add(this.groupBoxData);
            this.Controls.Add(this.groupBoxDeviceList);
            this.DoubleBuffered = true;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.Name = "FormMain";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Websocket 上位机测试";
            this.Load += new System.EventHandler(this.FormMain_Load);
            this.groupBoxDeviceList.ResumeLayout(false);
            this.groupBoxData.ResumeLayout(false);
            this.groupBoxData.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.ListBox listBoxDevices;
        private System.Windows.Forms.GroupBox groupBoxDeviceList;
        private System.Windows.Forms.GroupBox groupBoxData;
        private System.Windows.Forms.ListBox listBoxConsole;
        private System.Windows.Forms.Label labelDeviceSensorSpeed;
        private System.Windows.Forms.Label labelDeviceIP;
        private System.Windows.Forms.Label labelDeviceName;
        private System.Windows.Forms.Label labelDeviceState;
        private System.Windows.Forms.Timer RefreshTimer;
        private System.Windows.Forms.Label labelDeviceVersion;
    }
}

